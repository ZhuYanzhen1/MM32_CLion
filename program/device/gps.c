/**************************************************************************/ /**
    \file       gps.c
    \brief      Contains gps initialization and
                display of received gps related information on the screen
    \author     ZGL
    \version    V1.3.2
    \date       06. March 2021
******************************************************************************/


#include "gps.h"
#include "uart.h"
#include "delay.h"
#include "dma.h"
#include "hal_conf.h"

/* Send GPS initialization information */
#define UART3_CONFIG_GPS(cmdbuf)            for (unsigned char i = 0; i < (unsigned char) sizeof(cmdbuf); i++)\
                                                uart3_sendbyte((cmdbuf)[i]);\
                                            delayms(100);
/* Close packages other than RMC */
#define CLOSE_PACKAGE_OUTPUT(a, b, c, d)    output_rmc[7]=a;\
                                            output_rmc[8]=b;\
                                            output_rmc[9]=c;\
                                            output_rmc[10]=d;\
                                            UART3_CONFIG_GPS(output_rmc)

/* Receive information from GPS */
extern unsigned int usart3_dma_buffer_1[74];
extern unsigned int usart3_dma_buffer_2[74];

/*!
    \brief  Initialization of gps-related content
            1：Baud rate 115200
            2：10Hz output
            3：Output RMC packages only
            4：Using GPS and BeiDou
            5：Use NMEA version 4.1 compatible protocol
            6：uart3_dma_receive_config
            7：dma_nvic_config
            8：DMA_Cmd
*/
void gps_config() {
    char output_frequency_10Hz[] = {0xF1, 0xD9, 0x06, 0x42, 0x14, 0x00, 0x00, 0x0A,
                                    0x38, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x02, 0x24};
    char output_rmc[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0,
                         0x05, 0x01, 0x00, 0x1A};
    char output_hot_start[] = {0xF1, 0xD9, 0x06, 0x40, 0x01, 0x00, 0x03, 0x4A, 0x24};
    uart3_dma_nvic_config();
    uart3_dma_receive_config(usart3_dma_buffer_1, 74);
    DMA_Cmd(DMA1_Channel3, DISABLE);
    delayms(10);
    UART3_CONFIG_GPS(output_frequency_10Hz)
    UART3_CONFIG_GPS(output_hot_start)
    UART3_CONFIG_GPS(output_rmc)

    /* Close packages other than rmc */
    CLOSE_PACKAGE_OUTPUT(0x00, 0x00, 0xFA, 0x0F)
    CLOSE_PACKAGE_OUTPUT(0x01, 0x00, 0xFB, 0x11)
    CLOSE_PACKAGE_OUTPUT(0x02, 0x00, 0xFC, 0x13)
    CLOSE_PACKAGE_OUTPUT(0x04, 0x00, 0xFE, 0x17)
    CLOSE_PACKAGE_OUTPUT(0x06, 0x00, 0x00, 0x1B)
    CLOSE_PACKAGE_OUTPUT(0x07, 0x00, 0x01, 0x1D)
    CLOSE_PACKAGE_OUTPUT(0x08, 0x00, 0x02, 0x1F)
    CLOSE_PACKAGE_OUTPUT(0x20, 0x00, 0x1A, 0x4F)
    DMA_Cmd(DMA1_Channel3, ENABLE);
}
