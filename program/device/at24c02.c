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
            //Write command is sent when RX FIFO is not full
            if ((I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFNF)) && (flag == 0)) {
                //Configure to read
                I2C_ReadCmd(I2C1);
                _cnt++;
                //When flag is set, receive complete
                if (_cnt == gEeprom.cnt)
                    flag = 1;
            }
            //Check receive FIFO not empty
            if (I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_RFNE)) {
                //read data to gEeprom.ptr
                gEeprom.ptr[i] = I2C_ReceiveData(I2C1);
                break;
            }
        }
    }
}

unsigned char EEPROM_WriteBuff(unsigned char sub, unsigned char *ptr, unsigned short cnt) {
    //Send sub address
    iic1_writebyte(sub);
    while (cnt--) {
        //Send data
        iic1_writebyte(*ptr);
        //Point to the next data
        ptr++;
    }
    iic1_wait_for_stop();
    gEeprom.ack = 1;
    //I2C operation stops
    gEeprom.busy = 0;
    //Wait for EEPROM getting ready.
    EEPROM_WaitEEready();
    return 1;
}

unsigned char EEPROM_SendPacket(unsigned char sub, unsigned char *ptr, unsigned short cnt) {
    unsigned char i;
    //i2c option flag set to write
    gEeprom.opt = WR;
    //number to send
    gEeprom.cnt = cnt;
    //sub address
    gEeprom.sub = sub;
    //I2C operation starts
    gEeprom.busy = 1;
    gEeprom.ack = 0;

    if ((sub % PAGESIZE) > 0) {
        //Need temp number of data, just right to the page address
        unsigned char
            temp = ((PAGESIZE - sub % PAGESIZE) < (gEeprom.cnt) ? (PAGESIZE - sub % PAGESIZE) : (gEeprom.cnt));
        //If WRITE successful
        EEPROM_WriteBuff(sub, ptr, temp);
        //Point to the next page
        ptr += temp;
        gEeprom.cnt -= temp;
        sub += temp;
        //gEeprom.cnt = 0 means transmition complete
        if (gEeprom.cnt == 0) return 1;
    }
    for (i = 0; i < (gEeprom.cnt / PAGESIZE); i++) {
        //Full page write
        EEPROM_WriteBuff(sub, ptr, PAGESIZE);
        //Point to the next page
        ptr += PAGESIZE;
        sub += PAGESIZE;
        gEeprom.cnt -= PAGESIZE;
        if (gEeprom.cnt == 0) return 1;
    }
    if (gEeprom.cnt > 0) {
        EEPROM_WriteBuff(sub, ptr, gEeprom.cnt);
        return 1;
    }
    //I2C operation ends
    gEeprom.busy = 0;
    gEeprom.ack = 1;
    return 0;
}

void EEPROM_ReadPacket(unsigned char sub, unsigned char *ptr, unsigned short cnt) {
    //I2C operation starts
    gEeprom.busy = 1;
    gEeprom.ack = 0;
    gEeprom.sub = sub;
    gEeprom.ptr = ptr;
    gEeprom.cnt = cnt;

    //Send sub address
    iic1_writebyte(gEeprom.sub);
    //receive bytes
    EEPROM_ReadBuff();
    iic1_wait_for_stop();

    //I2C operation ends
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