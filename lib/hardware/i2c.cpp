/******************************************************************************
 * Project: stm32 - E4                                                        *
 * -----                                                                      *
 * Copyright 2024 - James Clarke                                              *
 * -----                                                                      *
 * Licence:                                                                   *
 *     Use of this source code is governed by an MIT-style                    *
 *     license that can be found in the LICENSE file or at                    *
 *     https://opensource.org/licenses/MIT.                                   *
 ******************************************************************************/
#include "i2c.h"

/******************************************************************************
 * Constructors for class.  Including default (empty)                         *
 * -------------------------------------------------------------------------- *
 * @param sdapin        // pin assignment for I2C SDA                         *
 * @param sclpin        // pin assignment for I2C SCL                         *
 * @param i2cmodule     // I2C module in STM32F411 (1 or 2)                   *
 ******************************************************************************/
I2C::I2C(uint16_t sdapin, uint16_t sclpin, uint16_t i2cmodule)
{
    _sclPin = sclpin;
    _sdaPin = sdapin;
    _i2cmodule = i2cmodule;

    if (i2cmodule == 1)
        _i2c = I2C1;
    
    if (i2cmodule == 2)
        _i2c = I2C2;

}

I2C::I2C(){}

void I2C::i2c_init()
{

    RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN; //enable gpiob clock
    if (_i2cmodule == 1)
    {
        RCC->APB1ENR|=RCC_APB1ENR_I2C1EN; //enable i2c1 clock

        // can be either (PB6 PB7) or (PB8 PB9) or (PB6 PB9) or (PB8 PB7) 
        // i.e only 4 combinations exist
        // SCL : PB6 or PB8
        // SDA : PB7 or PB9
        if (_sclPin == GPIO_PIN_6){
            // PB6
            GPIOB->MODER|=(2<<12);
            GPIOB->AFR[0]|=(4<<24);
            GPIOB->OTYPER|=GPIO_OTYPER_OT6;  // set pin nopen drain
        } else {
            // PB8
            GPIOB->MODER|=(2<<16);
            GPIOB->AFR[1]|=(4<<0);
            GPIOB->OTYPER|=GPIO_OTYPER_OT8; // set pin nopen drain
        }

        if (_sdaPin == GPIO_PIN_7){
            // PB7
            GPIOB->MODER|=(2<<14);
            GPIOB->AFR[0]|=(4<<28);
            GPIOB->OTYPER|=GPIO_OTYPER_OT7; // set pin nopen drain
        } else {
            // PB9
            GPIOB->MODER|=(2<<18);
            GPIOB->AFR[1]|=(4<<4);
            GPIOB->OTYPER|=GPIO_OTYPER_OT9; // set pin nopen drain
        }
    }

    if (_i2cmodule == 2)
    {
        RCC->APB1ENR|=RCC_APB1ENR_I2C2EN; //enable i2c2 clock
        // can only be PB10 and PB11
        GPIOB->MODER|=(2<<20)|(2<<22);
        GPIOB->AFR[1]|=(4<<8)|(4<<12);
        GPIOB->OTYPER|=GPIO_OTYPER_OT10|GPIO_OTYPER_OT11; //set pb8 and pb9 as open drain
    }

    _i2c->CR1=I2C_CR1_SWRST;
    _i2c->CR1&=~I2C_CR1_SWRST;	
    _i2c->CR2|=50;
    _i2c->CCR|=0x2|(1<<15)|(1<<14); 
    _i2c->TRISE=20; //output max rise 
    _i2c->CR1|=I2C_CR1_PE;
}

char I2C::i2c_readByte(char saddr,char maddr, char *data)
{
    volatile int tmp;
    while(_i2c->SR2&I2C_SR2_BUSY){;}
    _i2c->CR1|=I2C_CR1_START;
    while(!(_i2c->SR1&I2C_SR1_SB)){;}
    _i2c->DR=saddr<<1;
    while(!(_i2c->SR1&I2C_SR1_ADDR)){;}
    tmp=_i2c->SR2;
    while(!(_i2c->SR1&I2C_SR1_TXE)){;}
    _i2c->DR=maddr;
    while(!(_i2c->SR1&I2C_SR1_TXE)){;}
    _i2c->CR1|=I2C_CR1_START;
    while(!(_i2c->SR1&I2C_SR1_SB)){;}	
    _i2c->DR=saddr<<1|1;
    while(!(_i2c->SR1&I2C_SR1_ADDR)){;}
    _i2c->CR1&=~I2C_CR1_ACK;
    tmp =_i2c->SR2;
    if (tmp==0){}
    _i2c->CR1|=I2C_CR1_STOP;
    while(!(_i2c->SR1&I2C_SR1_RXNE)){;}
    *data++=_i2c->DR;
    return 0;
}

void I2C::i2c_writeByte(char saddr,char maddr,char data){
    volatile int tmp;
    while(_i2c->SR2&I2C_SR2_BUSY){;}    /* wait until bus not busy */
    _i2c->CR1|=I2C_CR1_START;           /* generate start */
    while(!(_i2c->SR1&I2C_SR1_SB)){;}   /* wait until start bit is set */
    _i2c->DR = saddr<< 1;               /* Send slave address */
    while(!(_i2c->SR1&I2C_SR1_ADDR)){;} /* wait until address flag is set */
    tmp = _i2c->SR2;                    /* clear SR2 by reading it */
    if (tmp==0){}
    while(!(_i2c->SR1&I2C_SR1_TXE)){;}  /* Wait until Data register empty */
    _i2c->DR = maddr;                   /* send memory address */
    while(!(_i2c->SR1&I2C_SR1_TXE)){;}  /* wait until data register empty */
    _i2c->DR = data; 	
    while (!(_i2c->SR1 & I2C_SR1_BTF)); /* wait until transfer finished */
    _i2c->CR1 |=I2C_CR1_STOP;		    /* Generate Stop */	
}

void I2C::i2c_WriteMulti(char saddr,char maddr,char *buffer, uint8_t length){
    while (_i2c->SR2 & I2C_SR2_BUSY); //wait until bus not busy
    _i2c->CR1 |= I2C_CR1_START; //generate start
    while (!(_i2c->SR1 & I2C_SR1_SB)){;} //wait until start is generated
    volatile int tmp;														
    _i2c->DR = saddr<< 1; // Send slave address
    while (!(_i2c->SR1 & I2C_SR1_ADDR)){;} //wait until address flag is set
    tmp = _i2c->SR2; //Clear SR2
    if (tmp==0){}
    while (!(_i2c->SR1 & I2C_SR1_TXE)); //Wait until Data register empty
    _i2c->DR = maddr; // send memory address
    while (!(_i2c->SR1 & I2C_SR1_TXE)); //wait until data register empty

    //sending the data
    for (uint8_t i=0;i<length;i++)
    { 
        _i2c->DR=buffer[i]; //filling buffer with command or data
        while (!(_i2c->SR1 & I2C_SR1_BTF));
    }	
                                
    _i2c->CR1 |= I2C_CR1_STOP; //wait until transfer finished
}