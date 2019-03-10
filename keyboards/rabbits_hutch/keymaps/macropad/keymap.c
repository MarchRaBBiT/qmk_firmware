/*
Copyright 2019 Toshiki Yagi <mrabbit@usagigoya.jp>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "rabbits_hutch.h"

enum layer_number {
    BASE = 0,
    SCRN
};
#define COPY LCTL_T(KC_C)
#define PASTE LCTL_T(KC_V)
#define CUT LCTL_T(KC_X)
#define LOCK LGUI_T(KC_L)
#define SHOT LGUI_T(LSFT_T(KC_S))

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{ 
[BASE] = LAYOUT(
//+-----+-------+--------+----------+----------+
    COPY, KC_PGDN,   KC_UP,  KC_PGUP,   KC_PGUP,
//+-----+--------+--------+---------+----------+
   PASTE, KC_LEFT, KC_DOWN, KC_RIGHT,   KC_PGDN,
//+--------------+--------+---------+----------+
     CUT,    LOCK,    LOCK,     LOCK,  TG(SCRN)
//+-----+--------+--------+---------+----------+
),
[SCRN] = LAYOUT(
//+-----+-------+--------+---------+----------+
   KC_NO,  KC_NO,   KC_NO,    KC_NO,   KC_NO,
//+-----+-------+--------+---------+----------+
   KC_NO,  KC_NO,    SHOT,    KC_NO,   KC_NO,
//+-------------+--------+---------+----------+
   KC_NO,   LOCK,    LOCK,     LOCK,  TG(SCRN)
//+-----+--------+--------+---------+----------+
),
};
