// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum uno_keycode { UNO = QK_KB_0 };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {[0] = LAYOUT(UNO)};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case UNO:
            if (record->event.pressed) {
                SEND_STRING("Hello!");
            }
            break;
            return false;
    }
    return true;
}

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
};
#endif
