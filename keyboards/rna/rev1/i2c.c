#include "ch.h"
#include "hal.h"

void i2c_init(void)
{
    palSetGroupMode(GPIOB, (1U<<4) | (1U<<6) | (1U<<7), 0, PAL_MODE_INPUT);
    palSetGroupMode(GPIOA, (1U<<7), 0, PAL_MODE_INPUT);
    chThdSleepMilliseconds(10);

    i2cStart(&I2CD1, &i2c1cfg);
    palSetPadMode(GPIOB, 6, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
    palSetPadMode(GPIOB, 7, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
    i2cStart(&I2CD3, &i2c2cfg);
    palSetPadMode(GPIOA, 7, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
    palSetPadMode(GPIOB, 4, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
}
