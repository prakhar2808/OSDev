#ifndef IDT_H
#define IDT_H

#include "../libc/string.h"
#include "types.h"

//Segment selector
#define KERNEL_CS 0x08

//Structure for interrupt handler
typedef struct {
	u16 low_offset; // Lower 16 bits of the handler address.
	u16 sel; // Kernel segment selector.
	u8 always0;
	u8 flags; // Bit 7: "Interrupt is present"
						// Bits 6-5: Privilege level of callers (0=kernel,3=user)
						// Bit 4: Set to 0 for interrupt handler
						// Bits 3-0: 1110 to signify 32 bits interrupt gate
	u16 high_offset; // Higher 16 bits of the handler address
} __attribute__((packed)) idt_gate_entry;

//Pointer to array of interrupt handlers
typedef struct {
	u16 limit;
	u32 base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
idt_gate_entry idt[IDT_ENTRIES];
idt_register_t idt_reg;

//Functions implemented in idt.c
void set_idt_gate_entry(int n, u32 handler);
void set_idt_reg();

#endif
