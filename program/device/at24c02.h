//
// Created by Lao·Zhu on 2022/2/4.
//

#ifndef MAIN_C_DEVICE_AT24C02_H_
#define MAIN_C_DEVICE_AT24C02_H_

void EEPROM_Read(unsigned char sub, unsigned char *ptr, unsigned short len);
void EEPROM_Write(unsigned char sub, unsigned char *ptr, unsigned short len);

#endif //MAIN_C_DEVICE_AT24C02_H_
