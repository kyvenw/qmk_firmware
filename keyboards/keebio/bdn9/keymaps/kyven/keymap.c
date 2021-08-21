/* Copyright 2019 Danny Nguyen <danny@keeb.io>
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
 */
#include QMK_KEYBOARD_H

enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
};

enum my_keycodes {
  TOGGLE_BASE_LAYER = SAFE_RANGE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
        | Knob 1: Vol Dn/Up |      | Knob 2: Page Dn/Up |
        | Press: Mute       | Home | Press: Play/Pause  |
        | Hold: Layer 2     | Up   | RGB Mode           |
        | Left              | Down | Right              |
     */
    [0] = LAYOUT(
        KC_MUTE, KC_HOME, KC_MPLY,
        MO(2)  , KC_UP  , RGB_MOD,
        KC_LEFT, KC_DOWN, KC_RGHT
    ),
    /*
        | RESET          | N/A  | Media Stop |
        | Held: Layer 2  | Home | RGB Mode   |
        | Media Previous | End  | Media Next |
     */
    [1] = LAYOUT(
        KC_MUTE, KC_HOME, KC_MPLY,
        MO(3)  , KC_UP  , RGB_MOD,
        KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [2] = LAYOUT(
        RESET  , BL_STEP, KC_STOP,
        _______, KC_HOME, TOGGLE_BASE_LAYER,
        KC_MPRV, KC_END , KC_MNXT
    ),

    [3] = LAYOUT(
        RESET  , BL_STEP, KC_STOP,
        _______, KC_HOME, TOGGLE_BASE_LAYER,
        KC_MPRV, KC_END , KC_MNXT
    )
};

layer_state_t layer_state_set_kb(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case 0:
            rgblight_sethsv_cyan();
            break;
        case 1:
            rgblight_sethsv_orange();
            break;
        case 2:
            rgblight_sethsv_blue();
            break;
        case 3:
            rgblight_sethsv_red();
            break;
        default:
            rgblight_sethsv_white();
            break;
    }

    return state;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == _LEFT) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    else if (index == _MIDDLE) {
        if (clockwise) {
          tap_code(KC_PGDN);
        } else {
          tap_code(KC_PGUP);
        }
    }
    else if (index == _RIGHT) {
        if (clockwise) {
            if (get_highest_layer(layer_state) == 0 || get_highest_layer(layer_state) == 2) {
                tap_code16(G(KC_Y));
            } else {
                tap_code16(C(KC_Y));
            }
        } else {
            if (get_highest_layer(layer_state) == 0 || get_highest_layer(layer_state) == 2) {
                tap_code16(G(KC_Z));
            } else {
                tap_code16(C(KC_Z));
            }
        }
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case TOGGLE_BASE_LAYER:
      if (record->event.pressed) {
        layer_debug();
        // Toggle layers
        if (get_highest_layer(layer_state) == 2) {
            set_single_persistent_default_layer(1);
            layer_move(3);
            layer_on(1);
        } else {
            set_single_persistent_default_layer(0);
            layer_move(2);
            layer_on(0);
        }
      }
      return true; // Skip all further processing of this key
    default:
      return true; // Process all other keycodes normally
  }
}
