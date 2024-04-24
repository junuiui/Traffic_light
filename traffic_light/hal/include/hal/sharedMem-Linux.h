#ifndef _SHAREDMEM_LINUX_H
#define _SHAREDMEM_LINUX_H

void sharedMem_init(void);
void* sharedMem_thread(void*);
void sharedMem_cleanup(void);
int sharedMem_getStatus(void);
#endif