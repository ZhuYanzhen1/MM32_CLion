#ifndef MM32F3277_SYSTEM_HEAP_H_
#define MM32F3277_SYSTEM_HEAP_H_

void *memalloc(unsigned int xWantedSize);
void memfree(void *pv);
unsigned int xPortGetFreeHeapSize(void);
unsigned int xPortGetMinimumEverFreeHeapSize(void);

#endif  // MM32F3277_SYSTEM_HEAP_H_
