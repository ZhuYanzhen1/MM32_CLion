/*****************************************************************************
\file     debugger.c
\brief    debugger function Source File
\author   Lao·Zhu
\version  V1.0.1
\date     3. December 2021
******************************************************************************/

#include "debugger.h"

/*!
    \brief        user callback function for unpacking completion of medium capacity transport protocol
    \param[in]    pid: medium capacity transport protocol package id
    \param[in]    data: received data array of size 8 bytes
    \retval none
*/
void mdtp_callback_handler(unsigned char pid, const unsigned char *data) {
}
