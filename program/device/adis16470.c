/**************************************************************************/ /**
    \file     adis16470.c
    \brief    ADIS16470 function Source File
    \author   ZGL
    \version  V1.0.1
    \date     23. November 2021
******************************************************************************/

#include "adis16470.h"
#include "delay.h"
#include "hal_gpio.h"
#include "hal_spi.h"
#include "mm32_device.h"
#include "spi.h"

static int16_t Send_Cmd = 0x6800;  //Burst Read command

/*!
    \brief                      The function to read the registers,
                                because the SPI method is efficient in continuous reading, so it is recommended to read continuously
    \param[in]  addr_register:  Array of registers to be read
    \param[in]  rx_point:       Location of the read data
    \param[in]  register_num:   Number of registers to be read
    \retval     none
*/
void adis_read_register(const uint8_t *addr_register, uint16_t *rx_point, uint8_t register_num) {
    uint16_t Tx_tmp = 0, Rx_tmp = 0;

    /* The first frame of data is sent, not received. */
    Tx_tmp = addr_register[0] << 8;
    Rx_tmp = spi2_readwrite_byte(Tx_tmp);
    for (uint8_t i = 1; i < register_num; i++) /* +1 because spi has a frame delay*/
    {
        Tx_tmp = addr_register[i] << 8; /* Ready to send frame format */
        Rx_tmp = spi2_readwrite_byte(Tx_tmp);
        rx_point[i - 1] = Rx_tmp;
    }
    /* The last frame of data, only receive and not send */
    Tx_tmp = 0;
    rx_point[register_num - 1] = spi2_readwrite_byte(Tx_tmp);
}

/*!
    \brief                      Internal write operation function for ADIS16470 register
    \param[in]  addr:           Address of the register to be written
    \param[in]  value:          The value written to the register
    \retval     none
*/
void adis_write_register(uint8_t addr, uint8_t value) {
    addr |= 0x80; /* Add the "write" flag */
    uint16_t Tx_tmp = (addr << 8) | value;
    spi2_readwrite_byte(Tx_tmp);
}

/*!
    \brief                      Read registers to obtain gyroscope three-axis acceleration,
                                three-axis angular velocity and three-axis attitude angle, 32-bit accuracy
    \param[in]  void
    \retval     none
*/
void adis_single_handle(void) {
    /* Used to save the last angle value and add it back later */
    float lastx = gyroscope_data.angle_x;
    float lasty = gyroscope_data.angle_y;
    float lastz = gyroscope_data.angle_z;

    static uint8_t addr[] =
        {
            0x04, 0x06,  //Gyro_X
            0x08, 0x0A,  //Gyro_Y
            0x0C, 0x0E,  //Gyro_Z
            0x10, 0x12,  //Acc_X
            0x14, 0x16,  //Acc_Y
            0x18, 0x1A,  //Acc_Z
            0x24, 0x26,  //Delta_X                      /* Note that the angle difference read here is within 2ms */
            0x28, 0x2A,  //Delta_Y
            0x2C, 0x2E,  //Delta_Z
        };
    static uint16_t data[sizeof(addr)];
    int32_t *point32 = (int32_t *) data;                /* Synthesize two int16_t types into int32_t type */
    float *gyroscope_float = (float *) &gyroscope_data; /* Convert int32_t type to float type */
    adis_read_register(addr, data, sizeof(addr));

    uint8_t i;
    for (i = 0; i < 3; i++)  //Gyro 0.1 °/s=2^16LSB
    {
        *(gyroscope_float + i) = (float) *(point32 + i) / 655360.0f;
    }
    for (; i < 6; i++)  //Acc 1.25 m/s^2=2^16LSB
    {
        *(gyroscope_float + i) = (float) *(point32 + i) / 52428.8f;
    }
    for (; i < 9; i++)  //Angle 2160 °=2^31LSB
    {
        *(gyroscope_float + i) = (float) *(point32 + i) / 994205.4f;
    }

    /* Add back the angle value */
    gyroscope_data.angle_x += lastx;
    gyroscope_data.angle_y += lasty;
    gyroscope_data.angle_z += lastz;
}

/*!
    \brief                      Zero bias calibration for gyroscopes
    \param[in]  void
    \note		                This function is recommended to be executed at the beginning,
                                and it is better not to move the gyroscope when it is executed because the gyroscope can save the calibration value,
                                so calibration is not necessarily necessary for each start.
    \retval     none
*/

/**
* @brief
* @
* @par 日志
*
*/
void Self_Calibration(void) {
    int32_t RawBiasData[6] = {0};                  /* Store the current error value in binary */
    uint8_t addr = 0x40;                           /* XG_BIAS_LOW address, because the address after it is continuous,
                                                    * so take this to start */
    uint8_t *writedata = (uint8_t *) RawBiasData;  //待写入内存的首地址 和addr一起递增
    for (uint8_t i = 0; i < 24; i++)               //24是6个校准值*4 因为一个校准值是4个字节的 现在先执行一次进行清零
    {
        adis_write_register(addr + i, writedata[i]);
    }
    uint16_t count = 0;
    //    uint32_t timestamp;
    gyroscope_t GyroInt = {0}; /* Temporary structures for integration */
    while (count < 5000)       /* Calibrated for 10 seconds */
    {
        adis_single_handle();
        //        timestamp = HAL_GetTick();
        //        while (HAL_GetTick() - timestamp < 2)
        //            ;
        count++;
    }
    GyroInt.angle_x = gyroscope_data.angle_x;
    GyroInt.angle_y = gyroscope_data.angle_y;
    GyroInt.angle_z = gyroscope_data.angle_z;
    /* Execution here means that the data collection is finished */
    float *pointf = (float *) &GyroInt; /* Floating point address Easy access to data from gyroscope_data */
    uint8_t i = 0;
    for (; i < 3; i++) {
        RawBiasData[i] = (int32_t) (-(*(pointf + i + 6)) * 131072.0f / 2.0f);
        /* Convert them back to int32_t Note that this time the calibration value is taken as the angle value.
        The angular change of 5s divided by 5 is equal to the angular velocity, so here it is divided by 5 */
    }
    /* At this point in execution, all calibration data is ready to be written. */
    for (i = 0; i < 12; i++) /* 12 is 3 calibration values multiplied by 4 because a calibration value is 4 bytes */
    {
        adis_write_register(addr + i, writedata[i]);
    }
}