/**************************************************************************/ /**
    \file     debugger.c
    \brief    debugger function Source File
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
******************************************************************************/

#include "debugger.h"
#ifdef IS_CONTROL_MCU
#include "../ctrl_mcu/config.h"
#else
#include "../proc_mcu/config.h"
#endif  // IS_CONTROL_MCU
#include "mdtp_pack.h"
#include "printf.h"
#include "string.h"
#include "dma.h"

unsigned int printf_mdtp_dma_buffer[16][12];
unsigned char printf_dma_counter = 0;
static unsigned int variable_mdtp_dma_buffer[64][12];
static debugger_variable_t variable_buffer[VARIABLE_BUFFER_SIZE];
static unsigned char variable_index = 0;
static unsigned char printf_byte_counter = 0;
static unsigned char printf_byte_buffer[8] = {0};
static unsigned char variable_dma_counter = 0;

control_signal_t control_signal = {32767, 32767};

/*!
    \brief        user callback function for unpacking completion of medium capacity transport protocol
    \param[in]    pid: medium capacity transport protocol package id
    \param[in]    data: received data array of size 8 bytes
    \retval none
*/
void mdtp_callback_handler(unsigned char pid, const unsigned char *data) {
    if (pid == 0x00) {
        control_signal.joystick_x = data[0] << 8 | data[1];
        control_signal.joystick_y = data[2] << 8 | data[3];
    }
}

#if DEBUG_USE_PROTOCOL == 1
void _fflush(void) {
    if (printf_byte_counter != 8) {
        for (unsigned char counter = printf_byte_counter; counter < 8; counter++)
            printf_byte_buffer[counter] = 0x00;
    }
    printf_byte_counter = 0;
    mdtp_data_transmit(0x04, printf_byte_buffer, printf_mdtp_dma_buffer[printf_dma_counter]);
    printf_dma_counter++;
}

void _putchar(char character) {
    if (printf_byte_counter != 8) {
        printf_byte_buffer[printf_byte_counter] = character;
        printf_byte_counter++;
    } else {
        printf_byte_counter = 0;
        mdtp_data_transmit(0x04, printf_byte_buffer, printf_mdtp_dma_buffer[printf_dma_counter]);
        printf_dma_counter++;
        printf_byte_buffer[printf_byte_counter] = character;
        printf_byte_counter++;
    }
}
#else
void _fflush(void) {}
#endif

static void debugger_report_variable(unsigned char index) {
    unsigned char tmp_buffer[8] = {0};
    unsigned long variable_physical_addr = (unsigned long) variable_buffer[index].var_address;
    tmp_buffer[0] = index;
    tmp_buffer[1] = (variable_buffer[index].var_status >> 16) & 0x000000ffUL;
    tmp_buffer[2] = (variable_buffer[index].var_status >> 8) & 0x000000ffUL;
    tmp_buffer[3] = variable_buffer[index].var_status & 0x000000ffUL;
    tmp_buffer[4] = variable_physical_addr >> 24;
    tmp_buffer[5] = (variable_physical_addr >> 16) & 0x000000ffUL;
    tmp_buffer[6] = (variable_physical_addr >> 8) & 0x000000ffUL;
    tmp_buffer[7] = variable_physical_addr & 0x000000ffUL;
    mdtp_data_transmit(0x00, tmp_buffer, variable_mdtp_dma_buffer[variable_dma_counter]);
    variable_dma_counter++;

    for (unsigned char counter = 0; counter < 7; ++counter)
        tmp_buffer[counter + 1] = variable_buffer[index].var_name[counter];
    mdtp_data_transmit(0x01, tmp_buffer, variable_mdtp_dma_buffer[variable_dma_counter]);
    variable_dma_counter++;

    for (unsigned char counter = 0; counter < 7; ++counter)
        tmp_buffer[counter + 1] = variable_buffer[index].var_name[counter + 7];
    mdtp_data_transmit(0x02, tmp_buffer, variable_mdtp_dma_buffer[variable_dma_counter]);
    variable_dma_counter++;
}

void debugger_register_variable(variable_type var_type, void *variable, const char *name) {
    unsigned char name_length = strlen(name) > 14 ? 14 : strlen(name);
    variable_buffer[variable_index].var_status = var_type;
    variable_buffer[variable_index].var_address = variable;
    variable_buffer[variable_index].var_lastvalue = 0x00000000UL;
    for (unsigned char counter = 0; counter < name_length; counter++)
        variable_buffer[variable_index].var_name[counter] = name[counter];
    variable_index = variable_index + 1;
#if DEBUG_USE_PROTOCOL == 1
    debugger_report_variable(variable_index - 1);
    uart1_dma_sent_config(variable_mdtp_dma_buffer[0], variable_dma_counter * 12);
#endif
}

void debugger_scan_variable(unsigned long time_stamp) {
    (void) time_stamp;
#if DEBUG_USE_PROTOCOL == 1
    unsigned char tmp_buffer[8] = {0};
    tmp_buffer[1] = (time_stamp >> 16) & 0x000000ffUL;
    tmp_buffer[2] = (time_stamp >> 8) & 0x000000ffUL;
    tmp_buffer[3] = time_stamp & 0x000000ffUL;
    for (unsigned char counter = 0; counter < variable_index; ++counter) {
        unsigned long tmp_variable_u32;
        switch (variable_buffer[counter].var_status) {
            case dbg_uint8:
            case dbg_int8:tmp_variable_u32 = (*((unsigned char *) variable_buffer[counter].var_address)) & 0x000000FFUL;
                break;
            case dbg_uint16:
            case dbg_int16:
                tmp_variable_u32 = (*((unsigned short *) variable_buffer[counter].var_address)) & 0x0000FFFFUL;
                break;
            case dbg_float32:
            case dbg_uint32:
            case dbg_int32:tmp_variable_u32 = (*((unsigned long *) variable_buffer[counter].var_address));
                break;
            default:tmp_variable_u32 = 0x00000000UL;
                break;
        }
        if (variable_buffer[counter].var_lastvalue != tmp_variable_u32) {
            tmp_buffer[0] = counter;
            tmp_buffer[4] = tmp_variable_u32 >> 24;
            tmp_buffer[5] = (tmp_variable_u32 >> 16) & 0x000000ffUL;
            tmp_buffer[6] = (tmp_variable_u32 >> 8) & 0x000000ffUL;
            tmp_buffer[7] = tmp_variable_u32 & 0x000000ffUL;
            mdtp_data_transmit(0x03, tmp_buffer, variable_mdtp_dma_buffer[variable_dma_counter]);
            variable_dma_counter++;
        }
        variable_buffer[counter].var_lastvalue = tmp_variable_u32;
    }
    uart1_dma_set_transmit_buffer(variable_mdtp_dma_buffer[0], variable_dma_counter * 12);
    variable_dma_counter = 0;
#endif
}
