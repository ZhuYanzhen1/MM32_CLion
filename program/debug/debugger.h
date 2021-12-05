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
    signed_int8 = 0,
    signed_int16 = 1,
    signed_int32 = 2,
    unsigned_int8 = 3,
    unsigned_int16 = 4,
    unsigned_int32 = 5,
    float_32bit = 6
} variable_type;

typedef struct DEBUGGER_VARIABLE_T {
    unsigned long var_status;
    unsigned long var_lastvalue;
    void *var_address;
    unsigned char var_name[16];
} debugger_variable_t;

void mdtp_callback_handler(unsigned char pid, const unsigned char *data);
void debugger_register_variable(variable_type var_type, void *variable, const char *name);

#endif  //MM32F3277_DEBUG_DEBUGGER_H_
