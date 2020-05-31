#ifndef UTIL_H
#define UTIL_H

#include "../cpu/types.h"

void memory_copy(char* source, char* destination, int no_bytes);
void int_to_ascii(int n, char str[]);

#endif
