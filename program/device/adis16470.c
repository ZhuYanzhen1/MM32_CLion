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
#define BURST_READ(x)       (x) = (short) software_spi3_mode3(0x0000);\
//                            checksum += (x);

adis16470_t imu;

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

unsigned int adis_read_register(unsigned int register_address) {
    unsigned short register_num;
    SPI3_NSS_RESET();
    delayus(1);         // CS时序要求tcs>200ns
    software_spi3_mode3(register_address);
    SPI3_NSS_SET();
    delayus(25);

    SPI3_NSS_RESET();
    delayus(1);         // CS时序要求tcs>200ns
    register_num = software_spi3_mode3(register_address);
    SPI3_NSS_SET();
    return register_num;
}

// 突发传输模式不需要发寄存器的地址。只需要发0x6800启动突发传输，后续的176个位就是寄存器的值。仔细看手册
void adis_burst_read() {
// 出厂默认配置为用户提供了一个DR 信号（见表5），该信号在输出数据寄存器更新时发出脉冲。将其与嵌入式处理器上的一个引脚相连，
// 在这个脉冲的第二个边沿触发数据采集。MSC_CTRL寄存器的第0位（见表101），控制这个信号的极性。
// 在图22中，寄存器MSC_CTRL，位0=1，这意味着数据采集必须在DR脉冲的上升沿开始。

// 突发读取功能提供了一种读取一批输出数据寄存器的方法，使用连续的比特流，速率高达1MHz（SCLK）。
// 这种方法不需要每个16位段之间的停顿时间（见图3）。
// 如图27所示，通过设置DIN=0x6800来启动这种模式，然后从DOUT中读出序列中的每个寄存器，同时在整个176位的序列中保持CS为低电平。
    unsigned int checksum = 0;
    SPI3_NSS_RESET()
    software_spi3_mode3(0x6800);

    BURST_READ(imu.diag_star)
    BURST_READ(imu.x_gyro)
    BURST_READ(imu.y_gyro)
    BURST_READ(imu.z_gyro)
    BURST_READ(imu.x_acll)
    BURST_READ(imu.y_acll)
    BURST_READ(imu.z_acll)
    BURST_READ(imu.temp_out)
    BURST_READ(imu.data_cntr)
//    BURST_READ(imu.checknum)
    imu.checknum = (short) software_spi3_mode3(0x0000);

//
//TODO 计算checksum是否正确
//

    SPI3_NSS_SET()

}
