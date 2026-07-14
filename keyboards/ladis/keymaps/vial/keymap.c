// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[0] = LAYOUT(
		KC_F13, 	KC_F14, 	KC_F15,
		KC_F16, 	KC_F17, 	KC_F18,
		KC_LCTL, 	KC_F19, 	KC_F20,
		KC_HOME,	KC_MUTE
		),
[1] = LAYOUT(
		KC_TRNS, 	KC_TRNS, 	KC_TRNS,
		KC_TRNS, 	KC_TRNS, 	KC_TRNS,
		KC_TRNS, 	KC_TRNS, 	KC_TRNS,
		KC_TRNS, 	KC_TRNS
		),

};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] =   {
		ENCODER_CCW_CW(MS_WHLD,MS_WHLU),
		ENCODER_CCW_CW(KC_VOLD,KC_VOLU)
		},
	[1] =   {
		ENCODER_CCW_CW(KC_TRNS, KC_TRNS),
		ENCODER_CCW_CW(KC_TRNS, KC_TRNS)
		},
};
#endif

bool oled_task_user(void) {
    // write custom code here to edit OLED display and return false at end.
    return true; // return false to disable logo and layer indicator
}

