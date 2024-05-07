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

void i2c_init(void){
    RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN; //enable gpiob clock
    RCC->APB1ENR|=RCC_APB1ENR_I2C1EN; //enable i2c1 clock
    GPIOB->MODER|=0xA0000; //set pb8 and pb9 to alternative function
    GPIOB->AFR[1]|=0x44;
    GPIOB->OTYPER|=GPIO_OTYPER_OT8|GPIO_OTYPER_OT9; //set pb8 and pb9 as open drain
    I2C1->CR1=I2C_CR1_SWRST;
    I2C1->CR1&=~I2C_CR1_SWRST;	
    I2C1->CR2|=50;
    I2C1->CCR|=0x2|(1<<15)|(1<<14); 
    I2C1->TRISE=20; //output max rise 
    I2C1->CR1|=I2C_CR1_PE;
}

char i2c_readByte(char saddr,char maddr, char *data){
    volatile int tmp;
    while(I2C1->SR2&I2C_SR2_BUSY){;}
    I2C1->CR1|=I2C_CR1_START;
    while(!(I2C1->SR1&I2C_SR1_SB)){;}
    I2C1->DR=saddr<<1;
    while(!(I2C1->SR1&I2C_SR1_ADDR)){;}
    tmp=I2C1->SR2;
    while(!(I2C1->SR1&I2C_SR1_TXE)){;}
    I2C1->DR=maddr;
    while(!(I2C1->SR1&I2C_SR1_TXE)){;}
    I2C1->CR1|=I2C_CR1_START;
    while(!(I2C1->SR1&I2C_SR1_SB)){;}	
    I2C1->DR=saddr<<1|1;
    while(!(I2C1->SR1&I2C_SR1_ADDR)){;}
    I2C1->CR1&=~I2C_CR1_ACK;
    tmp =I2C1->SR2;
    if (tmp==0){}
    I2C1->CR1|=I2C_CR1_STOP;
    while(!(I2C1->SR1&I2C_SR1_RXNE)){;}
    *data++=I2C1->DR;
    return 0;
}

void i2c_writeByte(char saddr,char maddr,char data){
    volatile int tmp;
    while(I2C1->SR2&I2C_SR2_BUSY){;}    /* wait until bus not busy */
    I2C1->CR1|=I2C_CR1_START;           /* generate start */
    while(!(I2C1->SR1&I2C_SR1_SB)){;}   /* wait until start bit is set */
    I2C1->DR = saddr<< 1;               /* Send slave address */
    while(!(I2C1->SR1&I2C_SR1_ADDR)){;} /* wait until address flag is set */
    tmp = I2C1->SR2;                    /* clear SR2 by reading it */
    if (tmp==0){}
    while(!(I2C1->SR1&I2C_SR1_TXE)){;}  /* Wait until Data register empty */
    I2C1->DR = maddr;                   /* send memory address */
    while(!(I2C1->SR1&I2C_SR1_TXE)){;}  /* wait until data register empty */
    I2C1->DR = data; 	
    while (!(I2C1->SR1 & I2C_SR1_BTF)); /* wait until transfer finished */
    I2C1->CR1 |=I2C_CR1_STOP;		    /* Generate Stop */	
}

void i2c_WriteMulti(char saddr,char maddr,char *buffer, uint8_t length){
    while (I2C1->SR2 & I2C_SR2_BUSY); //wait until bus not busy
    I2C1->CR1 |= I2C_CR1_START; //generate start
    while (!(I2C1->SR1 & I2C_SR1_SB)){;} //wait until start is generated
    volatile int tmp;														
    I2C1->DR = saddr<< 1; // Send slave address
    while (!(I2C1->SR1 & I2C_SR1_ADDR)){;} //wait until address flag is set
    tmp = I2C1->SR2; //Clear SR2
    if (tmp==0){}
    while (!(I2C1->SR1 & I2C_SR1_TXE)); //Wait until Data register empty
    I2C1->DR = maddr; // send memory address
    while (!(I2C1->SR1 & I2C_SR1_TXE)); //wait until data register empty

    //sending the data
    for (uint8_t i=0;i<length;i++)
    { 
        I2C1->DR=buffer[i]; //filling buffer with command or data
        while (!(I2C1->SR1 & I2C_SR1_BTF));
    }	
                                
    I2C1->CR1 |= I2C_CR1_STOP; //wait until transfer finished
}