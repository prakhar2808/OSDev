#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"
void memcpy(char* source, char* destination, int no_bytes);
void* memset(void *s, int c, u32 len);

u32 free_memory_addr;
u32 kmalloc(u32 size, int align);
u32 kmalloc_p(u32 size, int align, u32* physical_addr);
#endif
