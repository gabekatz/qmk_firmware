/* Copyright 2020 GhostSeven <work@ghost7.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */
#include QMK_KEYBOARD_H
#include <quantum/rgblight.h>
#include "process_tap_hold.h"
#define LS_TAB LSFT(KC_TAB) // Shift-TAB

enum custom_keycodes {
    SOME_KEYCODE = SAFE_RANGE,
    UPDATE_PARTY_SIZE,
    DISPLAY_PARTY_SIZE,
    PARTY_SIZE_HOLD_TAP,
    HOME_END_HOLD_TAP,
    _QK_TAP_HOLD
};

// Place this below the custom keycodes
uint16_t QK_TAP_HOLD = _QK_TAP_HOLD;
#define TH(n) (_QK_TAP_HOLD + n)

// This is where you place you tap-hold actions
// You can use the macros:
// ACTION_TAP_HOLD_SHIFT(KC_TAP, KC_HOLD, KC_TAP_SHIFT, KC_HOLD_SHIFT)
// and ACTION_TAP_HOLD(KC_TAP, KC_HOLD)
int party_size = 1;

void party_size_check(int party_size_param) {
//    if (party_size > 1) {
    rgblight_sethsv_at(HSV_BLUE, 1);
    if (party_size_param < 8) {
//            ffxiv_led = RGBLIGHT_LAYER_SEGMENTS(
//                {1, 1, HSV_RED},
//                {3, (party_size - 1), HSV_RED}
//            );

        rgblight_sethsv_range(HSV_BLUE, 3, party_size_param - 1);
    } else {
        rgblight_sethsv_range(HSV_GREEN, 0, 9);
//            ffxiv_led = RGBLIGHT_LAYER_BLINK(
//                {1, 1, HSV_RED},
//                {3, 6, HSV_RED}
//            );
    }
//    } else {
//        ffxiv_led = RGBLIGHT_LAYER_SEGMENTS(
//            {1,1, HSV_RED}
//        );
//    }
}

void update_party_size (void) {
//    if (record->event.pressed) {
        if (party_size == 8) {
            party_size = 1;
        } else {
            party_size = party_size + 1;
        }
//    }
}

void display_party_size (void) {
//    if (record->event.pressed) {
        party_size_check(party_size);
//    } else {
//        rgblight_toggle();
//    }
}

tap_hold_action_t tap_hold_actions[] = {
    [0] = ACTION_TAP_HOLD(KC_END, KC_HOME),
    [1] = ACTION_TAP_HOLD(UPDATE_PARTY_SIZE, DISPLAY_PARTY_SIZE)
};

uint16_t QK_TAP_HOLD_MACROS[] = {
    [0] = UPDATE_PARTY_SIZE,
    [1] = DISPLAY_PARTY_SIZE
};

func_type QK_TAP_HOLD_MACRO_FUNCTIONS[] = {
    [0] = update_party_size,
    [1] = display_party_size
};

void matrix_scan_user(void) {
    matrix_scan_tap_hold(); // Place this function call here
}

//enum layer_names {
//    0 = 0,
//    1 = 1,
//    2 = 2
//};

enum encoder_names {
    _LEFT,
    _RIGHT,
    _MIDDLE,
};

//rgblight_segment_t default_ffxiv_led =
//
//rgblight_segment_t ffxiv_led = default_ffxiv_led;

// Light LEDs 6 to 9 and 12 to 15 red when caps lock is active. Hard to ignore!
const rgblight_segment_t PROGMEM default_layer[] = RGBLIGHT_LAYER_SEGMENTS(
{RGBLIGHT_EFFECT_BREATHING}
);
// Light LEDs 9 & 10 in cyan when keyboard layer 1 is active
const rgblight_segment_t PROGMEM ffxiv_layer[] = RGBLIGHT_LAYER_SEGMENTS(
{RGBLIGHT_EFFECT_STATIC_GRADIENT}
);

const rgblight_segment_t PROGMEM intellij_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 9, HSV_PURPLE}
);

// etc..
//make a switch case
uint8_t functionKeys[8] = {KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8};
int target = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    process_record_tap_hold(keycode, record);
    switch (keycode) {
        case UPDATE_PARTY_SIZE:

            break;
        case DISPLAY_PARTY_SIZE:
//            if (record->event.pressed) {
//                party_size_check(party_size);
//            } else {
//                rgblight_toggle();
//            }
            break;
    }
    return true;
};

// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    default_layer,
    ffxiv_layer,    // Overrides caps lock layer
    intellij_layer     // Overrides other layers
);

void keyboard_post_init_user(void) {
    // Enable the LED layers
//    rgblight_layers = my_rgb_layers;
}
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
        | Mute (Vol Dn/Up) |  Toggle layer 1   | Knob 2: Page Dn/Up |
        | RGB cycle         | Home              | Press: Play/Pause  |
        | Media Previous    | Press: Play/Pause | Media Next         |
     */
    [0] = LAYOUT(
        KC_MUTE, TG(1), TH(0),
        RGB_MOD, KC_PGUP, KC_PGDN,
        KC_MPRV, KC_MPLY, KC_MNXT
    ),
    /*
        | F11 (Tab-Shift Tab) | Toggle layer 2 | F1 (F1-F8)|
        | Grave               | End            | Plus      |
        | Media Previous | End  | Media Next |
     */
    [1] = LAYOUT(
        KC_F11, TG(2), TH(1),
        KC_GRAVE, KC_END, KC_KP_PLUS,
        KC_LEFT, LCTL(KC_END), KC_RGHT
    ),
    /*
        | RESET          | Shift+CMD+B (Build VS Code) | Media Stop |
        | Held: Layer 2  | Home | RGB Mode   |
        | Media Previous | End  | Media Next |
    */
    [2] = LAYOUT(
        RESET, TO(0), KC_STOP,
        S(G(KC_G)), KC_HOME, RGB_MOD,
        KC_MPRV, KC_END, KC_MNXT
    ),
};

void left_rotary_encoder(bool clockwise) {
    uint8_t layer = get_highest_layer(layer_state);
    switch (layer) {
        case 1:
            if (clockwise) {
                tap_code(KC_TAB);
            } else {
                tap_code16(LS_TAB);
            }
            break;

        default:
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
    }
}

void right_rotary_encoder(bool clockwise) {
    uint8_t layer = get_highest_layer(layer_state);
    switch (layer) {
        case 1:
            if (clockwise) {
                target = target + 1;
                if (target > party_size - 1) {
                    target = 0;
                }
                tap_code(functionKeys[target]);
            } else {
                target = target - 1;
                if (target < 0) {
                    target = party_size - 1;
                }
                tap_code(functionKeys[target]);
            }
            break;

        default:
            if (clockwise) {
                tap_code(KC_MS_WH_UP);
            } else {
                tap_code(KC_MS_WH_DOWN);
            }
    }
}

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == _LEFT) {
        left_rotary_encoder(clockwise);
    } else if (index == _RIGHT) {
        right_rotary_encoder(clockwise);
    }
}
