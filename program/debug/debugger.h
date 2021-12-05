/**************************************************************************/ /**
    \file     debugger.h
    \brief    debugger function Header File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     3. December 2021
******************************************************************************/

#ifndef MM32F3277_DEBUG_DEBUGGER_H_
#define MM32F3277_DEBUG_DEBUGGER_H_

typedef enum {
    integer_8bit = 0,
    integer_16bit = 1,
    integer_32bit = 2,
    float_32bit = 3
} variable_type;

typedef struct DEBUGGER_VARIABLE_T {
    unsigned int var_status;
    void *var_address;
    unsigned char var_name[16];
} debugger_variable_t;

void mdtp_callback_handler(unsigned char pid, const unsigned char *data);
void debugger_register_variable(variable_type var_type, void *variable, const char *name);

#endif  //MM32F3277_DEBUG_DEBUGGER_H_
