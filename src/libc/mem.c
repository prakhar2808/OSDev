#include "mem.h"
#include "../drivers/screen.h"
#include "string.h"
// Copy no_bytes from source to destination
void memcpy(char* source, char* destination, int no_bytes) {
  int i;
  for(i = 0; i < no_bytes; i++) {
    *(destination + i) = *(source + i);
  }
}
//Allocate some free memory, which keeps growing.
//size - size of memory needed.
//align - whether to align with the page sizes (4KB).
//Returns the physical address of the beginning of the memory allocated.
u32 kmalloc(u32 size, int align) {
  //Check if alignment needed, and next free address is available
  if(align == 1 && (free_memory_addr & 0xFFFFF000)
      && (free_memory_addr & 0xFFFFF000) != free_memory_addr) {
    free_memory_addr &= 0xFFFFF000;
    //Going to the offset of the next available page, may cause internal
    //fragmentation.
    free_memory_addr += 0x1000;
  }
  //Hard coding beginning of free_memory at 0x10000.
  if(free_memory_addr < 0x10000) {
    free_memory_addr = 0x10000;
  }
  //Check if required amount of memory is available
  if(0xFFFFFFFF - free_memory_addr < size) {
    return -1;
  }
  free_memory_addr += size;
  return (free_memory_addr - size);
}
