/**************************************************************************/ /**
    \file     sdtp_unpack.h
    \brief    Small capacity transport protocol unpacking function Header File.
    \author   Lao·Zhu
    \version  V1.2.2
    \date     19. January 2022
******************************************************************************/

#ifndef MM32F3277_PROTOCOL_SDTP_UNPACK_H_
#define MM32F3277_PROTOCOL_SDTP_UNPACK_H_

typedef struct {
    float current;
    float temperature;
} bldc_t;

void sdtp_receive_handler(unsigned char data);

#endif  // MM32F3277_PROTOCOL_SDTP_UNPACK_H_
