#ifndef __LED_H
#define __LED_H

#include "mm32_device.h"
#include "hal_conf.h"

#define LED1_Port  GPIOB
#define LED1_Pin   GPIO_Pin_13
#define LED2_Port  GPIOH
#define LED2_Pin   GPIO_Pin_2

#define LED1_ON()  GPIO_ResetBits(LED1_Port,LED1_Pin)
#define LED1_OFF()  GPIO_SetBits(LED1_Port,LED1_Pin)
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(LED1_Port,LED1_Pin))?(GPIO_ResetBits(LED1_Port,LED1_Pin)):(GPIO_SetBits(LED1_Port,LED1_Pin))

#define LED2_ON()  GPIO_ResetBits(LED2_Port,LED2_Pin)
#define LED2_OFF()  GPIO_SetBits(LED2_Port,LED2_Pin)
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(LED2_Port,LED2_Pin))?(GPIO_ResetBits(LED2_Port,LED2_Pin)):(GPIO_SetBits(LED2_Port,LED2_Pin))

void LED_Init(void);

#endif
