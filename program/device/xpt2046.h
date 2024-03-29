/**************************************************************************/ /**
    \file       xpt2046.h
    \brief      Touch Screen Related Codes
    \author     Lao·Zhu
    \version    V1.6.1
    \date       24. July 2022
******************************************************************************/

#ifndef MM32F3277_DEVICE_XPT2046_H_
#define MM32F3277_DEVICE_XPT2046_H_

#define TOUCH_READ_TIMES    10
#define TOUCH_ERR_RANGE     20
#define TOUCH_X_CMD         0xD0
#define TOUCH_Y_CMD         0x90
#define TOUCH_Continue_Read 0xFF
#define TOUCH_X_MAX         4000
#define TOUCH_X_MIN         100
#define TOUCH_Y_MAX         4000
#define TOUCH_Y_MIN         100

#define LCD_CALx_MIN    (10)
#define LCD_CALx_MAX    (128 - LCD_CALx_MIN)
#define LCD_CALy_MIN    (10)
#define LCD_CALy_MAX    (160 - LCD_CALy_MIN)
#define LCD_CAL_X       (LCD_CALx_MAX - LCD_CALx_MIN)
#define LCD_CAL_Y       (LCD_CALy_MAX - LCD_CALy_MIN)

void xpt2046_calibrate(void);
unsigned char xpt2046_scan(unsigned char *x_pos, unsigned char *y_pos);

#endif // MM32F3277_DEVICE_XPT2046_H_
