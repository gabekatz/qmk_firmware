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

#include "process_tap_hold.h"
#define LS_TAB LSFT(KC_TAB) // Shift-TAB

//RGB_MATRIX_EFFECT(party_size_check)

enum custom_keycodes {
    SOME_KEYCODE = SAFE_RANGE,
    UPDATE_PARTY_SIZE,
    DISPLAY_PARTY_SIZE,
    PARTY_SIZE_HOLD_TAP,
    HOME_END_HOLD_TAP,
    _QK_TAP_HOLD
};

//enum led_commands {
//    LED_PARTY_SIZE,
//    LED_PLAYER_SELECT
//};

//uint8_t selected_led_command = LED_PLAYER_SELECT;

// Place this below the custom keycodes
uint16_t QK_TAP_HOLD = _QK_TAP_HOLD;
#define TH(n) (_QK_TAP_HOLD + n)

// This is where you place you tap-hold actions
// You can use the macros:
// ACTION_TAP_HOLD_SHIFT(KC_TAP, KC_HOLD, KC_TAP_SHIFT, KC_HOLD_SHIFT)
// and ACTION_TAP_HOLD(KC_TAP, KC_HOLD)
int party_size = 1;
int active_player_led_indices[8] = {1, 3, 4, 5, 6, 7, 8, 9};
int target = 0;

//rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
void set_rgb_range_blue(int start, int end) {
    int i = start;
    while (i < end) {
        if (i == active_player_led_indices[target]) {
            rgb_matrix_set_color(i, 0xFF, 0xFF, 0x00);
        } else {
            rgb_matrix_set_color(i, 0x00, 0x00, 0xFF);
        }
        i++;
    }
}

void set_rgb_board_green(void) {
    int i = 0;
    while (i < 9) {
        if (i == active_player_led_indices[target]) {
            rgb_matrix_set_color(i, 0xFF, 0xFF, 0x00);
        } else {
            rgb_matrix_set_color(i, 0x00, 0xFF, 0x00);
        }
        i++;
    }
}

void party_size_check(int party_size_param) {
    if (target == 0) {
        rgb_matrix_set_color(1, 0xFF, 0xFF, 0x00);
    } else {
        rgb_matrix_set_color(1, 0x00, 0x00, 0xFF);
    }
    if (party_size_param < 8) {
        set_rgb_range_blue(3, party_size_param + 2);
    } else {
        set_rgb_board_green();
    }
}

void update_party_size (void) {
    if (party_size == 8) {
        party_size = 1;
    } else {
        party_size = party_size + 1;
    }
}

void player_select (void) {
    rgb_matrix_set_color(active_player_led_indices[target], 0xFF, 0xFF, 0x00);
}

void display_party_size (void) {
    #ifdef RGB_MATRIX_ENABLE
//        selected_led_command = LED_PARTY_SIZE;
        rgb_matrix_indicators_kb();
    #endif
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

enum encoder_names {
    _LEFT,
    _RIGHT,
    _MIDDLE,
};
uint8_t functionKeys[8] = {KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    process_record_tap_hold(keycode, record);
    return true;
};

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

void rgb_matrix_indicators_kb(void) {
        party_size_check(party_size);
}
