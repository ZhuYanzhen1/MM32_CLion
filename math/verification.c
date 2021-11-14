/**************************************************************************//**
  \file     verification.c
  \brief    verification function Source File
  \author   Lao·Zhu
  \version  V1.0.1
  \date     15. November 2021
 ******************************************************************************/

#include "verification.h"
#include "const_table.h"

unsigned char verification_checksum_u8(unsigned char *ptr, unsigned short len) {
    unsigned short i;
    unsigned char sum = 0, checksum;
    for (i = 0; i < len; i++)
        sum += *ptr++;
    checksum = sum & 0xff;
    return checksum;
}

unsigned char verification_checkxor_u8(const unsigned char *ptr, unsigned short len) {
    unsigned short i;
    unsigned char x = 0;
    for (i = 0; i < len; i++)
        x = x ^ (*(ptr + i));
    return x;
}

unsigned short verification_crc16(unsigned char *ptr, unsigned short len) {
    unsigned char uchCRCHi = 0xFF;
    unsigned char uchCRCLo = 0xFF;
    unsigned long uIndex;
    while (len--) {
        uIndex = uchCRCHi ^ *ptr++;
        uchCRCHi = uchCRCLo ^ verification_crc16_tabelh[uIndex];
        uchCRCLo = verification_crc16_tabell[uIndex];
    }
    return (uchCRCHi << 8 | uchCRCLo);
}

unsigned char verification_crc8(unsigned char *ptr, unsigned short len) {
    unsigned char crc = 0, i;
    while (len--) {
        crc ^= *ptr++;
        for (i = 0; i < 8; i++) {
            if (crc & 0x01)
                crc = (crc >> 1) ^ 0x8C;
            else
                crc >>= 1;
        }
    }
    return crc;
}
