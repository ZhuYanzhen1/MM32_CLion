/**************************************************************************/ /**
    \file     decrypt.c
    \brief    Medium capacity transport protocol unpacking function Source File.
    \author   Lao·Zhu
    \version  V1.0.3
    \date     3. December 2021
******************************************************************************/

#include "mdtp_unpack.h"
#include "string.h"

#ifndef RUNNING_UNIT_TEST
#include "debugger.h"
#else
extern void mdtp_callback_handler(unsigned char pid, const unsigned char *data);
#endif

/*!
    \brief  medium capacity transport protocol receive state variable
          0 idle state and waiting for start of package           \n
          1 receive status trying to receive a complete packet.   \n
          2 end status processing the received data
*/
volatile static unsigned char mdtp_receive_status = 0;
/*!
    \brief  medium capacity transport protocol receive character counter
*/
volatile static unsigned char mdtp_receive_number_counter = 0;
/*!
    \brief  medium capacity transport protocol receive buffer array
*/
static unsigned char mdtp_receive_data_buffer[10] = {0};

/*!
    \brief        medium capacity data transmission protocol unpacking handler
    \param[in]    data: data received from UART peripheral
    \retval none
*/
void mdtp_receive_handler(unsigned char data) {
    /* data receiving finite state machine */
    switch (mdtp_receive_status) {
        case 0:
            /* judge whether the packet header is received */
            if (data == 0xff) {
                /* enter the receive state */
                mdtp_receive_status = 1;

                /* clear receive array counter and buffer */
                mdtp_receive_number_counter = 0;
                memset(mdtp_receive_data_buffer, 0x00, sizeof(mdtp_receive_data_buffer));
            }
            break;

        case 1:
            /* judge whether the end of the packet is mistakenly recognized as the header */
            if (data == 0xff && mdtp_receive_number_counter != 0) {
                /* an unexpected data had been received */
                /* reset to receive start of package state */
                mdtp_receive_status = 0;

                /* clear receive array counter and buffer */
                mdtp_receive_number_counter = 0;
                memset(mdtp_receive_data_buffer, 0x00, sizeof(mdtp_receive_data_buffer));
            } else if (data != 0xff) {
                /* judge whether the reception is completed or the error data is received */
                if (mdtp_receive_number_counter != 10) {
                    /* receive the data into the array in turn */
                    mdtp_receive_data_buffer[mdtp_receive_number_counter] = data;
                    mdtp_receive_number_counter = mdtp_receive_number_counter + 1;
                    if (mdtp_receive_number_counter == 10)
                        mdtp_receive_status = 2;
                }
            }
            break;

        case 2:
            if (data == 0xff) {
                mdtp_receive_status = 0;

                /* verify whether the pid byte is correct*/
                if ((mdtp_receive_data_buffer[0] >> 4) == (~mdtp_receive_data_buffer[0] & 0x0f)) {
                    unsigned char tmp_rcv_buffer[8], counter = 0;
                    /* judge whether the package content is all 0xff */
                    if (mdtp_receive_data_buffer[1] == 0xa5 && mdtp_receive_data_buffer[9] == 0xa5)
                        memset(tmp_rcv_buffer, 0xff, sizeof(tmp_rcv_buffer));
                    else {
                        /* traverse the data byte to be adjusted */
                        for (; counter < 8; ++counter)
                            if (((mdtp_receive_data_buffer[9] >> counter) & 0x01) == 0x01)
                                tmp_rcv_buffer[counter] = 0xff;
                            else
                                tmp_rcv_buffer[counter] = mdtp_receive_data_buffer[counter + 1];
                    }
                    /* call user callback function to complete the next step */
                    mdtp_callback_handler(mdtp_receive_data_buffer[0] >> 4, tmp_rcv_buffer);
                }
            } else {
                /* an unexpected data had been received */
                /* reset to receive start of package state */
                mdtp_receive_status = 0;

                /* clear receive array counter and buffer */
                mdtp_receive_number_counter = 0;
                memset(mdtp_receive_data_buffer, 0x00, sizeof(mdtp_receive_data_buffer));
            }
            break;
        default: break;
    }
}
