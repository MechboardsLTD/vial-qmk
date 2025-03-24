// Copyright 2025 Mechboards
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "analog.h"
#include "gpio.h"
#include "debug.h"

uint16_t          axis_reading[JOYSTICK_AXIS_COUNT];
joystick_config_t joystick_axes[JOYSTICK_AXIS_COUNT] = {
    JOYSTICK_AXIS_IN(GP29, 1023, 520, 3),
    JOYSTICK_AXIS_IN(GP28, 3, 520, 1023),
};

typedef union {
    uint32_t raw;
    struct {
        bool joystick_analog_mode : 1;
    };
} kb_config_t;

kb_config_t kb_config;

void keyboard_post_init_kb(void) {
    // debug_enable = true;
    // debug_matrix = true;

    kb_config.raw = eeconfig_read_kb();
    keyboard_post_init_user();
};

void eeconfig_init_user(void) {
    kb_config.raw                  = 0;
    kb_config.joystick_analog_mode = true; // Joystick analog mode by default.
    eeconfig_update_kb(kb_config.raw);     // Write default value to EEPROM now
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case JOY_ANA:
            if (record->event.pressed) {
                kb_config.joystick_analog_mode = true;
                eeconfig_update_kb(kb_config.raw);
            }
            return false;
        case JOY_KEY:
            if (record->event.pressed) {
                kb_config.joystick_analog_mode = false;
                eeconfig_update_kb(kb_config.raw);
            }
            return false;
        case JOY_TOG:
            if (record->event.pressed) {
                kb_config.joystick_analog_mode = !kb_config.joystick_analog_mode;
                eeconfig_update_kb(kb_config.raw);
            }
            return false;
        case JOY_CAL:
            if (record->event.pressed) {
                // Reset mid point
                axis_reading[0]            = analogReadPin(joystick_axes[0].input_pin);
                joystick_axes[0].mid_digit = axis_reading[0];
                axis_reading[1]            = analogReadPin(joystick_axes[1].input_pin);
                joystick_axes[1].mid_digit = axis_reading[1];
            }
            return false;
        default:
            return true; // Process all other keycodes normally
    }
}

__attribute__((weak)) void joystick_axis_init(uint8_t axis) {
    if (axis >= JOYSTICK_AXIS_COUNT) return;

    gpio_set_pin_input(joystick_axes[axis].input_pin);
    wait_ms(1);
    analogReadPin(joystick_axes[axis].input_pin);                                 // Discord first reading, necessary?
    axis_reading[axis]            = analogReadPin(joystick_axes[axis].input_pin); // Store reading
    joystick_axes[axis].mid_digit = axis_reading[axis];                           // Set mid point on startup. (Auto cal for any drift (hopefully))
}

uint16_t joystick_axis_sample(uint8_t axis) {
    if (axis >= JOYSTICK_AXIS_COUNT) return 0;
    axis_reading[axis] = analogReadPin(joystick_axes[axis].input_pin);
    dprintf("Axis %d: %d\n", axis, axis_reading[axis]);
    return kb_config.joystick_analog_mode ? axis_reading[axis] : joystick_axes[axis].mid_digit; // only send updates if in analog mode
}

// A lot of duplicated code from matrix.c but activates matrix locations using the joystick when not in analog mode.
// A hack to allow dynamic mapping using via/vial.
// The deadzone is important so keys aren't always being pressed.
#define DEADZONE_RANGE 10

static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;
static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;

static inline void gpio_atomic_set_pin_output_low(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        gpio_set_pin_output(pin);
        gpio_write_pin_low(pin);
    }
}

static inline void gpio_atomic_set_pin_input_high(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        gpio_set_pin_input_high(pin);
    }
}

static bool select_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
        gpio_atomic_set_pin_output_low(pin);
        return true;
    }
    return false;
}

static void unselect_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
#ifdef MATRIX_UNSELECT_DRIVE_HIGH
        gpio_atomic_set_pin_output_high(pin);
#else
        gpio_atomic_set_pin_input_high(pin);
#endif
    }
}

static inline uint8_t readMatrixPin(pin_t pin) {
    if (pin != NO_PIN) {
        return (gpio_read_pin(pin) == 0) ? 0 : 1;
    } else {
        return 1;
    }
}

void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    if (!select_row(current_row)) { // Select row
        if (kb_config.joystick_analog_mode) {
            return;
        } else {
            // Left = 1, Right = 2, Up = 0, Down = 3
            if (axis_reading[0] > (joystick_axes[0].mid_digit + DEADZONE_RANGE)) {
                current_row_value |= 1 << 1;
            } else if (axis_reading[0] < (joystick_axes[0].mid_digit - DEADZONE_RANGE)) {
                current_row_value |= 1 << 2;
            }

            if (axis_reading[1] < (joystick_axes[1].mid_digit - DEADZONE_RANGE)) {
                current_row_value |= 1 << 0;
            } else if (axis_reading[1] > (joystick_axes[1].mid_digit + DEADZONE_RANGE)) {
                current_row_value |= 1 << 3;
            }
        }
    } else {
        matrix_output_select_delay();

        // For each col...
        matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
        for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++, row_shifter <<= 1) {
            uint8_t pin_state = readMatrixPin(col_pins[col_index]);

            // Populate the matrix row with the state of the col pin
            current_row_value |= pin_state ? 0 : row_shifter;
        }

        // Unselect row
        unselect_row(current_row);
        matrix_output_unselect_delay(current_row, current_row_value != 0); // wait for all Col signals to go HIGH
    }

    // Update the matrix
    current_matrix[current_row] = current_row_value;
}