/**************************************************************************/ /**
    \file     adis16470.c
    \brief    ADIS16470 function Source File
    \author   ZGL
    \version  V1.0.1
    \date     23. November 2021
******************************************************************************/

#include "adis16470.h"
#include "spi.h"

static const unsigned char address[] =
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

/*!
    \brief                          The function to read the registers,
                                    because the SPI method is efficient in continuous reading, so it is recommended to read continuously
    \param[in]  address_register:   Array of registers to be read
    \param[in]  rx_point:           Location of the read data
    \param[in]  register_num:       Number of registers to be read
    \retval     none
*/
void adis16470_read_register(const unsigned char *address_register,
                             unsigned int *rx_point,
                             unsigned char register_num) {
    unsigned short tm_tmp, rx_tmp;

    /* The first frame of data is sent, not received. */
    tm_tmp = address_register[0] << 8;
    spi2_readwrite_byte(tm_tmp);

    /* +1 because spi has a frame delay */
    for (unsigned char i = 1; i < register_num; i++) {
        /* Ready to send frame format */
        tm_tmp = address_register[i] << 8;
        rx_tmp = spi2_readwrite_byte(tm_tmp);
        rx_point[i - 1] = rx_tmp;
    }

    /* The last frame of data, only receive and not send */
    tm_tmp = 0;
    rx_point[register_num - 1] = spi2_readwrite_byte(tm_tmp);
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
    unsigned short tm_tmp = (address_ << 8) | value;
    spi2_readwrite_byte(tm_tmp);
}

/*!
    \brief  Read registers to obtain gyroscope three-axis acceleration,
            three-axis angular velocity and three-axis attitude angle, 32-bit accuracy
    \retval none
*/
void adis16470_single_handle(void) {
    /* Used to save the last angle value and add it back later */
    float last_x = gyroscope_data.angle_x;
    float last_y = gyroscope_data.angle_y;
    float last_z = gyroscope_data.angle_z;

    static unsigned int data[sizeof(address)];

    /* Synthesize two int16_t types into int type */
    int *point32 = (int *) data;

    /* Convert int type to float type */
    float *gyroscope_float = (float *) &gyroscope_data;
    adis16470_read_register(address, data, sizeof(address));

    unsigned char i;
    for (i = 0; i < 3; i++) {
        /* Gyro 0.1 °/s=2^16LSB */
        *(gyroscope_float + i) = (float) *(point32 + i) / 655360.0f;
    }
    for (; i < 6; i++) {
        /* Acc 1.25 m/s^2=2^16LSB */
        *(gyroscope_float + i) = (float) *(point32 + i) / 52428.8f;
    }
    for (; i < 9; i++) {
        /* Angle 2160 °=2^31LSB */
        *(gyroscope_float + i) = (float) *(point32 + i) / 994205.4f;
    }

    /* Add back the angle value */
    gyroscope_data.angle_x += last_x;
    gyroscope_data.angle_y += last_y;
    gyroscope_data.angle_z += last_z;
}

/*!
    \brief  Zero bias calibration for gyroscopes
    \note	This function is recommended to be executed at the beginning,
            and it is better not to move the gyroscope when it is executed because the gyroscope can save the calibration value,
            so calibration is not necessarily necessary for each start.
    \retval none
*/
void self_calibration(void) {
    /* Store the current error value in binary */
    int RawBiasData[6] = {0};

    /* XG_BIAS_LOW address, because the address after it is continuous,so take this to start */
    unsigned char _address = 0x40;

    /* The first address to be written to memory is incremented along with _address */
    unsigned char *writedata = (unsigned char *) RawBiasData;

    /* 24 is 6 calibrated values multiplied by 4, because a calibrated value is 4 bytes, now perform a zeroing first */
    for (unsigned char i = 0; i < 24; i++) {
        adis16470_write_register(_address + i, writedata[i]);
    }
    unsigned short count = 0;

    /* Temporary structures for integration */
    gyroscope_t gyroscope_int = {0};

    /* Calibrated for 10 seconds */
    while (count < 5000) {
        adis16470_single_handle();
        //        timestamp = HAL_GetTick();
        //        while (HAL_GetTick() - timestamp < 2)
        //            ;
        count++;
    }
    gyroscope_int.angle_x = gyroscope_data.angle_x;
    gyroscope_int.angle_y = gyroscope_data.angle_y;
    gyroscope_int.angle_z = gyroscope_data.angle_z;

    /* Execution here means that the data collection is finished */
    float *pointf = (float *) &gyroscope_int; /* Floating point _address Easy access to data from gyroscope_data */
    unsigned char i = 0;
    for (; i < 3; i++) {
        RawBiasData[i] = (int) (-(*(pointf + i + 6)) * 131072.0f / 2.0f);
        /* Convert them back to int Note that this time the calibration value is taken as the angle value.
        The angular change of 5s divided by 5 is equal to the angular velocity, so here it is divided by 5 */
    }

    /* At this point in execution, all calibration data is ready to be written. */
    /* 12 is 3 calibration values multiplied by 4 because a calibration value is 4 bytes */
    for (i = 0; i < 12; i++) {
        adis16470_write_register(_address + i, writedata[i]);
    }
}
