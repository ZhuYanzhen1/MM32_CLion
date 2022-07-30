/**************************************************************************/ /**
    \file     mdtp_pack.c
    \brief    Medium capacity transport protocol packing function source file.
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
******************************************************************************/

#include "mdtp_pack.h"

/*!
    \brief      medium capacity data transmission protocol packing function
    \param[in]    pid: medium capacity transport protocol package id
    \param[in]    buffer: transmit data array of size 8 bytes
    \retval none
*/
void mdtp_data_transmit(unsigned char pid, const unsigned char *buffer, const unsigned int *dma_buffer) {
    unsigned int *temp_buf = (unsigned int *) dma_buffer;
    unsigned char mdtp_pack_counter;

    temp_buf[0] = 0xff;
    temp_buf[11] = 0xff;
    temp_buf[10] = 0x00;
    /* traverse the array to determine whether there are bytes to be adjusted */
    for (mdtp_pack_counter = 0; mdtp_pack_counter < 8; mdtp_pack_counter++) {
        if (buffer[mdtp_pack_counter] == 0xff) {
            temp_buf[2 + mdtp_pack_counter] = 0x00;
            temp_buf[10] = (temp_buf[10] | (1 << mdtp_pack_counter));
        } else
            temp_buf[2 + mdtp_pack_counter] = buffer[mdtp_pack_counter];
    }

    /* judge whether the package is all 0xff */
    if (temp_buf[10] == 0xff)
        temp_buf[10] = temp_buf[2] = 0xa5;

    /* load self checking packet id byte */
    temp_buf[1] = (unsigned char) (pid << 4 | ((~pid) & 0x0f));

    /* traverse the buffer array and send all bytes using DMA */
}
