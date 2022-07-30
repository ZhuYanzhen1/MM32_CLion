/**************************************************************************/ /**
    \file       at24c02.h
    \brief      at24c02 function Header File
    \author     LaoZhu
    \version    V1.6.1
    \date       24. July 2022
******************************************************************************/

#ifndef MM32F3277_DEVICE_AT24C02_H_
#define MM32F3277_DEVICE_AT24C02_H_

void at24c02_readbytes(unsigned char sub, unsigned char *ptr, unsigned short len);
void at24c02_writebytes(unsigned char sub, unsigned char *ptr, unsigned short len);
unsigned char at24c02_test_memory(void);
void at24c02_saveparams(void);
void at24c02_readparams(void);

#endif // MM32F3277_DEVICE_AT24C02_H_
