#ifndef __DELAY_H
#define __DELAY_H

#include <string.h>
#include "mm32_device.h"

void delay_config(void);
void delayms(__IO u32 xms);
//void DELAY_Us(__IO u32 nTime);

#endif
