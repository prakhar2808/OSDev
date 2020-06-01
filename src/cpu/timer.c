#include "timer.h"

u32 tick = 0;

static void timer_callback(registers_t reg) {
	tick++;
	//printk("Tick : ");
	char tick_ascii[256];
	int_to_ascii(tick, tick_ascii);
	//printk(tick_ascii);
	//printk("\n");
}

void init_timer(u32 freq) {
	//Installing timer_callback function to the IRQ
	register_interrupt_handler(IRQ0, timer_callback);

	//Hardware clock at 1193180 Hz
	u32 divisor = 1193180 / freq;
	u8 low = (u8)(divisor & 0xFF);
	u8 high = (u8)((divisor >> 8) & 0xFF);
	//Send the command
	port_byte_out(0x43, 0x36); //Command port
	port_byte_out(0x40, low);
	port_byte_out(0x40, high);
}
