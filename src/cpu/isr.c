#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "../kernel/util.h"

void isr_install() {
		set_idt_gate_entry(0, (u32)isr0);
    set_idt_gate_entry(1, (u32)isr1);
    set_idt_gate_entry(2, (u32)isr2);
    set_idt_gate_entry(3, (u32)isr3);
    set_idt_gate_entry(4, (u32)isr4);
    set_idt_gate_entry(5, (u32)isr5);
    set_idt_gate_entry(6, (u32)isr6);
    set_idt_gate_entry(7, (u32)isr7);
    set_idt_gate_entry(8, (u32)isr8);
    set_idt_gate_entry(9, (u32)isr9);
    set_idt_gate_entry(10, (u32)isr10);
    set_idt_gate_entry(11, (u32)isr11);
    set_idt_gate_entry(12, (u32)isr12);
    set_idt_gate_entry(13, (u32)isr13);
    set_idt_gate_entry(14, (u32)isr14);
    set_idt_gate_entry(15, (u32)isr15);
    set_idt_gate_entry(16, (u32)isr16);
    set_idt_gate_entry(17, (u32)isr17);
    set_idt_gate_entry(18, (u32)isr18);
    set_idt_gate_entry(19, (u32)isr19);
    set_idt_gate_entry(20, (u32)isr20);
    set_idt_gate_entry(21, (u32)isr21);
    set_idt_gate_entry(22, (u32)isr22);
    set_idt_gate_entry(23, (u32)isr23);
    set_idt_gate_entry(24, (u32)isr24);
    set_idt_gate_entry(25, (u32)isr25);
    set_idt_gate_entry(26, (u32)isr26);
    set_idt_gate_entry(27, (u32)isr27);
    set_idt_gate_entry(28, (u32)isr28);
    set_idt_gate_entry(29, (u32)isr29);
    set_idt_gate_entry(30, (u32)isr30);
    set_idt_gate_entry(31, (u32)isr31);

		set_idt_reg();
}

/* To print the message which defines every exception */
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(registers_t r) {
	printk("\nReceived interrupt: ");
	char s[3];
	int_to_ascii(r.int_no, s);
	printk(s);
	printk("\n");
	printk(exception_messages[r.int_no]);
	printk("\n");
}
