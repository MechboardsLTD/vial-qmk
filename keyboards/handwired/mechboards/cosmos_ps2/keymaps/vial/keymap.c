// Copyright 2025 Mechboards
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(

        KC_ESC , KC_1  , KC_2  , KC_3  , KC_4  , KC_5  ,
        KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,
        KC_LSFT, KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,
        KC_LCTL, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  ,      KC_UP  ,
                         KC_LBRC,KC_RBRC,            KC_LEFT ,JOY_TOG , KC_RIGHT,
                                                              KC_DOWN, 
                                                              
                                                              KC_ENTER
    ),
};
// clang-format on
