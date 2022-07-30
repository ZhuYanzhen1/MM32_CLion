/**************************************************************************/ /**
    \file     cmb_cfg.h
    \brief    CMBackTrace Header file
    \author   Armink
    \version  V1.6.1
    \date     24. July 2022
******************************************************************************/

#ifndef MM32F3277_DEBUG_CORTEXM_BACKTRACE_H_
#define MM32F3277_DEBUG_CORTEXM_BACKTRACE_H_

void cm_backtrace_config(const char *firmware_name, const char *hardware_ver, const char *software_ver);
void cm_backtrace_firmware_info(void);
unsigned int cm_backtrace_call_stack(unsigned int *buffer, unsigned int size, unsigned int sp);
void cm_backtrace_assert(unsigned int sp);
void cm_backtrace_fault(unsigned int fault_handler_lr, unsigned int fault_handler_sp);

#endif  // MM32F3277_DEBUG_CORTEXM_BACKTRACE_H_
