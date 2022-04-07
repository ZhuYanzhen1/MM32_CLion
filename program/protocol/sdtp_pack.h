/**************************************************************************/ /**
    \file     sdtp_pack.h
    \brief    Small capacity transport protocol packing function Header file.
    \author   Lao·Zhu
    \version  V1.2.2
    \date     19. January 2022
******************************************************************************/

#ifndef MAIN_C_PROTOCOL_SDTP_PACK_H_
#define MAIN_C_PROTOCOL_SDTP_PACK_H_

void sdtp_data_transmit(const unsigned char *data);
void sdtp_data_transmit_speed(float sent_data, unsigned int transmit_buffer[4]);

#endif  // MAIN_C_PROTOCOL_SDTP_PACK_H_
