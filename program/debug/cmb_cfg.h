/**************************************************************************/ /**
    \file     cmb_cfg.h
    \brief    CMBackTrace configuration file
    \author   Armink
    \version  V1.0.1
    \date     19. January 2022
******************************************************************************/

#ifndef MM32F3277_DEBUG_CMB_CFG_H_
#define MM32F3277_DEBUG_CMB_CFG_H_

/* print line, must config by user */
#define cmb_println(...)     printf_(__VA_ARGS__);printf("\r\n");fflush()

#endif  // MM32F3277_DEBUG_CMB_CFG_H_
