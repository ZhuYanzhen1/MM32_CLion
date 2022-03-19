//
// Created by Lao·Zhu on 2022/2/4.
//

#ifndef MAIN_DEVICE_AT24C02_H_
#define MAIN_DEVICE_AT24C02_H_

void EEPROM_Read(unsigned char sub, unsigned char *ptr, unsigned short len);
void EEPROM_Write(unsigned char sub, unsigned char *ptr, unsigned short len);
unsigned char at24c02_test_memory(void);

#endif //MAIN_DEVICE_AT24C02_H_
