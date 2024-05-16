#pragma once
/******************************************************************************
 * Project: stm32 - E4                                                        *
 * -------------------------------------------------------------------------- *
 * Copyright 2024 - James Clarke                                              *
 * -------------------------------------------------------------------------- *
 * Licence:                                                                   *
 *     Use of this source code is governed by an MIT-style                    *
 *     license that can be found in the LICENSE file or at                    *
 *     https://opensource.org/licenses/MIT.                                   *
 ******************************************************************************/
#ifndef I2C_H
#define I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"  // Device header

/*!
* @brief Generic I2C Library
*/
class I2C {

private:
    uint8_t _i2cmodule;
    uint16_t _sclPin;
    uint16_t _sdaPin;

    I2C_TypeDef* _i2c;

public:
    I2C();
    I2C(uint16_t sdapin, uint16_t sclpin, uint8_t i2cmodule);

    void i2c_init();
    char i2c_readByte(char saddr,char maddr,char *data);
    void i2c_writeByte(char saddr,char maddr,char data);
    void i2c_WriteMulti(char saddr,char maddr,char *buffer, uint8_t length);
};

#ifdef __cplusplus
}
#endif

#endif // I2C_H