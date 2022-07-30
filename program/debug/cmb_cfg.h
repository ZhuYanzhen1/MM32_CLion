/**************************************************************************/ /**
    \file     cmb_cfg.h
    \brief    CMBackTrace configuration file
    \author   Armink
    \version  V1.6.1
    \date     24. July 2022
******************************************************************************/

#ifndef MM32F3277_DEBUG_CMB_CFG_H_
#define MM32F3277_DEBUG_CMB_CFG_H_

/* print line, must config by user */
#define cmb_println(...)     printf_(__VA_ARGS__);printf("\r\n");_fflush()

#endif  // MM32F3277_DEBUG_CMB_CFG_H_
