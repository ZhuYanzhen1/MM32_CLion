#ifndef __DELAY_H
#define __DELAY_H

#include <string.h>
#include "mm32_device.h"

void DELAY_Init(void);
void DELAY_Ms(__IO u32 nTime);
//void DELAY_Us(__IO u32 nTime);

#endif
