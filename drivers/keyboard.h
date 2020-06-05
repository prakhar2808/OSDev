#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../kernel/kernel.h"

void init_keyboard();
void print_letter(u8 scancode);

#endif
