/**************************************************************************/ /**
    \file     sdtp_pack.h
    \brief    Small capacity transport protocol packing function Header file.
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
******************************************************************************/

#ifndef MM32F3277_PROTOCOL_SDTP_PACK_H_
#define MM32F3277_PROTOCOL_SDTP_PACK_H_

void sdtp_data_transmit(const unsigned char *data);
void sdtp_data_transmit_speed(const unsigned short sent_data, unsigned int transmit_buffer[4]);

#endif  // MM32F3277_PROTOCOL_SDTP_PACK_H_
