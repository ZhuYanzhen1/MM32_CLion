//
// Created by 16625 on 2022-03-31.
//

#include "flash.h"
#include "hal_flash.h"

#define BASED_FLASH_SECTOR_ADDRESS   0x0801E000    // 第120页

/* 返回0代表写入失败；1代表成功 */
unsigned char flash_write(unsigned int value, unsigned int addr) {
    FLASH_Unlock();
    // Clear All pending flags
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASH_ErasePage(BASED_FLASH_SECTOR_ADDRESS + addr);
    FLASH_ClearFlag(FLASH_FLAG_EOP);
    FLASH_ProgramWord(BASED_FLASH_SECTOR_ADDRESS + addr, value);
    FLASH_ClearFlag(FLASH_FLAG_EOP);
    FLASH_Lock();
    if ((*(__IO unsigned int *) (BASED_FLASH_SECTOR_ADDRESS + addr)) != value) {
        return 0;
    }
    return 1;
}

unsigned int flash_read(unsigned int addr) {
    return *(unsigned int *) (addr + BASED_FLASH_SECTOR_ADDRESS);
}
