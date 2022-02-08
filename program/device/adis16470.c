/**************************************************************************/ /**
    \file     adis16470.c
    \brief    ADIS16470 function Source File
    \author   ZGL
    \version  V1.2.2
    \date     23. November 2021
******************************************************************************/

#include "adis16470.h"
#include "spi.h"
#include "hal_gpio.h"
#include "hal_conf.h"
#include "delay.h"

#define SPI3_NSS_SET()      GPIO_SetBits(GPIOD, GPIO_Pin_7);
#define SPI3_NSS_RESET()    GPIO_ResetBits(GPIOD, GPIO_Pin_7);

static const unsigned char address[] =
    {
        0x04, 0x06,  //Gyro_X
        0x08, 0x0A,  //Gyro_Y
        0x0C, 0x0E,  //Gyro_Z
        0x10, 0x12,  //Acc_X
        0x14, 0x16,  //Acc_Y
        0x18, 0x1A,  //Acc_Z
    };

/*!
    \brief                          The function to read the registers,
                                    because the SPI method is efficient in continuous reading,
                                    so it is recommended to read continuously
    \param[in]  address_register:   Array of registers to be read
    \param[in]  rx_point:           Location of the read data
    \param[in]  register_num:       Number of registers to be read
    \retval     none
*/
void adis16470_read_register(const unsigned char *address_register,
                             unsigned int *rx_point,
                             unsigned char register_num) {
    unsigned short tx_tmp, rx_tmp;

    /* The first frame of data is sent, not received. */
    tx_tmp = address_register[0] << 8;
    spi3_readwrite_byte(tx_tmp);

    /* +1 because spi has a frame delay */
    for (unsigned char i = 1; i < register_num; i++) {
        /* Ready to send frame format */
        tx_tmp = address_register[i] << 8;
        rx_tmp = spi3_readwrite_byte(tx_tmp);
        rx_point[i - 1] = rx_tmp;
    }

    /* The last frame of data, only receive and not send */
    tx_tmp = 0;
    rx_point[register_num - 1] = spi3_readwrite_byte(tx_tmp);
}

/*!
    \brief                  Internal write operation function for ADIS16470 register
    \param[in]  address:    Address of the register to be written
    \param[in]  value:      The value written to the register
    \retval     none
*/
void adis16470_write_register(unsigned char address_, unsigned char value) {
    /* Add the "write" flag */
    address_ |= 0x80;
    unsigned short tx_tmp = (address_ << 8) | value;
    spi3_readwrite_byte(tx_tmp);
}

unsigned short adis_read_prod_id(void) {
    unsigned short prod_id;
    SPI3_NSS_RESET();
    delayus(1);         // CS时序要求tcs>200ns
    software_spi3_mode3(0x7200);
    SPI3_NSS_SET();
    delayus(25);

    SPI3_NSS_RESET();
    delayus(1);         // CS时序要求tcs>200ns
    prod_id = software_spi3_mode3(0x7200);
    SPI3_NSS_SET();
    return prod_id;
}

#define BURST_READ(x)   delayus(1);\
                        (x) = (short) software_spi3_mode3(0x0000);

// 突发传输模式不需要发寄存器的地址。只需要发0x6800启动突发传输，后续的176个位就是寄存器的值。仔细看手册
void adis_burst_read() {
//    突发读取功能提供了一种读取一批输出数据寄存器的方法，使用连续的比特流，速率高达1MHz（SCLK）。
//    这种方法不需要每个16位段之间的停顿时间（见图3）。
//    如图27所示，通过设置DIN=0x6800来启动这种模式，然后从DOUT中读出序列中的每个寄存器，同时在整个176位的序列中保持CS为低电平。

    SPI3_NSS_RESET()
    software_spi3_mode3(0x6800);

//    delayus(1);         // burst_read 两次读取中间不确定要多久，所以需要dealy吗？应该不需要？
//    imu.diag_star = (short) software_spi3_mode3(0x00);
    BURST_READ(imu.diag_star)
    BURST_READ(imu.x_gyro)
    BURST_READ(imu.y_gyro)
    BURST_READ(imu.z_gyro)
    BURST_READ(imu.x_acll)
    BURST_READ(imu.y_acll)
    BURST_READ(imu.z_acll)
    BURST_READ(imu.temp)
    BURST_READ(imu.data_cntr)
    BURST_READ(imu.checknum)

    SPI3_NSS_SET()
}
