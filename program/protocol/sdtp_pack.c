/**************************************************************************/ /**
    \file     sdtp_pack.c
    \brief    Small capacity transport protocol packing function source file.
    \author   Lao·Zhu
    \version  V1.2.2
    \date     19. January 2022
******************************************************************************/
#include "sdtp_pack.h"
#include "verification.h"
#ifndef RUNNING_UNIT_TEST
#include "uart.h"
#define common_sendbyte uart3_sendbyte
#else
extern void common_sendbyte(unsigned char data);
#endif  // RUNNING_UNIT_TEST

/*!
    \brief        small capacity data transmission protocol packing function
    \param[in]    data: received data array of size 3 bytes
    \retval       none
*/
void sdtp_data_transmit(const unsigned char *data) {
    unsigned char transmit_buffer[4], transmit_counter;

    /* add packet header after splitting data bytes */
    transmit_buffer[0] = (data[0] >> 2);
    transmit_buffer[1] = (0x01 << 6) | ((data[0] & 0x03) << 4) | (data[1] >> 4);
    transmit_buffer[2] = (0x01 << 7) | ((data[1] & 0x0f) << 2) | (data[2] >> 6);
    transmit_buffer[3] = (0x03 << 6) | (data[2] & 0x3f);

    /* traverse the buffer array and send all bytes through general sendbyte function */
    for (transmit_counter = 0; transmit_counter < 4; transmit_counter++)
        /* transmit single byte through general sendbyte function */
        common_sendbyte(transmit_buffer[transmit_counter]);
}

/*!
    \brief      small capacity data transmission protocol packing function
                Used to encapsulate speed information to send to the brushless driver board
    \param[in]  sent_data: The data being sent
    \param[out] transmit_buffer: Packetize the sent data
    \retval     none
*/
void sdtp_data_transmit_speed(const unsigned short sent_data, unsigned int transmit_buffer[4]) {
    unsigned char data[3] = {0};
    data[0] = (sent_data & 0xff00) >> 8;
    data[1] = sent_data & 0x00ff;

    unsigned int check[2] = {data[0], data[1]};
    data[2] = verification_crc8(check, 2);

    /* add packet header after splitting data bytes */
    transmit_buffer[0] = (data[0] >> 2);
    transmit_buffer[1] = (0x01 << 6) | ((data[0] & 0x03) << 4) | (data[1] >> 4);
    transmit_buffer[2] = (0x01 << 7) | ((data[1] & 0x0f) << 2) | (data[2] >> 6);
    transmit_buffer[3] = (0x03 << 6) | (data[2] & 0x3f);
}
