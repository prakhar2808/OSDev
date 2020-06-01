#include "../drivers/screen.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"

void main() {
		isr_install();
    //clear_screen();
		/* Test the interrupts */
    //__asm__ __volatile__("int $0");
    //__asm__ __volatile__("int $3");
		__asm__ __volatile__("sti");
		//init_timer(50);
		init_keyboard();
}
