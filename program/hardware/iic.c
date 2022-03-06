/**************************************************************************/ /**
    \file       iic.c
    \brief      iic function Source File
    \author     ZGL & LaoZhu
    \version    V1.3.2
    \date       06. March 2022
******************************************************************************/

#include "iic.h"
#include "hal_device.h"
#include "hal_conf.h"
#include "pin.h"
#include "config.h"
#include "delay.h"

void iic1_writebyte(unsigned char dat) {
    I2C_SendData(I2C1, dat);
    while (I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFE) == 0);
}

void iic1_wait_for_stop(void) {
    I2C_GenerateSTOP(I2C1, ENABLE);
    while ((I2C_GetITStatus(I2C1, I2C_IT_STOP_DET)) == 0);
}

void iic1_set_slave_addr(unsigned char deviceaddr) {
    I2C_Cmd(I2C1, DISABLE);
    I2C_Send7bitAddress(I2C1, deviceaddr, I2C_Direction_Transmitter);
    I2C_Cmd(I2C1, ENABLE);
}

void iic1_config(void) {
    I2C_InitTypeDef I2C_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_I2C1, ENABLE);
    I2C_StructInit(&I2C_InitStruct);

    GPIO_PinAFConfig(IIC_SCL_PORT, GPIO_PinSource10, GPIO_AF_4);
    GPIO_PinAFConfig(IIC_SDA_PORT, GPIO_PinSource11, GPIO_AF_4);

    GPIO_InitStruct.GPIO_Pin = IIC_SCL_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(IIC_SCL_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = IIC_SDA_PIN;
    GPIO_Init(IIC_SDA_PORT, &GPIO_InitStruct);

    I2C_InitStruct.Mode = I2C_CR_MASTER;
    I2C_InitStruct.OwnAddress = 0;
    I2C_InitStruct.Speed = I2C_CR_FAST;
    I2C_InitStruct.ClockSpeed = IIC1_SPEED;
    I2C_Init(I2C1, &I2C_InitStruct);
    I2C_Cmd(I2C1, ENABLE);
}

void iic1_software_config(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOD, ENABLE);

    GPIO_InitStruct.GPIO_Pin = IIC_SCL_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(IIC_SCL_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = IIC_SDA_PIN;
    GPIO_Init(IIC_SDA_PORT, &GPIO_InitStruct);
}

void iic_start(void) {
    IIC_SDA_HIGH()
    IIC_SCL_HIGH()
    delayus(2);
    IIC_SDA_LOW()
    delayus(2);
    IIC_SCL_LOW()
}

void iic_stop(void) {
    IIC_SCL_LOW()
    IIC_SDA_LOW()
    delayus(2);
    IIC_SCL_HIGH()
    delayus(2);
    IIC_SDA_HIGH()
    delayus(2);
    IIC_SCL_LOW()
}

void iic_ack(void) {
    IIC_SCL_LOW()
    IIC_SDA_LOW()
    delayus(2);
    IIC_SCL_HIGH()
    delayus(2);
    IIC_SCL_LOW()
}

void iic_not_ack(void) {
    IIC_SCL_LOW()
    IIC_SDA_HIGH()
    delayus(2);
    IIC_SCL_HIGH()
    delayus(2);
    IIC_SCL_LOW()
    IIC_SDA_HIGH();
}

char iic_wait_ack(void) {
    unsigned char temp = 0;
    IIC_SDA_HIGH()
    delayus(2);
    IIC_SCL_HIGH()
    delayus(2);
    while (IIC_SDA) {
        temp++;
        if (temp > 250) {
            iic_stop();
            return 1;
        }
    }
    IIC_SCL_LOW()
    return 0;
}

void iic_send_byte(unsigned char byte) {
    for (unsigned char t = 0; t < 8; t++) {
        if (byte & 0x80) {
            IIC_SDA_HIGH()
        } else {
            IIC_SDA_LOW()
        }
        byte <<= 1;
        delayus(2);
        IIC_SCL_HIGH()
        delayus(2);
        IIC_SCL_LOW()
    }
    iic_wait_ack();
}

char iic_read_byte() {
    unsigned char i, value;
    IIC_SDA_HIGH()
    value = 0;
    for (i = 0; i < 8; i++) {
        IIC_SCL_LOW()
        delayus(2);
        value <<= 1;
        IIC_SCL_HIGH()
        delayus(2);
        if (IIC_SDA) {
            value++;
        }
    }
    return value;
}

