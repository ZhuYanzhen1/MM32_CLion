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

void debugger_register_variable(variable_type var_type, void *variable, const char *name) {
    unsigned char name_length = strlen(name) > 16 ? 16 : strlen(name);
    debugger_variable_t *new_variable = memalloc(sizeof(debugger_variable_t));
    new_variable->var_status = var_type;
    new_variable->var_address = variable;
    for (unsigned char counter = 0; counter < name_length; counter++)
        new_variable->var_name[counter] = name[counter];
    variable_buffer[variable_index] = new_variable;
    variable_index = variable_index + 1;
}

void debugger_scan_variable(unsigned long time_stamp) {
}
