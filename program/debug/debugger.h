/**************************************************************************/ /**
    \file     debugger.h
    \brief    debugger function Header File
    \author   Lao·Zhu
    \version  V1.0.2
    \date     19. January 2022
******************************************************************************/

#ifndef MM32F3277_DEBUG_DEBUGGER_H_
#define MM32F3277_DEBUG_DEBUGGER_H_

typedef enum {
    dbg_int8 = 0,
    dbg_int16 = 1,
    dbg_int32 = 2,
    dbg_uint8 = 3,
    dbg_uint16 = 4,
    dbg_uint32 = 5,
    dbg_float32 = 6,
} variable_type;

typedef struct DEBUGGER_VARIABLE_T {
    unsigned long var_status;
    unsigned long var_lastvalue;
    void *var_address;
    unsigned char var_name[16];
} debugger_variable_t;

typedef struct CONTROL_SIGNAL_T {
    unsigned short joystick_x;
    unsigned short joystick_y;
} control_signal_t;

extern control_signal_t control_signal;

void mdtp_callback_handler(unsigned char pid, const unsigned char *data);
void debugger_register_variable(variable_type var_type, void *variable, const char *name);
void debugger_scan_variable(unsigned long time_stamp);

#endif  //MM32F3277_DEBUG_DEBUGGER_H_
