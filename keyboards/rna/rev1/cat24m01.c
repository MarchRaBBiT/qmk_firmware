#include <stdint.h>
#include <string.h>

#include "hal.h"
#include "cat24m01.h"

I2CDriver *i2c_drv;
const I2CConfig *i2c_cfg;

uint8_t CAT24M01_SecRead(const uint32_t Address, uint8_t *buf, size_t size);
uint8_t CAT24M01_SelectiveRead(const uint32_t Address);
void CAT24M01_WriteDataByte(const uint32_t Address, uint8_t Value);
void CAT24M01_WritePage(const uint32_t Address, const uint8_t *Values, size_t len);

void CAT24M01_Init(I2CDriver *drv, const I2CConfig *cfg)
{
    i2c_drv = drv;
    i2c_cfg = cfg;
}

uint8_t CAT24M01_SelectiveRead(const uint32_t Address) {
    uint8_t buf;
    return CAT24M01_SecRead(Address, &buf, 1);
}

uint8_t CAT24M01_SecRead(const uint32_t Address, uint8_t *buf, size_t size)
{
    uint8_t MSB = (Address >> 16) & 0x1;
    uint16_t read_address = Address & 0xffff;
    uint8_t issue_address = CAT24M01_BASE_ADDR | MSB;
    if (i2c_drv == NULL || i2c_cfg == NULL) {
        return 0xff;
    }
    i2cStart(i2c_drv, i2c_cfg);
    i2cMasterTransmitTimeout(i2c_drv, issue_address, (uint8_t *)&read_address, 2, buf, size, TIME_INFINITE);
    return *buf;
}

void CAT24M01_WriteDataByte(const uint32_t Address, uint8_t Value)
{
    CAT24M01_WritePage(Address, &Value, 1);
}

void CAT24M01_WritePage(const uint32_t Address, const uint8_t *Values, size_t len)
{
    uint8_t MSB = (Address >> 16) & 0x1;
    uint16_t write_address = Address & 0xffff;
    uint8_t issue_address = CAT24M01_BASE_ADDR | MSB;
    uint8_t buf[CAT24M01_PAGESIZE + 2];
    if (i2c_drv == NULL || i2c_cfg == NULL) {
        return;
    }
    memcpy(buf, (uint8_t *)&write_address, 2);
    memcpy(buf + 2, Values, len);
    i2cStart(i2c_drv, i2c_cfg);
    i2cMasterTransmitTimeout(i2c_drv, issue_address, buf, 2 + len, NULL, 0, TIME_INFINITE);

}

uint8_t eeprom_read_byte (const uint8_t *Address)
{
    const uint32_t p = (const uint32_t) Address;
    return CAT24M01_SelectiveRead(p);
}

void eeprom_write_byte (uint8_t *Address, uint8_t Value)
{
    uint32_t p = (uint32_t) Address;
    CAT24M01_WriteDataByte(p, Value);
}

void eeprom_update_byte (uint8_t *Address, uint8_t Value)
{
    uint32_t p = (uint32_t) Address;
    CAT24M01_WriteDataByte(p, Value);
}

uint16_t eeprom_read_word (const uint16_t *Address)
{
    uint8_t buf[2];
    const uint32_t p = (const uint32_t) Address;
    CAT24M01_SecRead(p, buf, 2);
    return *(uint16_t *)buf;
}

void eeprom_write_word (uint16_t *Address, uint16_t Value)
{
    uint32_t p = (uint32_t) Address;
    if (p / CAT24M01_PAGESIZE == (p+1) / CAT24M01_PAGESIZE) {
        CAT24M01_WritePage(p, (uint8_t *)&Value, 2);
    } else {
        CAT24M01_WriteDataByte(p, ((uint8_t *)&Value)[0]);
        CAT24M01_WriteDataByte(p + 1, ((uint8_t *)&Value)[1]);
    }
}

void eeprom_update_word (uint16_t *Address, uint16_t Value)
{
    uint32_t p = (uint32_t) Address;
    uint16_t existingValue;
    CAT24M01_SecRead(p, (uint8_t *)&existingValue, 2);
    if (existingValue == Value) {
        return;
    }
    eeprom_write_word(Address, Value);
}

uint32_t eeprom_read_dword (const uint32_t *Address)
{
    const uint32_t p = (const uint32_t) Address;
    uint8_t buf[4];
    CAT24M01_SecRead(p, buf, 4);
    return *(uint32_t *)buf;
}

void eeprom_write_dword (uint32_t *Address, uint32_t Value)
{
    uint32_t p = (const uint32_t) Address;
    if (p / CAT24M01_PAGESIZE == (p+3) / CAT24M01_PAGESIZE){
        CAT24M01_WritePage(p, (uint8_t *)&Value, 4);
    } else {
        CAT24M01_WriteDataByte(p, ((uint8_t *)&Value)[0]);
        CAT24M01_WriteDataByte(p + 1, ((uint8_t *)&Value)[1]);
        CAT24M01_WriteDataByte(p + 2, ((uint8_t *)&Value)[2]);
        CAT24M01_WriteDataByte(p + 3, ((uint8_t *)&Value)[3]);
    }
}

void eeprom_update_dword (uint32_t *Address, uint32_t Value)
{
    uint32_t p = (const uint32_t) Address;
    uint32_t existingValue;
    CAT24M01_SecRead(p, (uint8_t *)&existingValue, 4);
    if(Value != existingValue){
        eeprom_write_dword(Address, Value);
    }
}

void eeprom_read_block(void *buf, const void *addr, uint32_t len) {
    uint32_t p = (const uint32_t)addr;
    CAT24M01_SecRead(p, buf, len);
}

void eeprom_write_block(const void *buf, void *addr, uint32_t len) {
    uint32_t bytes_written = 0;
    uint8_t *cursor = (uint8_t *)buf;
    uint32_t p = (uint32_t)addr;
    size_t n;
    size_t page_offset = p % CAT24M01_PAGESIZE;
    while (bytes_written < len) {
        n = CAT24M01_PAGESIZE - page_offset;
        if (len - bytes_written < n) {
            n = len - bytes_written;
        }
        CAT24M01_WritePage(p + bytes_written, cursor, n);
        page_offset = 0;
        bytes_written += n;
        cursor += n;
    }
    const uint8_t *src = (const uint8_t *)buf;
    while (len--) {
        eeprom_write_byte((uint8_t *)p++, *src++);
    }
}

void eeprom_update_block(const void *buf, void *addr, uint32_t len) {
    uint8_t *p = (uint8_t *)addr;
    const uint8_t *src = (const uint8_t *)buf;
    while (len--) {
        eeprom_write_byte(p++, *src++);
    }
}
