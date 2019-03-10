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

#include "ch.h"
#include "hal.h"

/*
 * scan matrix
 */
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "wait.h"

#ifndef DEBOUNCE
#   define DEBOUNCE 5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);


inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    // initialize row and col
    unselect_rows();
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

    //debug
    debug_matrix = true;
    wait_ms(500);
}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        wait_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            wait_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        pbin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

/* Column pin configuration
 */
static void  init_cols(void)
{
    // All columns are Active-Low, then setting ports to pull-up
    // Row 1
    palSetPadMode(GPIOB, 5, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 6, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 7, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 4, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 5, PAL_MODE_INPUT_PULLUP);
    // Row 2
    palSetPadMode(GPIOB, 4, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 2, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 3, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 6, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 7, PAL_MODE_INPUT_PULLUP);
    // Row 3
    palSetPadMode(GPIOB, 3, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 15, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 10, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 9, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 8, PAL_MODE_INPUT_PULLUP);
}

/* Returns status of switches(1:on, 0:off) */
static matrix_row_t read_cols()
{
    uint32_t gpio_a = ~palReadPort(GPIOA);
    uint32_t gpio_b = ~palReadPort(GPIOB);
    matrix_row_t cols;

    cols = ((gpio_b & 0x00000070) >> 4) |
           (gpio_a & 0x00000018) | // >> 3 << 3
           ((gpio_b & 0x00000008) << 2) | // >> 3 << 5
           ((gpio_a & 0x00000006) << 5) | // >> 1 << 6
           ((gpio_a & 0x000000b0) << 3) | // >> 5 << 8
           ((gpio_b & 0x00000004) << 8) | // >> 2 << 10
           ((gpio_a & 0x00004000) >> 3) | // >> 14 << 11
           ((gpio_a & 0x00000200) << 3) | // >> 9 << 12
           ((gpio_a & 0x00000100) << 5) | // >> 8 << 13
           ((gpio_a & 0x00000080) << 7) ; // >> 7 << 14

    return cols;
}

/* Row pin configuration
 */
static void unselect_rows(void)
{
    // do nothing
}

static void select_row(uint8_t row)
{
    (void)row;
}
