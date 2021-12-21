//
// Created by Lao·Zhu on 2021/9/4.
//

#include "sdtp_unpack.h"

static unsigned char receive_buffer[3];

void sdtp_callback_handler(unsigned char *buffer) {

}

/*!
    \brief        small capacity data transmission protocol unpacking handler
    \param[in]    data: data received from general receive function
    \retval       none
*/
void sdtp_receive_handler(unsigned char data) {
    /* data receiving finite state machine */
    switch (((data & 0xc0) >> 6)) {
        case 0x00:
            /* separate the first two bits of the byte to obtain valid data */
            receive_buffer[0] = ((data & 0x3f) << 2);
            break;
        case 0x01:
            /* separate the first two bits of the byte to obtain valid data */
            receive_buffer[0] |= ((data & 0x30) >> 4);
            receive_buffer[1] = ((data & 0x0f) << 4);
            break;
        case 0x02:
            /* separate the first two bits of the byte to obtain valid data */
            receive_buffer[1] |= ((data & 0x3c) >> 2);
            receive_buffer[2] = ((data & 0x03) << 6);
            break;
        case 0x04:
            /* separate the first two bits of the byte to obtain valid data */
            receive_buffer[2] |= (data & 0x3f);
            /* call user callback function to complete the next step */
            sdtp_callback_handler(receive_buffer);
            break;
    }
}
