#ifndef __CAT24M01_H__
#define __CAT24M01_H__
/*
 * CAT24M01 I2C EEPROM Driver
 */

#define CAT24M01_BASE_ADDR 0xA0
#define CAT24M01_READ 0x1
#define CAT24M01_WRITE 0x0
#define CAT24M01_PAGESIZE 256

void CAT24M01_Init(I2CDriver *drv, const I2CConfig *cfg);
#endif

