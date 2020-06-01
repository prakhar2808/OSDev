#include "mem.h"

// Copy no_bytes from source to destination
void memcpy(char* source, char* destination, int no_bytes) {
  int i;
  for(i = 0; i < no_bytes; i++) {
    *(destination + i) = *(source + i);
  }
}
