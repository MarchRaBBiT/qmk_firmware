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
#include "quantum.h"
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
#include "mcp23017.h"
#include "config.h"

#ifndef DEBOUNCE
#   define DEBOUNCE 5
#endif
#define ROW_SHIFTER ((matrix_row_t)1)
static uint8_t debouncing = DEBOUNCE;

static pin_t master_row_pins[(MATRIX_ROWS + 1) / 2] = MATRIX_ROW_PINS;
static pin_t master_col_pins[MATRIX_COLS] = MATRIX_COL_PINS;
static const uint8_t expander_row_pins[MATRIX_ROWS] = EXPANDER_MATRIX_ROW_PINS;
static const uint8_t expander_col_pins[MATRIX_COLS] = EXPANDER_MATRIX_COL_PINS;
/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];
static uint8_t matrix_row_read[2] = { 0xff, 0xff };
static uint8_t read_command = 0;
static uint16_t expander_col_mask = 0;

static matrix_row_t read_cols(uint8_t row);
static void init_cols(void);

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

/*
 * I2C1 config.
 */
static const I2CConfig i2ccfg = {
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_2,
};

void matrix_init(void)
{
    // initialize row and col
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

    // debug
    // debug_enable = true;
    // debug_matrix = true;
    // wait_ms(500);
}

uint8_t matrix_scan(void)
{
    matrix_row_t cols;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        if (i < MATRIX_ROWS / 2) {
            // left hand(master)
            select_direct_row(i);
            wait_us(30);
            cols = read_master_cols();
        } else {
            // right hand(slave)
            select_expander_row(i - MATRIX_ROWS / 2);
            wait_us(30);
            cols = read_expander_cols();
        }
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
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

inline
void select_direct_row(uint8_t row)
{
    setPinOutput(master_row_pins[row]);
    writePinLow(master_row_pins[row]);
}

inline
void unselect_direct_row(uint8_t row)
{
    setPinInputHigh(master_row_pins[row]);
}

static matrix_row_t read_master_cols()
{
    matrix_row_t cols = (matrix_row_t)0;
    for (int i = 0 ; i < matrix_cols(); i++) {
        // Select the col pin to read (active low)
        uint8_t pin_state = readPin(master_col_pins[i]);

        // Populate the matrix row with the state of the col pin
        cols |=  pin_state ? 0 : (ROW_SHIFTER << i);
    }

    return cols;
}

inline
void select_expander_row(uint8_t row)
{
    expander_select(expander_row_pins[row]);
}

inline
void unselect_expander_row(uint8_t row)
{
    expander_unselect(row);
}

static matrix_row_t read_expander_cols()
{
    matrix_row_t cols = (matrix_row_t)0;
    uint16_t expander_pinstates = 0;
    if (expander_col_mask & 0xff) {
        expander_pinstates |= expander_read_gpioa();
    }
    if (expander_col_mask & 0xff00) {
        expander_pinstates |= (uint16_t)expander_read_gpiob() << 8;
    }
    for(int i=0; i < matrix_cols(); i++) {
        uint8_t pin_state = expander_pinstates | (1 << expander_col_pins[i]);
        cols |= pin_state ? 0 : (ROW_SHIFTER << i);
    }
    return cols;
}
/* Column pin configuration
 */
static void init_cols(void)
{
    unselect_master_rows();
    init_slave_expander();
}

static void init_slave_expander(void)
{
    // setup I2C hardware
    palSetPadMode(GPIOB, 6, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);   /* SCL */
    palSetPadMode(GPIOB, 7, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);   /* SDA */
    palSetPadMode(GPIOB, 10, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);   /* SCL */
    palSetPadMode(GPIOB, 11, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);   /* SDA */
    expander_init(&I2CD1, &i2ccfg);
}

static void unselect_master_rows(void)
{
    uint8_t row;
    for (row = 0; row < MATRIX_ROWS / 2; row++){
        unselect_direct_row(row);
    }
}

