#include "isr.h"

// Array of 256 interrupt handler functions that take 'registers_t' as input and
// return void.
isr_t interrupt_handlers[256];

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

    // BIOS maps both the master and slave PICs at command 0x20 data 0x21, and
    // command 0xA0 and data 0xA1. We need to remap them (slave to 0x28
    // command), and give vector offset, tell master and slave about each
    // other and give additional information about the environment (8086 or
    // others). That is done by first initializing the two PICs by the command
    // 0x11, and then give 3 extra initialization words, ending with unmasking
    // everything.

    // Initializing
    port_byte_out(0x20, 0x11); //(port, data)
    port_byte_out(0xA0, 0x11);
    //Giving vector offset
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    //Telling master and slave about each other
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    //Environment description
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    //Initializing masks to 0
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);

    //Installing the IRQs
    set_idt_gate_entry(32, (u32)irq0);
    set_idt_gate_entry(33, (u32)irq1);
    set_idt_gate_entry(34, (u32)irq2);
    set_idt_gate_entry(35, (u32)irq3);
    set_idt_gate_entry(36, (u32)irq4);
    set_idt_gate_entry(37, (u32)irq5);
    set_idt_gate_entry(38, (u32)irq6);
    set_idt_gate_entry(39, (u32)irq7);
    set_idt_gate_entry(40, (u32)irq8);
    set_idt_gate_entry(41, (u32)irq9);
    set_idt_gate_entry(42, (u32)irq10);
    set_idt_gate_entry(43, (u32)irq11);
    set_idt_gate_entry(44, (u32)irq12);
    set_idt_gate_entry(45, (u32)irq13);
    set_idt_gate_entry(46, (u32)irq14);
    set_idt_gate_entry(47, (u32)irq15);

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

void irq_install() {
  // Enable interrupts
  __asm__ __volatile__("sti");
  // IRQ0 : Timer
  init_timer(50);
  // IRQ1 : Keyboard
  init_keyboard();
}

void isr_handler(registers_t r) {
  printk("\nReceived interrupt: ");
  char s[3];
  int_to_ascii(r.int_no, s);
  printk(s);
  printk("\n");
  printk(exception_messages[r.int_no]);
  printk("\n");
}

// handler points to a function that is to be used to handle the interrupt
// number n
void register_interrupt_handler(u8 n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r) {
  //Need to send EOI (End of interrupt) (Code = 0x20) after every interrupt to 
  //the PICs so that they get ready for the next interrupt
  if(r.int_no >= 40) {
    //Slave PIC raised the interrupt
    port_byte_out(0xA0, 0x20); //To slave
  }
  port_byte_out(0x20, 0x20); //To master
  
  // Check if the handler is defined for the given interrupt
  if(interrupt_handlers[r.int_no] != 0) {
    isr_t handler = interrupt_handlers[r.int_no];
    handler(r);
  }
}
