#ifndef TIMER_H
#define TIMER_H

#include "../libc/string.h"
#include "../drivers/screen.h"
#include "isr.h"

void init_timer(u32 freq);

#endif
