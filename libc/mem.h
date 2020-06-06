#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"
void memcpy(char* source, char* destination, int no_bytes);
void* memset(void *s, int c, u32 len);
#endif
