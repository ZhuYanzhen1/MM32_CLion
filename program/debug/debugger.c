/**************************************************************************/ /**
    \file     debugger.c
    \brief    debugger function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     3. December 2021
******************************************************************************/

#include "debugger.h"
#include "config.h"
#include "encrypt.h"
#include "malloc.h"
#include "string.h"

static debugger_variable_t *variable_buffer[VARIABLE_BUFFER_SIZE];
static unsigned char variable_index = 0;

/*!
    \brief        user callback function for unpacking completion of medium capacity transport protocol
    \param[in]    pid: medium capacity transport protocol package id
    \param[in]    data: received data array of size 8 bytes
    \retval none
*/
void mdtp_callback_handler(unsigned char pid, const unsigned char *data) {
}

static void debugger_report_variable(unsigned char index) {
    unsigned char tmp_buffer[8] = {0};
    unsigned long variable_physical_addr = (unsigned long) variable_buffer[index]->var_address;
    tmp_buffer[0] = index - 1;
    tmp_buffer[1] = (variable_buffer[index]->var_status >> 16) & 0x000000ffUL;
    tmp_buffer[2] = (variable_buffer[index]->var_status >> 8) & 0x000000ffUL;
    tmp_buffer[3] = variable_buffer[index]->var_status & 0x000000ffUL;
    tmp_buffer[4] = variable_physical_addr >> 24;
    tmp_buffer[5] = (variable_physical_addr >> 16) & 0x000000ffUL;
    tmp_buffer[6] = (variable_physical_addr >> 8) & 0x000000ffUL;
    tmp_buffer[7] = variable_physical_addr & 0x000000ffUL;
    mdtp_data_transmit(0x00, tmp_buffer);

    for (unsigned char counter = 0; counter < 7; ++counter)
        tmp_buffer[counter + 1] = variable_buffer[index]->var_name[counter];
    mdtp_data_transmit(0x01, tmp_buffer);

    for (unsigned char counter = 0; counter < 7; ++counter)
        tmp_buffer[counter + 1] = variable_buffer[index]->var_name[counter + 7];
    mdtp_data_transmit(0x02, tmp_buffer);
}

void debugger_register_variable(variable_type var_type, void *variable, const char *name) {
    unsigned char name_length = strlen(name) > 14 ? 14 : strlen(name);
    debugger_variable_t *new_variable = memalloc(sizeof(debugger_variable_t));
    new_variable->var_status = var_type;
    new_variable->var_address = variable;
    new_variable->var_lastvalue = 0x00000000UL;
    for (unsigned char counter = 0; counter < name_length; counter++)
        new_variable->var_name[counter] = name[counter];
    variable_buffer[variable_index] = new_variable;
    variable_index = variable_index + 1;
    debugger_report_variable(variable_index - 1);
}

void debugger_scan_variable(unsigned long time_stamp) {
    unsigned char tmp_buffer[8] = {0};
    tmp_buffer[1] = (time_stamp >> 16) & 0x000000ffUL;
    tmp_buffer[2] = (time_stamp >> 8) & 0x000000ffUL;
    tmp_buffer[3] = time_stamp & 0x000000ffUL;
    for (unsigned char counter = 0; counter < variable_index; ++counter) {
        unsigned long tmp_variable_u32;
        switch (variable_buffer[counter]->var_status) {
            case signed_int8:
                tmp_variable_u32 = (unsigned long) (*((char *) variable_buffer[counter]->var_address));
                break;
            case signed_int16:
                tmp_variable_u32 = (unsigned long) (*((short *) variable_buffer[counter]->var_address));
                break;
            case signed_int32:
                tmp_variable_u32 = (unsigned long) (*((long *) variable_buffer[counter]->var_address));
                break;
            default:
            case unsigned_int8:
                tmp_variable_u32 = (unsigned long) (*((unsigned char *) variable_buffer[counter]->var_address));
                break;
            case unsigned_int16:
                tmp_variable_u32 = (unsigned long) (*((unsigned short *) variable_buffer[counter]->var_address));
                break;
            case float_32bit:
            case unsigned_int32:
                tmp_variable_u32 = (*((unsigned long *) variable_buffer[counter]->var_address));
                break;
        }
        if (variable_buffer[counter]->var_lastvalue != tmp_variable_u32) {
            tmp_buffer[0] = counter;
            tmp_buffer[4] = tmp_variable_u32 >> 24;
            tmp_buffer[5] = (tmp_variable_u32 >> 16) & 0x000000ffUL;
            tmp_buffer[6] = (tmp_variable_u32 >> 8) & 0x000000ffUL;
            tmp_buffer[7] = tmp_variable_u32 & 0x000000ffUL;
            mdtp_data_transmit(0x03, tmp_buffer);
        }
        variable_buffer[counter]->var_lastvalue = tmp_variable_u32;
    }
}
