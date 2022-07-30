/**************************************************************************/ /**
    \file     verification.h
    \brief    verification function Header File
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

#ifndef MM32F3277_MATH_VERIFICATION_H_
#define MM32F3277_MATH_VERIFICATION_H_

unsigned char verification_checksum_u8(unsigned char *ptr, unsigned short len);
unsigned char verification_checkxor_u8(const unsigned char *ptr, unsigned short len);
unsigned short verification_crc16(unsigned int *ptr, unsigned short len);
unsigned char verification_crc8(unsigned int *ptr, unsigned short len);

#endif  // MM32F3277_MATH_VERIFICATION_H_
