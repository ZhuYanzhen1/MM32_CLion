//
// Created by Lao·Zhu on 2022/2/4.
//

#ifndef MAIN_C_HARDWARE_IIC_H_
#define MAIN_C_HARDWARE_IIC_H_

#define IIC_SDA_HIGH()  GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_PIN);
#define IIC_SDA_LOW()   GPIO_ResetBits(IIC_SDA_PORT,IIC_SDA_PIN);
#define IIC_SCL_HIGH()  GPIO_SetBits(IIC_SCL_PORT,IIC_SCL_PIN);
#define IIC_SCL_LOW()   GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL_PIN);
#define IIC_SDA         GPIO_ReadInputDataBit(IIC_SDA_PORT,IIC_SDA_PIN)

void iic1_writebyte(unsigned char dat);
void iic1_wait_for_stop(void);
void iic1_set_slave_addr(unsigned char deviceaddr);
void iic1_config(void);

void iic1_software_config(void);
void iic_start(void);
void iic_stop(void);
void iic_send_byte(unsigned char byte);
char iic_wait_ack(void);
char iic_read_byte();

#endif //MAIN_C_HARDWARE_IIC_H_
