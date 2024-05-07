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
#include <stdint.h>

void i2c_init(void);
char i2c_readByte(char saddr,char maddr,char *data);
void i2c_writeByte(char saddr,char maddr,char data);
void i2c_WriteMulti(char saddr,char maddr,char *buffer, uint8_t length);

#ifdef __cplusplus
}
#endif

#endif // I2C_H