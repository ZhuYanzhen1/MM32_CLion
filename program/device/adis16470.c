/**************************************************************************/ /**
    \file     adis16470.c
    \brief    ADIS16470 function Source File
    \author   ZGL
    \version  V1.3.2
    \date     06. March 2021
******************************************************************************/

#include "adis16470.h"
#include "spi.h"
#include "hal_gpio.h"
#include "hal_conf.h"
#include "delay.h"

#define SPI3_NSS_SET()              GPIO_SetBits(GPIOD, GPIO_Pin_7);
#define SPI3_NSS_RESET()            GPIO_ResetBits(GPIOD, GPIO_Pin_7);
#define BURST_READ(x)               (x) = (short) spi3_software_mode3(0x0000);
#define CALIBRATION_ACLL_NUMBER     20

short offset_ax = 0, offset_ay = 0;
adis16470_t imu = {0};

/*!
    \brief  Read the uid of adis16470,
            you can use this method to verify whether adis16470 can be used properly
    \retval uid of adis16470
*/
short adis_read_uid() {
    short uid;
    SPI3_NSS_RESET();

    /* CS timing requirement tcs>200ns */
    delayus(1);
    spi3_software_mode3(0x7200);
    SPI3_NSS_SET();
    delayus(25);

    SPI3_NSS_RESET();
    delayus(1);
    uid = spi3_software_mode3(0x7200);
    SPI3_NSS_SET();
    return uid;
}

/*!
    \brief      Read data from a register of adis16470
    \param[in]  register_address
    \retval     Data on registers
    \note       Cannot be used in conjunction with burst read mode
*/
short adis_read_register(unsigned int register_address) {
    short value;
    SPI3_NSS_RESET();
    delayus(1);
    value = spi3_software_mode3(register_address);
    SPI3_NSS_SET();
    return value;
}

/*!
    \brief  Use the burst read function of adis16470 to read all the required data at once
    \note   When the burst read function is enabled,
            you can no longer use spi to read data from other registers directly,
            and an exception will be generated.
*/
void adis_burst_read() {
// 出厂默认配置为用户提供了一个DR 信号（见表5），该信号在输出数据寄存器更新时发出脉冲。将其与嵌入式处理器上的一个引脚相连，
// 在这个脉冲的第二个边沿触发数据采集。MSC_CTRL寄存器的第0位（见表101），控制这个信号的极性。
// 在图22中，寄存器MSC_CTRL，位0=1，这意味着数据采集必须在DR脉冲的上升沿开始。

// 突发读取功能提供了一种读取一批输出数据寄存器的方法，使用连续的比特流，速率高达1MHz（SCLK）。
// 这种方法不需要每个16位段之间的停顿时间（见图3）。
// 如图27所示，通过设置DIN=0x6800来启动这种模式，然后从DOUT中读出序列中的每个寄存器，同时在整个176位的序列中保持CS为低电平。
    unsigned int checksum = 0;
    SPI3_NSS_RESET()
    spi3_software_mode3(0x6800);
    BURST_READ(imu.diag_star)
    BURST_READ(imu.x_gyro)
    BURST_READ(imu.y_gyro)
    BURST_READ(imu.z_gyro)
    BURST_READ(imu.x_acll)
    BURST_READ(imu.y_acll)
    BURST_READ(imu.z_acll)
    BURST_READ(imu.temp_out)
    BURST_READ(imu.data_cntr)
    SPI3_NSS_SET()
}

/*!
    \brief  Calculate the zero offset value
    \note   Calibrate only the x and y axes, not the z axis
            Subtract the zero bias within the function of the coordinate system transformation
*/
void calibration_acll() {
    short ax = 0, ay = 0;
    for (unsigned char i = 0; i < CALIBRATION_ACLL_NUMBER; i++) {
        adis_burst_read();
        ax = (short) (ax + imu.x_acll);
        ay = (short) (ay + imu.y_acll);
    }
    offset_ax = (short) (ax / CALIBRATION_ACLL_NUMBER);
    offset_ay = (short) (ay / CALIBRATION_ACLL_NUMBER);
}
