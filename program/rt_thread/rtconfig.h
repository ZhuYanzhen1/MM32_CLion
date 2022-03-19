#ifndef __RTTHREAD_CFG_H__
#define __RTTHREAD_CFG_H__

#include "rtthread.h"

#define RT_THREAD_PRIORITY_MAX  8
#define RT_TICK_PER_SECOND  1000
#define RT_ALIGN_SIZE   4
#define RT_NAME_MAX    8
// <c1>Using RT-Thread components initialization
//  <i>Using RT-Thread components initialization
#define RT_USING_COMPONENTS_INIT
#define RT_USING_USER_MAIN

// <o>the stack size of main thread<1-4086>
//  <i>Default: 512
#define RT_MAIN_THREAD_STACK_SIZE     2048
#define RT_DEBUG_INIT 0

// <e>Software timers Configuration
// <i> Enables user timers
#define RT_USING_TIMER_SOFT         0
#if RT_USING_TIMER_SOFT == 0
#undef RT_USING_TIMER_SOFT
#endif
// <o>The priority level of timer thread <0-31>
//  <i>Default: 4
#define RT_TIMER_THREAD_PRIO        4
// <o>The stack size of timer thread <0-8192>
//  <i>Default: 512
#define RT_TIMER_THREAD_STACK_SIZE  512

#define RT_USING_SEMAPHORE
//#define RT_USING_MUTEX
//#define RT_USING_EVENT
#define RT_USING_MAILBOX
//#define RT_USING_MESSAGEQUEUE

//#define RT_USING_HEAP
#define RT_USING_SMALL_MEM
//#define RT_USING_TINY_SIZE

#endif
