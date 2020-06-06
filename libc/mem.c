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

void* memset(void *s, int c, u32 len) {
  unsigned char *dst = s;
  while (len > 0) {
    *dst = (unsigned char) c;
    dst++;
    len--;
  }
  return s;
}

