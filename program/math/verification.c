/**************************************************************************/ /**
    \file     verification.c
    \brief    verification function Source File
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

#include "verification.h"
#include "const_table.h"

unsigned char verification_checksum_u8(unsigned char *ptr, unsigned short len) {
    unsigned short i;
    unsigned char sum = 0;
    for (i = 0; i < len; i++)
        sum += *ptr++;
    return sum;
}

unsigned char verification_checkxor_u8(const unsigned char *ptr, unsigned short len) {
    unsigned short i;
    unsigned char x = 0;
    for (i = 0; i < len; i++)
        x = x ^ (*(ptr + i));
    return x;
}

// CRC-16/MODBUS
unsigned short verification_crc16(unsigned int *ptr, unsigned short len) {
    unsigned char uchCRCHi = 0xff;
    unsigned char uchCRCLo = 0xff;
    unsigned long uIndex;
    while (len--) {
        uIndex = uchCRCHi ^ (*ptr++ & 0x000000ff);
        uchCRCHi = uchCRCLo ^ verification_crc16_tabelh[uIndex];
        uchCRCLo = verification_crc16_tabell[uIndex];
    }
    return (uchCRCLo << 8 | uchCRCHi);
}

// CRC-8/MAXIM
unsigned char verification_crc8(unsigned int *ptr, unsigned short len) {
    unsigned char crc = 0, i;
    while (len--) {
        crc ^= (*ptr++ & 0x000000ff);
        for (i = 0; i < 8; i++) {
            if (crc & 0x01)
                crc = (crc >> 1) ^ 0x8C;
            else
                crc >>= 1;
        }
    }
    return crc;
}
