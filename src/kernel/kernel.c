#include "../drivers/screen.h"
#include "../libc/mem.h"
#include "../libc/string.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"

void main() {
  isr_install();
  irq_install();
  clear_screen();
  printk("Hello User! Type a command below (EXIT to stop)\n");
}

void user_input(char str[]) {
  printk("Kernel has received the message : ");
  printk(str);
  printk("\n");
  if(strcmp(str, "PAGE")) {
    u32 physical_addr = kmalloc(0x1000, 1);
    if(physical_addr == -1) {
      printk("Bad kmalloc : Memory not available\n");
    }
    else {
      printk("Page alloted at physical address : ");
      char* address;
      hex_to_ascii(physical_addr, address);
      printk(address);
      printk("\n");
    }
  }
  if(strcmp(str, "EXIT")) {
    printk("Kernel is stopping now ...\n");
    printk("Goodbye!\n");
    __asm__ __volatile__ ("hlt");
  }
}
