#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"
void memcpy(char* source, char* destination, int no_bytes);

u32 free_memory_addr;
u32 kmalloc(u32 size, int align);
#endif
