//
// Created by Lao·Zhu on 2022/2/4.
//

#include "at24c02.h"
#include "iic.h"
#include "delay.h"
#include "hal_device.h"
#include "hal_conf.h"

#define EEPROM_ADDR 0xA0
#define PAGESIZE    8

void EEPROM_ReadBuff(unsigned char counter, unsigned char *buf) {
    unsigned char i, flag = 0, _cnt = 0;
    for (i = 0; i < counter; i++) {
        while (1) {
            if ((I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFNF)) && (flag == 0)) {
                I2C_ReadCmd(I2C1);
                _cnt++;
                if (_cnt == counter)
                    flag = 1;
            }
            if (I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_RFNE)) {
                buf[i] = I2C_ReceiveData(I2C1);
                break;
            }
        }
    }
}

unsigned char EEPROM_WriteBuff(unsigned char sub, unsigned char *ptr, unsigned short cnt) {
    iic1_writebyte(sub);
    while (cnt--) {
        iic1_writebyte(*ptr);
        ptr++;
    }
    iic1_wait_for_stop();
    delayus(100);
    return 1;
}

unsigned char EEPROM_SendPacket(unsigned char address, unsigned char *ptr, unsigned short counter) {
    if ((address % PAGESIZE) > 0) {
        unsigned char
            temp = ((PAGESIZE - address % PAGESIZE) < (counter) ? (PAGESIZE - address % PAGESIZE) : (counter));
        EEPROM_WriteBuff(address, ptr, temp);
        ptr += temp;
        counter -= temp;
        address += temp;
        if (counter == 0)
            return 1;
    }
    for (unsigned char i = 0; i < (counter / PAGESIZE); i++) {
        EEPROM_WriteBuff(address, ptr, PAGESIZE);
        ptr += PAGESIZE;
        address += PAGESIZE;
        counter -= PAGESIZE;
        if (counter == 0)
            return 1;
    }
    if (counter > 0)
        EEPROM_WriteBuff(address, ptr, counter);
    return 0;
}

void EEPROM_ReadPacket(unsigned char address, unsigned char *ptr, unsigned short counter) {
    iic1_writebyte(address);
    EEPROM_ReadBuff(counter, ptr);
    iic1_wait_for_stop();
    delayus(100);
}

unsigned char at24c02_test_memory(void) {
    unsigned char send_buf[128] = {0};
    unsigned char read_buf[128] = {0};
    for (unsigned char counter = 0; counter < 128; ++counter)
        send_buf[counter] = counter;
    iic1_set_slave_addr(EEPROM_ADDR);
    delayms(1);
    EEPROM_SendPacket(0x00, send_buf, 16);
    delayms(1);
    EEPROM_ReadPacket(0x00, read_buf, 16);
    delayms(1);
    for (unsigned char counter = 0; counter < 128; ++counter)
        if (read_buf[counter] != counter)
            return 1;
    return 0;
}
