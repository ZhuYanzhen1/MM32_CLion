//
// Created by Lao·Zhu on 2022/2/4.
//

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
    IIC_SDA_HIGH()       //先将SDA和SCL都拉高为起始信号做准备
    IIC_SCL_HIGH()
    delayus(2);    //延时稳定
    IIC_SDA_LOW()        //拉低SDA线,又高到低跳变,起始
    delayus(2);   //延时稳定
    IIC_SCL_LOW()       //拉低SCL线,钳住IIC总线
}

void iic_stop(void) {
    IIC_SCL_LOW()        //先将SDA和SCL都拉低为起始信号做准备
    IIC_SDA_LOW()
    delayus(2);    //延时稳定
    IIC_SCL_HIGH()        //拉高SCL等待SDA上升沿
    delayus(2);    //延时稳定
    IIC_SDA_HIGH()        //拉高SDA
    delayus(2);
    IIC_SCL_LOW()         //拉低SCL，钳住总线
}

void iic_ack(void) {
    IIC_SCL_LOW()                //先将SDA和SCL都拉低为起始信号做准备
    IIC_SDA_LOW()
    delayus(2);           //延时稳定
    IIC_SCL_HIGH()                //将SCL拉高
    delayus(2);               //在此延时阶段，SDA一直为低
    IIC_SCL_LOW()                    //拉低SCL，钳住总线
}

void iic_not_ack(void) {
    IIC_SCL_LOW()                //将SCL拉低为高电平做准备
    IIC_SDA_HIGH()                //将SDA先拉高
    delayus(2);
    IIC_SCL_HIGH()            //拉高SCL
    delayus(2);
    IIC_SCL_LOW()            //钳住总线
    IIC_SDA_HIGH();//CPU释放数据总线
}

char iic_wait_ack(void) {
    unsigned char temp = 0;
    IIC_SDA_HIGH()
    delayus(2);
    IIC_SCL_HIGH()         //拉高SCL准备读取SDA线，SDA和SCL同时为高，释放总线控制权
    delayus(2);
    while (IIC_SDA)                //当SDA拉低变为低电平的时候表示有效答应，调出循环
    {
        temp++;
        if (temp > 250)        //当循环250次后SDA还没有拉低，则表示没有答应信号（不准确的延时）
        {
            iic_stop();
            return 1;           //没有答应返回1
        }
    }
    IIC_SCL_LOW()
    return 0;               //有答应，返回0
}

void iic_send_byte(unsigned char byte) {
    for (unsigned char t = 0; t < 8; t++) {
        if (byte & 0x80) { // 判断字节的最高位，如果为1，输出高电平，如果为0，输出低电平
            IIC_SDA_HIGH();
        } else {
            IIC_SDA_LOW();
        }
        byte <<= 1;
        delayus(2);
        IIC_SCL_HIGH(); // 钳住数据，高电平数据线上的状态就改变不了了
        delayus(2);
        IIC_SCL_LOW(); // 置低位，为下次数据线上的状态改变做准备
    }
    iic_wait_ack();
}

char iic_read_byte() {
    unsigned char i, value;
    IIC_SDA_HIGH()
    value = 0;
    for (i = 0; i < 8; i++) {
        IIC_SCL_LOW()                //钳住总线，准备下一此读取
        delayus(2);
        value <<= 1;
        // 数据左移，为下一此读取腾出位置
        IIC_SCL_HIGH()
        delayus(2);
        if (IIC_SDA)                //如果SDA为1，则value自加
        {
            value++;
        }
//        IIC_SCL_LOW()                //钳住总线，准备下一此读取
//        delayus(2);
    }
//    iic_ack();
    return value;               //返回读取到的值
}

