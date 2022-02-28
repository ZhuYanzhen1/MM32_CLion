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

#define SPI3_NSS_SET()              GPIO_SetBits(GPIOD, GPIO_Pin_7);
#define SPI3_NSS_RESET()            GPIO_ResetBits(GPIOD, GPIO_Pin_7);
#define BURST_READ(x)               (x) = (short) spi3_software_mode3(0x0000);
#define CONTINUOUS_READ(x, y)       SPI3_NSS_RESET();\
                                    delayus(1);      \
                                    (x) = spi3_software_mode3(y); \
                                    SPI3_NSS_SET();  \
                                    delayus(25);

adis16470_t imu;

int wz_cumulative;
int last_wz;

short adis_read_uid() {
    short uid;
    SPI3_NSS_RESET();
    delayus(1);         // CS时序要求tcs>200ns
    spi3_software_mode3(0x7200);
    SPI3_NSS_SET();
    delayus(25);

    SPI3_NSS_RESET();
    delayus(1);         // CS时序要求tcs>200ns
    uid = spi3_software_mode3(0x7200);
    SPI3_NSS_SET();
    return uid;
}

short adis_read_register(unsigned int register_address) {
    short value;
    SPI3_NSS_RESET();
    delayus(1);         // CS时序要求tcs>200ns
    value = spi3_software_mode3(register_address);
    SPI3_NSS_SET();
    return value;
}

void adis_point_wz(void) {
    int wz, cumulative;
    SPI3_NSS_RESET();
    delayus(1);
    (wz) = spi3_software_mode3(0x0E00);
    SPI3_NSS_SET();
    cumulative = (wz + last_wz) / 2;
    wz_cumulative += cumulative;
    last_wz = wz;
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
//    BURST_READ(imu.checknum)
    imu.checknum = (short) spi3_software_mode3(0x0000);

//
//TODO 计算checksum是否正确
//

    SPI3_NSS_SET()

}
