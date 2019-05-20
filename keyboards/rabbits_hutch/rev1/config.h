#ifndef RABBITS_HATCH_REV1_CONFIG_H_
#define RABBITS_HATCH_REV1_CONFIG_H_

#include "../config.h"

#define I2C1_BANK GPIOF
#define I2C1_SDA 0
#define I2C1_SCL 1

#ifdef MATRIX_ROWS
#undef MATRIX_ROWS
#endif
#define MATRIX_ROWS 3

#ifdef MATRIX_COLS
#undef MATRIX_COLS
#endif
#define MATRIX_COLS 5

#define DIRECT_PINS { {A8, A9, A10, A15, B3}, {A7, A6, A3, A2, B4}, {A5, A4, B7, B6, B5} }
#define UNUSED_PINS { F0, F1 }

#endif /* RABBITS_HATCH_REV1_CONFIG_H_ */
