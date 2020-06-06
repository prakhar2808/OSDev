#include "../drivers/screen.h"
#include "../libc/mem.h"
#include "../libc/string.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../libc/kheap.h"

void main() {
  isr_install();
  irq_install();
  clear_screen();
  char ch[200];
  u32 a = kmalloc(8, 0);
  hex_to_ascii(a, ch);
  printk(ch);
  printk("\n");
  init_paging();
  //printk("Hello User! Type a command below (EXIT to stop)\n");
  u32 b = kmalloc(8, 0);
  hex_to_ascii(b,ch);
  printk(ch);
  printk("\n");
  u32 c = kmalloc(10,0);
  hex_to_ascii(c,ch);
  printk(ch);
  printk("\n");
  kfree(b);
  kfree(c);
  b = kmalloc(8,0);
  hex_to_ascii(b,ch);
  printk(ch);
  printk("\n");
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
