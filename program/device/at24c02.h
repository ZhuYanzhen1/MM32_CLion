//
// Created by Lao·Zhu on 2022/2/4.
//

#ifndef MAIN_DEVICE_AT24C02_H_
#define MAIN_DEVICE_AT24C02_H_

void at24c02_readbytes(unsigned char sub, unsigned char *ptr, unsigned short len);
void at24c02_writebytes(unsigned char sub, unsigned char *ptr, unsigned short len);
unsigned char at24c02_test_memory(void);
void at24c02_saveparams(void);
void at24c02_readparams(void);

#endif //MAIN_DEVICE_AT24C02_H_
