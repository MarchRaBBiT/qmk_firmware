/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

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


#define USE_SERIAL

#define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS 

#include "../../config.h"

#ifdef  RGB_DI_PIN
#undef RGB_DI_PIN
#define RGB_DI_PIN D3
#endif
#ifdef RGBLED_NUM
#undef RGBLED_NUM
#define RGBLED_NUM 10
#endif
#define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_LIMIT_VAL 255
#define RGBLIGHT_HUE_STEP 10
#define RGBLIGHT_SAT_STEP 17
#define RGBLIGHT_VAL_STEP 17
#define USB_MAX_POWER_CONSUMPTION 330