// Copyright 2025 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Copyright 2026 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// clang-format off

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_7,    KC_8,    KC_9,    KC_BSPC,
    KC_4,    KC_5,    KC_6,    KC_TAB,
    KC_1,    KC_2,    KC_3,    KC_ESC,
    KC_MINS, KC_0,    KC_DOT,  LT(1, KC_ENT)
  ),
[1] = LAYOUT(
    KC_TRNS,    KC_UP,    KC_TRNS,    KC_TRNS,
    KC_LEFT,    KC_DOWN,    KC_RIGHT,    KC_TRNS,
    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
    KC_TRNS, KC_TRNS,    KC_TRNS,  KC_TRNS
  ),
};


// clang-format on
