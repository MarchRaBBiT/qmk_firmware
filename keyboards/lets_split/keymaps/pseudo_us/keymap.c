#include QMK_KEYBOARD_H
#include "lets_split.h"
#include "action_layer.h"
#include "keymap_jp.h"
#include "eeconfig.h"
#include "keymap_jis2us.h"
#include "action_pseudo_lut.h"

extern keymap_config_t keymap_config;
void dance_lang (qk_tap_dance_state_t *state, void *user_data);

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 6
#define _NORMAN 7
#define _DVORAK 8
#define _EUCALYN 9
#define _LOWER 10
#define _RAISE 11
#define _QWERTY_JP 0
#define _NORMAN_JP 1
#define _DVORAK_JP 2
#define _EUCALYN_JP 3
#define _LOWER_JP 4
#define _RAISE_JP 5
#define _ADJUST 16

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  NORMAN,
  DVORAK,
  EUCALYN,
  LOWER,
  RAISE,
  ADJUST,
};

enum tapdance_def {
  TD_LANG = 0,
};

enum function_id {
  PSEUDO_US_FUNCTION_Q,
  PSEUDO_US_FUNCTION_N,
  PSEUDO_US_FUNCTION_D,
  PSEUDO_US_FUNCTION_E,
  PSEUDO_US_FUNCTION_L,
  PSEUDO_US_FUNCTION_R,
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |Adjust| Ctrl | Alt  | GUI  |Lower |Space |Space |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY_JP] = KEYMAP( \
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
  KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_FN1,  KC_FN1, \
  OSM(MOD_LSFT), KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_FN1,  KC_ENT , \
  ADJUST,  KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
),
[_QWERTY] = KEYMAP( \
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
  KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT , \
  ADJUST,  KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
),

/* Norman
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   D  |   F  |   K  |   J  |   U  |   R  |   L  |   ;  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   E  |   T  |   G  |   Y  |   N  |   I  |   O  |   H  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   P  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |Adjust| Ctrl | Alt  | GUI  |Lower |Space |Space |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_NORMAN_JP] = KEYMAP( \
  KC_TAB,        KC_Q,    KC_W,    KC_D,    KC_F,    KC_K,    KC_J,    KC_U,    KC_R,    KC_L,    KC_FN2,  KC_BSPC, \
  KC_ESC,        KC_A,    KC_S,    KC_E,    KC_T,    KC_G,    KC_Y,    KC_N,    KC_I,    KC_O,    KC_H,    KC_FN2,  \
  OSM(MOD_LSFT), KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_P,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT , \
  ADJUST,        KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
),
[_NORMAN] = KEYMAP( \
  KC_TAB,  KC_Q,    KC_W,    KC_D,    KC_F,    KC_K,    KC_J,    KC_U,    KC_R,    KC_L,    KC_SCLN, KC_BSPC, \
  KC_ESC,  KC_A,    KC_S,    KC_E,    KC_T,    KC_G,    KC_Y,    KC_N,    KC_I,    KC_O,    KC_H,    KC_QUOT, \
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_P,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT , \
  ADJUST,  KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
),

/* Dvorak
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   "  |   ,  |   .  |   P  |   Y  |   F  |   G  |   C  |   R  |   L  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   O  |   E  |   U  |   I  |   D  |   H  |   T  |   N  |   S  |  /   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   ;  |   Q  |   J  |   K  |   X  |   B  |   M  |   W  |   V  |   Z  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |Adjust| Ctrl | Alt  | GUI  |Lower |Space |Space |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_DVORAK_JP] = KEYMAP( \
  KC_TAB,  KC_FN3,  KC_COMM, KC_DOT,  KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_BSPC, \
  KC_ESC,  KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH, \
  OSM(MOD_LSFT), KC_FN3,  KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_ENT , \
  ADJUST,  KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
),
[_DVORAK] = KEYMAP( \
  KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_BSPC, \
  KC_ESC,  KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH, \
  KC_LSFT, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_ENT , \
  ADJUST,  KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
),

/* Eucalyn
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   "  |   ,  |   .  |   F  |   M  |   R  |   D  |   Y  |   P  |   L  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   O  |   E  |   U  |   I  |   G  |   T  |   K  |   S  |   N  |  /   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   W  |   B  |   H  |   J  |   L  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |Adjust| Ctrl | Alt  | GUI  |Lower |Space |Space |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_EUCALYN] = KEYMAP( \
  KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_F,    KC_M,    KC_R,    KC_D,    KC_Y,    KC_P,    KC_L,    KC_BSPC, \
  KC_ESC,  KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_G,    KC_T,    KC_K,    KC_S,    KC_N,    KC_SLSH, \
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_W,    KC_B,    KC_H,    KC_J,    KC_L,    KC_SCLN, KC_ENT , \
  ADJUST,  KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
),
[_EUCALYN_JP] = KEYMAP( \
  KC_TAB,  KC_FN4,  KC_COMM, KC_DOT,  KC_F,    KC_M,    KC_R,    KC_D,    KC_Y,    KC_P,    KC_L,    KC_BSPC, \
  KC_ESC,  KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_G,    KC_T,    KC_K,    KC_S,    KC_N,    KC_SLSH, \
  OSM(MOD_LSFT), KC_Z,    KC_X,    KC_C,    KC_V,    KC_W,    KC_B,    KC_H,    KC_J,    KC_L,    KC_FN4,  KC_ENT , \
  ADJUST,  KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   +  |   _  |   {  |   }  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  |  |   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO ~ |ISO | |      |      |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER_JP] = KEYMAP( \
  KC_FN5,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   JP_PLUS, JP_UNDS, JP_LCBR, JP_RCBR,     KC_BSPC,  \
  KC_DEL,  KC_EXLM, JP_AT,   KC_HASH, KC_DLR,  KC_PERC, JP_CIRC, JP_AMPR, JP_ASTR, JP_LPRN, JP_RPRN,     JP_PIPE, \
  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______, TD(TD_LANG), _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU,     KC_MPLY \
),[_LOWER] = KEYMAP( \
  KC_TILD, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_BSPC, \
  KC_DEL,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE, \
  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY \
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   `  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   =  |   -  |   [  |   ]  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |  \   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |      |      |      |      |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE_JP] = KEYMAP( \
  KC_FN6,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_FN6,  KC_FN6,  KC_FN6,  KC_FN6,      KC_BSPC, \
  KC_DEL,  KC_FN6,  KC_FN6,  KC_FN6,  KC_FN6,  KC_FN6,  KC_FN6,  KC_FN6,  KC_FN6,  KC_FN6,  KC_0,        KC_FN6, \
  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______, TD(TD_LANG), _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU,     KC_MPLY \
),
[_RAISE] = KEYMAP( \
  KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_EQL,  KC_MINS, KC_LBRC, KC_RBRC, KC_BSPC,  \
  KC_DEL,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSLS, \
  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY \
),

/* Adjust (Lower + Raise)
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|      |      |      |      |      |      |      |      |      |  Del |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |Aud on|Audoff|AGnorm|AGswap|Qwerty|Norman|Dvorak|Eucaln|      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      | Lock |      |      |      |      |HueUp |HueDn |SaUp  |SaDn  |VaUp  |VaDn  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] =  KEYMAP( \
  _______, RESET,   _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL, \
  _______, _______, _______, AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  NORMAN,  DVORAK,  EUCALYN, _______, \
  _______, KC_EJCT, _______, _______, _______, _______, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, \
  _______, _______, _______, _______, _______, _______, _______, _______, RGB_M_B, RGB_M_R, RGB_M_K, RGB_M_SW \
)


};

#ifdef AUDIO_ENABLE
float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
float tone_dvorak[][2]     = SONG(DVORAK_SOUND);
float tone_norman[][2]    = SONG(COLEMAK_SOUND);
#endif

const uint16_t PROGMEM fn_actions[] = {
  [1] = ACTION_FUNCTION(PSEUDO_US_FUNCTION_Q),          // FN1 - pseudo US mode
  [2] = ACTION_FUNCTION(PSEUDO_US_FUNCTION_N),          // FN1 - pseudo US mode
  [3] = ACTION_FUNCTION(PSEUDO_US_FUNCTION_D),          // FN1 - pseudo US mode
  [4] = ACTION_FUNCTION(PSEUDO_US_FUNCTION_E),          // FN1 - pseudo US mode
  [5] = ACTION_FUNCTION(PSEUDO_US_FUNCTION_L),          // FN1 - pseudo US mode
  [6] = ACTION_FUNCTION(PSEUDO_US_FUNCTION_R),          // FN1 - pseudo US mode
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

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_qwerty);
        #endif
        persistent_default_layer_set(1UL<<_QWERTY_JP);
      }
      return false;
      break;
    case NORMAN:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_colemak);
        #endif
        persistent_default_layer_set(1UL<<_NORMAN_JP);
      }
      return false;
      break;
    case DVORAK:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_dvorak);
        #endif
        persistent_default_layer_set(1UL<<_DVORAK_JP);
      }
      return false;
      break;
    case EUCALYN:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_dvorak);
        #endif
        persistent_default_layer_set(1UL<<_EUCALYN_JP);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER_JP);
        update_tri_layer(_LOWER_JP, _RAISE_JP, _ADJUST);
      } else {
        layer_off(_LOWER_JP);
        update_tri_layer(_LOWER_JP, _RAISE_JP, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE_JP);
        update_tri_layer(_LOWER_JP, _RAISE_JP, _ADJUST);
      } else {
        layer_off(_RAISE_JP);
        update_tri_layer(_LOWER_JP, _RAISE_JP, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
  }
  return true;
}

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
  uint16_t base;
  switch (id) {
    case PSEUDO_US_FUNCTION_Q:
        base = _QWERTY;
        action_pseudo_lut(record, base, keymap_jis2us);
        break;
    case PSEUDO_US_FUNCTION_N:
        base = _NORMAN;
        action_pseudo_lut(record, base, keymap_jis2us);
        break;
    case PSEUDO_US_FUNCTION_D:
        base = _DVORAK;
        action_pseudo_lut(record, base, keymap_jis2us);
        break;
    case PSEUDO_US_FUNCTION_E:
        base = _EUCALYN;
        action_pseudo_lut(record, base, keymap_jis2us);
        break;
    case PSEUDO_US_FUNCTION_L:
        base = _LOWER;
        action_pseudo_lut(record, base, keymap_jis2us);
        break;
    case PSEUDO_US_FUNCTION_R:
        base = _RAISE;
        action_pseudo_lut(record, base, keymap_jis2us);
        break;
  }
}
