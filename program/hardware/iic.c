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
#include "delay.h"
#include "config.h"

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
