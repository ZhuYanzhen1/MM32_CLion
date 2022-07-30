/**************************************************************************/ /**
    \file     mdtp_pack.h
    \brief    Medium capacity transport protocol packing function Header File.
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
******************************************************************************/

#ifndef MM32F3277_DEBUG_MDTP_PACK_H_
#define MM32F3277_DEBUG_MDTP_PACK_H_

void mdtp_data_transmit(unsigned char pid, const unsigned char *buffer, const unsigned int *dma_buffer);

#endif  // MM32F3277_DEBUG_MDTP_PACK_H_
