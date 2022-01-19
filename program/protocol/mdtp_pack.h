/**************************************************************************/ /**
    \file     mdtp_pack.h
    \brief    Medium capacity transport protocol packing function Header File.
    \author   Lao·Zhu
    \version  V1.2.2
    \date     19. January 2022
******************************************************************************/

#ifndef MM32F3277_DEBUG_MDTP_PACK_H_
#define MM32F3277_DEBUG_MDTP_PACK_H_

void mdtp_data_transmit(unsigned char pid, const unsigned char *data);

#endif  // MM32F3277_DEBUG_MDTP_PACK_H_
