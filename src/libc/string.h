#ifndef STRING_H
#define STRING_H

#include "../cpu/types.h"

void int_to_ascii(int n, char str[]);
int strlen(char str[]);
void reverse(char str[]);
void append(char str[], char to_append[]);
void backspace(char str[]);
void clear(char str[]);
int strcmp(char str1[], char str2[]);

#endif
