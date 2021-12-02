/*****************************************************************************
\file     debugger.h
\brief    debugger function Header File
\author   Lao·Zhu
\version  V1.0.1
\date     3. December 2021
******************************************************************************/

#ifndef MM32F3277_DEBUG_DEBUGGER_H_
#define MM32F3277_DEBUG_DEBUGGER_H_

void mdtp_callback_handler(unsigned char pid, const unsigned char *data);

#endif  //MM32F3277_DEBUG_DEBUGGER_H_
