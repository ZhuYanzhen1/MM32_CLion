//
// Created by 16625 on 2022-03-31.
//

#ifndef MAIN_C_HARDWARE_FLASH_H_
#define MAIN_C_HARDWARE_FLASH_H_

unsigned char flash_write(unsigned int value, unsigned int addr);
unsigned int flash_read(unsigned int addr);

#endif //MAIN_C_HARDWARE_FLASH_H_
