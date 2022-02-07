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

#define SPI3_NSS_SET GPIO_SetBits(GPIOD,GPIO_Pin_7);
#define SPI3_NSS_RESET GPIO_ResetBits(GPIOD,GPIO_Pin_7);

static const unsigned char address[] =
    {
        (unsigned char) SPI3_BASE + 0x04, (unsigned char) SPI3_BASE + 0x06,  //Gyro_X
        (unsigned char) SPI3_BASE + 0x08, (unsigned char) SPI3_BASE + 0x0A,  //Gyro_Y
        (unsigned char) SPI3_BASE + 0x0C, (unsigned char) SPI3_BASE + 0x0E,  //Gyro_Z
        (unsigned char) SPI3_BASE + 0x10, (unsigned char) SPI3_BASE + 0x12,  //Acc_X
        (unsigned char) SPI3_BASE + 0x14, (unsigned char) SPI3_BASE + 0x16,  //Acc_Y
        (unsigned char) SPI3_BASE + 0x18, (unsigned char) SPI3_BASE + 0x1A,  //Acc_Z
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

//    /* The first frame of data is sent, not received. */
//    tx_tmp = address_register[0] << 8;
//    spi3_readwrite_byte(tx_tmp);

    /* +1 because spi has a frame delay */
    for (unsigned char i = 0; i < register_num; i++) {
        /* Ready to send frame format */
        tx_tmp = address_register[i] << 8;
        rx_tmp = spi3_readwrite_byte(tx_tmp);
        rx_point[i] = rx_tmp;
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

unsigned short tmp_tx[10] = {0};
void adis_burst_read() {
    SPI3_NSS_RESET
    short rx_point[12];
    adis16470_read_register(address, (unsigned int *) &rx_point, 12);
//    imu.x_acll = rx_point[0];
    SPI3_NSS_SET

}
