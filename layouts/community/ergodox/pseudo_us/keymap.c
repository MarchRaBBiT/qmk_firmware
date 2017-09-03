#include "quantum.h"
#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"
#include "keymap_jis2us.h"
#include "action_pseudo_lut.h"

enum keymap_layout {
  BASE = 0,
  PSEUDO_US,
  DVORAK,
  SYMBOL,
  MEDIA,
  SMEDIA,
};

enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE, // can always be here
  EPRM,
  VRSN,
  RGB_SLD
};

enum tapdance_def {
  TD_LANG = 0,
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 /* # Keymap 0: Basic layer
  #
  # ,--------------------------------------------------.           ,--------------------------------------------------.
  # | Grv    |   1  |   2  |   3  |   4  |   5  |  Esc |           | CADel|   6  |   7  |   8  |   9  |   0  |   =+   |
  # |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
  # | Tab    |   Q  |   W  |   E  |   R  |   T  |  [   |           |   ]  |   Y  |   U  |   I  |   O  |   P  |   -_   |
  # |--------+------+------+------+------+------|  {   |           |   }  |------+------+------+------+------+--------|
  # | LCtrl  |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |   '"   |
  # |--------+------+------+------+------+------|  (   |           |   )  |------+------+------+------+------+--------|
  # | LShift |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |  /?  |   \|   |
  # `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
  #   | Hyper| RCtrl| LAlt | LGUI | MHEN |                                       | HENK | Left |  Up  | Down | Right|
  #   `----------------------------------'                                       `----------------------------------'
  #                                        ,-------------.       ,-------------.
  #                                        |  L1  | PgUp |       | DEL  |  L2  |
  #                                 ,------|------|------|       |------+------+------.
  #                                 |      |      | PgDn |       | BSpc |      |      |
  #                                 | Space|Lshift|------|       |------| LCtrl| Enter|
  #                                 |      |      | SAlt |       | RAlt |      |      |
  #                                 `--------------------'       `--------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = KEYMAP(  // layer 0 : default
        // left hand
        KC_GRV,         KC_1,    KC_2,   KC_3,   KC_4,   KC_5,  KC_ESC,
        KC_TAB,         KC_Q,    KC_W,   KC_E,   KC_R,   KC_T,  KC_LBRC,
        KC_LCTL,        KC_A,    KC_S,   KC_D,   KC_F,   KC_G,
        KC_LSFT,        KC_Z,    KC_X,   KC_C,   KC_V,   KC_B,  KC_LPRN,
        KC_HYPR,        KC_RCTL, KC_LALT,KC_LGUI,TD(TD_LANG),
                                                       TG(MEDIA), KC_PGUP,
                                                                  KC_PGDN,
                                               KC_SPC, KC_RSFT,   LALT(KC_LSFT),
        // right hand
             LALT(LCTL(KC_DEL)),     KC_6,   KC_7,   KC_8,   KC_9,   KC_0,    KC_EQL,
             KC_RBRC,                KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,    KC_MINS,
                                     KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN, KC_QUOT,
             KC_RPRN,                KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH, KC_BSLS,
                                             KC_HENK,KC_LEFT,KC_DOWN,KC_UP,   KC_RGHT,
             KC_DEL,        TG(SYMBOL),
             KC_BSPC,
             KC_RALT,KC_LCTL, KC_ENT
    ),
    /* Keymap 2: Pseudo US Layer */
    [PSEUDO_US] = KEYMAP(
        // left hand
        KC_FN2,         KC_FN2,  KC_FN2, KC_FN2, KC_FN2, KC_FN2, KC_ESC,
        KC_TAB,         KC_Q,    KC_W,   KC_E,   KC_R,   KC_T,   KC_FN2,
        KC_LCTL,        KC_A,    KC_S,   KC_D,   KC_F,   KC_G,
        OSM(MOD_LSFT),  KC_Z,    KC_X,   KC_C,   KC_V,   KC_B,   S(KC_8),
        KC_HYPR,        KC_RCTL, KC_LALT,KC_LGUI,TD(TD_LANG),
                                            TG(MEDIA),    KC_PGUP,
                                                          KC_PGDN,
                                    KC_SPC, OSM(MOD_LSFT),LALT(KC_LSFT),
        // right hand
     LALT(LCTL(KC_DEL)), KC_FN2,   KC_FN2,  KC_FN2,   KC_FN2,  KC_FN2, KC_FN2,
             KC_FN2,     KC_Y,     KC_U  ,  KC_I,     KC_O,    KC_P,   KC_FN2,
                         KC_H,     KC_J,    KC_K,     KC_L,    KC_FN2, KC_FN2,
             S(KC_9),    KC_N,     KC_M,    KC_FN2,   KC_FN2,  KC_SLSH,KC_FN2,
                                   KC_HENK, KC_LEFT,  KC_DOWN, KC_UP,  KC_RGHT,
             KC_DEL,        TG(SYMBOL),
             KC_BSPC,
             KC_RALT,KC_LCTL, KC_ENT
        
    ),
    /* Keymap 2: `DVORAK layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | Esc    |   1  |   2  |   3  |   4  |   5  | MEDIA|           |SYMBOL|   6  |   7  |   8  |   9  |   0  |   -    |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Tab    |   '  |   ,  |   .  |   P  |   Y  |  \|  |           |  /   |   F  |   G  |   C  |   R  |   L  |   /    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | LCtrl  |   A  |   O  |   E  |   U  |   I  |------|           |------|   D  |   H  |   T  |   N  |   S  |   =+   |
 * |--------+------+------+------+------+------| ESC  |           | '"   |------+------+------+------+------+--------|
 * | LShift |   ;  |   Q  |   J  |   K  |   X  |      |           |      |   B  |   M  |   W  |   V  |Z/Ctrl| RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |Grv   | '"   | LGui | Left | Right|                                       |  Up  | Down |   [  |   ]  | ~L1  |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,---------------.
 *                                        | App  | LGui |       | Alt  |Ctrl/Esc|
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      | Home |       | PgUp |        |      |
 *                                 | Space| BS   |------|       |------| Tab    |Enter |
 *                                 |      |      | Enter|       | PgDn |        |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[DVORAK] = KEYMAP(  // layer 0 : default
  // left hand
  KC_ESC,         KC_1,           KC_2,     KC_3,   KC_4,   KC_5,   TG(MEDIA),
  KC_TAB,         KC_QUOT,        KC_COMM,  KC_DOT, KC_P,   KC_Y,   KC_BSLS,
  KC_LCTL,        KC_A,           KC_O,     KC_E,   KC_U,   KC_I,
  KC_LSFT,        KC_SCLN,        KC_Q,     KC_J,   KC_K,   KC_X,   KC_ESC,
  KC_GRV,         KC_LALT,        KC_LGUI,  KC_LBRC,KC_RBRC,
                                        CTL_T(KC_APP),  KC_LGUI,
                                                        KC_CAPS,
                                         KC_SPC,KC_BSPC,KC_ENT,
  // right hand
       TG(SYMBOL),   KC_6,   KC_7,   KC_8,   KC_9,   KC_0,             KC_MINS,
       KC_SLSH,      KC_F,   KC_G,   KC_C,   KC_R,   KC_L,             KC_BSLS,
                     KC_D,   KC_H,   KC_T,   KC_N,   KC_S,             KC_EQL,
       KC_QUOT,      KC_B,   KC_M,   KC_W,   KC_V,   CTL_T(KC_Z),      KC_RSFT,
                     KC_LEFT,KC_DOWN,KC_UP,  KC_RGHT,KC_FN1,
       KC_LALT,       CTL_T(KC_ESC),
       KC_PGUP,
       KC_PGDN,KC_DEL, KC_ENT
), 

/* Keymap 4: Symbol Layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |Version  |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |   !  |   @  |   {  |   }  |   |  |      |           |      |   Up |   7  |   8  |   9  |   *  |   F12  |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |   #  |   $  |   (  |   )  |   `  |------|           |------| Down |   4  |   5  |   6  |   +  |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |   %  |   ^  |   [  |   ]  |   ~  |      |           |      |   &  |   1  |   2  |   3  |   \  |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | EPRM  |      |      |      |      |                                       |      |    . |   0  |   =  |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |Animat|      |       |Toggle|Solid |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |Bright|Bright|      |       |      |Hue-  |Hue+  |
 *                                 |ness- |ness+ |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// SYMBOLS
[SYMBOL] = KEYMAP(
       // left hand
       VRSN,   KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_TRNS,
       KC_TRNS,KC_EXLM,KC_AT,  KC_LCBR,KC_RCBR,KC_PIPE,KC_TRNS,
       KC_TRNS,KC_HASH,KC_DLR, KC_LPRN,KC_RPRN,KC_GRV,
       KC_TRNS,KC_PERC,KC_CIRC,KC_LBRC,KC_RBRC,KC_TILD,KC_TRNS,
          EPRM,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
                                       KC_TRNS,KC_TRNS,
                                               KC_TRNS,
                               KC_TRNS,KC_TRNS,KC_TRNS,
       // right hand
       KC_TRNS, KC_F6,   KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,
       KC_TRNS, KC_UP,   KC_7,   KC_8,    KC_9,    KC_ASTR, KC_F12,
                KC_DOWN, KC_4,   KC_5,    KC_6,    KC_PLUS, KC_TRNS,
       KC_TRNS, KC_AMPR, KC_1,   KC_2,    KC_3,    KC_BSLS, KC_TRNS,
                         KC_TRNS,KC_DOT,  KC_0,    KC_EQL,  KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
),
/* Keymap 5: Media and mouse keys
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |  Up  |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      | Left | Down |Right |      |------|           |------|MsLeft|MsDown| MsUp |MsRght|      |  Play  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      | Prev | Next |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      | Lclk | Rclk |                                       |VolUp |VolDn | Mute |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |MOUSE |      |      |       |      |      |      |
 *                                 |WHEEL |      |------|       |------| Rclk | Lclk |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// MEDIA AND MOUSE
[MEDIA] = KEYMAP(
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS,   KC_UP, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1, KC_BTN2,
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                               MO(SMEDIA), KC_TRNS, KC_TRNS,
    // right hand
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_TRNS, KC_MPLY,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_MPRV, KC_MNXT, KC_TRNS, KC_TRNS,
                          KC_VOLU, KC_VOLD, KC_MUTE, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_BTN2, KC_BTN1
),
[SMEDIA] = KEYMAP(
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS,   KC_UP, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1, KC_BTN2,
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                                  KC_TRNS, KC_TRNS, KC_TRNS,
    // right hand
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, KC_TRNS, KC_MPLY,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_MPRV, KC_MNXT, KC_TRNS, KC_TRNS,
                          KC_VOLU, KC_VOLD, KC_MUTE, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_BTN2, KC_BTN1
),
};

enum function_id {
  PSEUDO_US_FUNCTION,
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
  switch(id) {
    case PSEUDO_US_FUNCTION:
      action_pseudo_lut(record, BASE, keymap_jis2us);
      break;
  }
}
const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_TAP_TOGGLE(SYMBOL),                // FN1 - Momentary Layer 1 (Symbols)
    [2] = ACTION_FUNCTION(PSEUDO_US_FUNCTION),          // FN2 - pseudo US mode
};

void dance_lang (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code (KC_MHEN);
    unregister_code (KC_MHEN);
    register_code (KC_LANG2);
    unregister_code (KC_LANG2);
  } else {
    register_code (KC_HENK);
    unregister_code (KC_HENK);
    register_code (KC_LANG1);
    unregister_code (KC_LANG1);
  }
};

//Tap Dance Definitions
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_LANG]  = ACTION_TAP_DANCE_FN (dance_lang)
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
        if (record->event.pressed) {
          SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
        }
        break;
        case 1:
        if (record->event.pressed) { // For resetting EEPROM
          eeconfig_init();
        }
        break;
      }
    return MACRO_NONE;
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // dynamically generate these.
    case EPRM:
      if (record->event.pressed) {
        eeconfig_init();
      }
      return false;
      break;
    case VRSN:
      if (record->event.pressed) {
        SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
      }
      return false;
      break;
    case RGB_SLD:
      if (record->event.pressed) {
        #ifdef RGBLIGHT_ENABLE
          rgblight_mode(1);
        #endif
      }
      return false;
      break;
  }
  return true;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

};


// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
      // TODO: Make this relevant to the ErgoDox EZ.
        case SYMBOL:
            ergodox_right_led_1_on();
            break;
        case MEDIA:
            ergodox_right_led_2_on();
            break; 
        case SMEDIA:
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
            break;
        default:
            // none
            break;
    }
};
