/**************************************************************************/ /**
    \file     mdtp_pack.c
    \brief    Medium capacity transport protocol packing function source file.
    \author   Lao·Zhu
    \version  V1.2.2
    \date     19. January 2022
******************************************************************************/

#include "mdtp_pack.h"
#include "mm32f3x_it.h"

#ifndef RUNNING_UNIT_TEST
#include "dma.h"
#else
#define common_sendbyte mdtp_sendbyte
extern void mdtp_sendbyte(unsigned char data);
#endif  // RUNNING_UNIT_TEST

extern unsigned char dma1_ch4_flag;
extern unsigned int usart1_dma_buffer_1[12];
extern unsigned int usart1_dma_buffer_2[12];

/*!
    \brief      medium capacity data transmission protocol packing function
    \param[in]    pid: medium capacity transport protocol package id
    \param[in]    buffer: transmit data array of size 8 bytes
    \retval none
*/
void mdtp_data_transmit(unsigned char pid, const unsigned char *buffer) {
    unsigned int *temp_buf = choose_uart1_buffer();
//    unsigned int *temp_buf[12] = {0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
//                                  0x00, 0x00, 0x00, 0x00, 0x00, 0xff};
    unsigned char mdtp_pack_counter;

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

    /* traverse the buffer array and send all bytes through UART0 */
#ifdef RUNNING_UNIT_TEST
    for (mdtp_pack_counter = 0; mdtp_pack_counter < 12; mdtp_pack_counter++)
        common_sendbyte(temp_buf[mdtp_pack_counter]);
#else
    while (1)
        if (dma1_ch4_flag == 0)
            break;
    uart1_dma_sent_config(temp_buf, 12);
    dma1_ch4_flag = 1;

#endif  // RUNNING_UNIT_TEST

}
