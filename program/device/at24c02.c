//
// Created by Lao·Zhu on 2022/2/4.
//

#include "at24c02.h"
#include "iic.h"
#include "delay.h"
#include "hal_device.h"
#include "hal_conf.h"

#define EEPROM_ADDR     0xA0
#define PAGESIZE        8

typedef struct {
    unsigned char busy;
    unsigned char ack;
    unsigned char fault;
    unsigned char opt;
    unsigned char sub;
    unsigned char cnt;
    unsigned char *ptr;
    unsigned char sadd;
} gEepromTypeDef;

enum { WR };

gEepromTypeDef gEeprom;

void EEPROM_WaitEEready(void) {
    unsigned int i = 10000;
    while (i--);
}

void EEPROM_ReadBuff(void) {
    unsigned char i, flag = 0, _cnt = 0;
    for (i = 0; i < gEeprom.cnt; i++) {
        while (1) {
            if ((I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFNF)) && (flag == 0)) {
                I2C_ReadCmd(I2C1);
                _cnt++;
                if (_cnt == gEeprom.cnt)
                    flag = 1;
            }
            if (I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_RFNE)) {
                gEeprom.ptr[i] = I2C_ReceiveData(I2C1);
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
    gEeprom.ack = 1;
    gEeprom.busy = 0;
    EEPROM_WaitEEready();
    return 1;
}

unsigned char EEPROM_SendPacket(unsigned char sub, unsigned char *ptr, unsigned short cnt) {
    unsigned char i;
    gEeprom.opt = WR;
    gEeprom.cnt = cnt;
    gEeprom.sub = sub;
    gEeprom.busy = 1;
    gEeprom.ack = 0;

    if ((sub % PAGESIZE) > 0) {
        unsigned char
            temp = ((PAGESIZE - sub % PAGESIZE) < (gEeprom.cnt) ? (PAGESIZE - sub % PAGESIZE) : (gEeprom.cnt));
        EEPROM_WriteBuff(sub, ptr, temp);
        ptr += temp;
        gEeprom.cnt -= temp;
        sub += temp;
        if (gEeprom.cnt == 0) return 1;
    }
    for (i = 0; i < (gEeprom.cnt / PAGESIZE); i++) {
        EEPROM_WriteBuff(sub, ptr, PAGESIZE);
        ptr += PAGESIZE;
        sub += PAGESIZE;
        gEeprom.cnt -= PAGESIZE;
        if (gEeprom.cnt == 0) return 1;
    }
    if (gEeprom.cnt > 0) {
        EEPROM_WriteBuff(sub, ptr, gEeprom.cnt);
        return 1;
    }
    gEeprom.busy = 0;
    gEeprom.ack = 1;
    return 0;
}

void EEPROM_ReadPacket(unsigned char sub, unsigned char *ptr, unsigned short cnt) {
    gEeprom.busy = 1;
    gEeprom.ack = 0;
    gEeprom.sub = sub;
    gEeprom.ptr = ptr;
    gEeprom.cnt = cnt;

    iic1_writebyte(gEeprom.sub);
    EEPROM_ReadBuff();
    iic1_wait_for_stop();

    gEeprom.busy = 0;
    gEeprom.ack = 1;
    EEPROM_WaitEEready();
}

void EEPROM_Write(unsigned char sub, unsigned char *ptr, unsigned short len) {
    iic1_set_slave_addr(EEPROM_ADDR);
    delayms(2);
    do {
        EEPROM_SendPacket(sub, ptr, len);
        while (gEeprom.busy);
    } while (!gEeprom.ack);
}

void EEPROM_Read(unsigned char sub, unsigned char *ptr, unsigned short len) {
    iic1_set_slave_addr(EEPROM_ADDR);
    delayms(2);
    do {
        EEPROM_ReadPacket(sub, ptr, len);
        while (gEeprom.busy);
    } while (!gEeprom.ack);
}

unsigned char at24c02_test_memory(void) {
    unsigned char writebuffer[15] = {0};
    unsigned char readbuffer[15] = {0};
    for (int counter = 0; counter < sizeof(writebuffer); ++counter)
        writebuffer[counter] = counter * 2;
    EEPROM_Write(0x00, writebuffer, sizeof(writebuffer));
    EEPROM_Read(0x00, readbuffer, sizeof(readbuffer));
    for (int counter = 0; counter < sizeof(writebuffer); ++counter)
        if (writebuffer[counter] != readbuffer[counter])
            return 1;
    return 0;
}
