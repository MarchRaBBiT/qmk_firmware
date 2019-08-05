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
#include "config.h"

#ifndef DEBOUNCE
#   define DEBOUNCE 5
#endif
static uint8_t debouncing = DEBOUNCE;

static pin_t master_row_pins[(MATRIX_ROWS + 1) / 2] = MATRIX_ROW_PINS;
static pin_t master_col_pins[MATRIX_COLS] = MATRIX_COL_PINS;
/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];
static uint8_t matrix_row_read[2] = { 0xff, 0xff };

static uint8_t read_command = 0;

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
}

static void unselect_master_rows(void)
{
    uint8_t row;
    for (row = 0; row < MATRIX_ROWS / 2; row++){
        unselect_direct_row(row);
    }
}

static matrix_row_t read_master_cols(uint8_t row)
{
    writePinLow(MATRIX_ROW_PINS[row]);

    return (matrix_row_t)0;
}

/* Returns status of switches(1:on, 0:off) */
static matrix_row_t read_cols(uint8_t row)
{
    msg_t status = MSG_OK;

    if (MSG_OK != status) {
        i2cflags_t error_code;
        error_code = i2cGetErrors(i2cDrivers[row]);
        printf("I2C TX Error: %x\n", error_code);
        return (matrix_row_t)0;
    }

    status = i2cMasterReceive(i2cDrivers[row], addresses[row], matrix_row_read, 2);

    if (MSG_OK != status) {
        i2cflags_t error_code;
        error_code = i2cGetErrors(i2cDrivers[row]);
        printf("I2C RX Error: %x\n", error_code);
        return (matrix_row_t)0;
    }

    uint8_t low = ~matrix_row_read[0];
    uint8_t high = ~matrix_row_read[1];

    return high<<8 | low;
}
