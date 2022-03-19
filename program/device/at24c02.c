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

struct at24c02_typedef_t {
    unsigned char busy;
    unsigned char ack;
    unsigned char fault;
    unsigned char opt;
    unsigned char sub;
    unsigned char cnt;
    unsigned char *ptr;
};

enum { WR };

struct at24c02_typedef_t at24c02;

void at24c02_delay(void) {
    unsigned int i = 10000;
    while (i--);
}

void at24c02_readnocheck(void) {
    unsigned char i, flag = 0, _cnt = 0;
    for (i = 0; i < at24c02.cnt; i++) {
        while (1) {
            if ((I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFNF)) && (flag == 0)) {
                I2C_ReadCmd(I2C1);
                _cnt++;
                if (_cnt == at24c02.cnt)
                    flag = 1;
            }
            if (I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_RFNE)) {
                at24c02.ptr[i] = I2C_ReceiveData(I2C1);
                break;
            }
        }
    }
}

unsigned char at24c02_writenocheck(unsigned char sub, unsigned char *ptr, unsigned short cnt) {
    iic1_writebyte(sub);
    while (cnt--) {
        iic1_writebyte(*ptr);
        ptr++;
    }
    iic1_wait_for_stop();
    at24c02.ack = 1;
    at24c02.busy = 0;
    at24c02_delay();
    return 1;
}

unsigned char at24c02_writepage(unsigned char sub, unsigned char *ptr, unsigned short cnt) {
    unsigned char i;
    at24c02.opt = WR;
    at24c02.cnt = cnt;
    at24c02.sub = sub;
    at24c02.busy = 1;
    at24c02.ack = 0;

    if ((sub % PAGESIZE) > 0) {
        unsigned char
            temp = ((PAGESIZE - sub % PAGESIZE) < (at24c02.cnt) ? (PAGESIZE - sub % PAGESIZE) : (at24c02.cnt));
        at24c02_writenocheck(sub, ptr, temp);
        ptr += temp;
        at24c02.cnt -= temp;
        sub += temp;
        if (at24c02.cnt == 0) return 1;
    }
    for (i = 0; i < (at24c02.cnt / PAGESIZE); i++) {
        at24c02_writenocheck(sub, ptr, PAGESIZE);
        ptr += PAGESIZE;
        sub += PAGESIZE;
        at24c02.cnt -= PAGESIZE;
        if (at24c02.cnt == 0) return 1;
    }
    if (at24c02.cnt > 0) {
        at24c02_writenocheck(sub, ptr, at24c02.cnt);
        return 1;
    }
    at24c02.busy = 0;
    at24c02.ack = 1;
    return 0;
}

void at24c02_readpage(unsigned char sub, unsigned char *ptr, unsigned short cnt) {
    at24c02.busy = 1;
    at24c02.ack = 0;
    at24c02.sub = sub;
    at24c02.ptr = ptr;
    at24c02.cnt = cnt;

    iic1_writebyte(at24c02.sub);
    at24c02_readnocheck();
    iic1_wait_for_stop();

    at24c02.busy = 0;
    at24c02.ack = 1;
    at24c02_delay();
}

void at24c02_writebytes(unsigned char sub, unsigned char *ptr, unsigned short len) {
    iic1_set_slave_addr(EEPROM_ADDR);
    delayms(2);
    do {
        at24c02_writepage(sub, ptr, len);
        while (at24c02.busy);
    } while (!at24c02.ack);
}

void at24c02_readbytes(unsigned char sub, unsigned char *ptr, unsigned short len) {
    iic1_set_slave_addr(EEPROM_ADDR);
    delayms(2);
    do {
        at24c02_readpage(sub, ptr, len);
        while (at24c02.busy);
    } while (!at24c02.ack);
}

unsigned char at24c02_test_memory(void) {
    unsigned char writebuffer[8] = {0};
    unsigned char readbuffer[8] = {0};
    for (int counter = 0; counter < sizeof(writebuffer); ++counter)
        writebuffer[counter] = counter * 2;
    for (unsigned char page = 0; page < 32; page++) {
        at24c02_writebytes(page * 8, writebuffer, sizeof(writebuffer));
        at24c02_readbytes(page * 8, readbuffer, sizeof(readbuffer));
        for (int counter = 0; counter < sizeof(writebuffer); ++counter)
            if (writebuffer[counter] != readbuffer[counter])
                return 1;
    }
    return 0;
}

extern short xoffset, yoffset;
extern float xfactor, yfactor;
void at24c02_saveparams(void) {
    unsigned int float_int32 = (*(unsigned int *) &xfactor);
    unsigned char tmp_buf[8] = {xoffset >> 8, xoffset & 0x00ff, yoffset >> 8, yoffset & 0x00ff,
                                float_int32 >> 24, (float_int32 >> 16) & 0x000000ff, (float_int32 >> 8) & 0x000000ff,
                                float_int32 & 0x000000ff};
    at24c02_writebytes(0x00, tmp_buf, 8);

    float_int32 = (*(unsigned int *) &yfactor);
    tmp_buf[0] = float_int32 >> 24;
    tmp_buf[1] = (float_int32 >> 16) & 0x000000ff;
    tmp_buf[2] = (float_int32 >> 8) & 0x000000ff;
    tmp_buf[3] = float_int32 & 0x000000ff;
    at24c02_writebytes(0x08, tmp_buf, 8);
}

void at24c02_readparams(void) {
    unsigned char tmp_buf[8] = {0};

    at24c02_readbytes(0x00, tmp_buf, 8);
    xoffset = (short) (tmp_buf[0] << 8 | tmp_buf[1]);
    yoffset = (short) (tmp_buf[2] << 8 | tmp_buf[3]);
    unsigned int float_int32 = (tmp_buf[4] << 24 | tmp_buf[5] << 16 | tmp_buf[6] << 8 | tmp_buf[7]);
    xfactor = (*(float *) &float_int32);

    at24c02_readbytes(0x08, tmp_buf, 8);
    float_int32 = (tmp_buf[0] << 24 | tmp_buf[1] << 16 | tmp_buf[2] << 8 | tmp_buf[3]);
    yfactor = (*(float *) &float_int32);
}
