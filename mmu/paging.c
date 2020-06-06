#include "paging.h"

void alloc_frame(page_t* page, int is_kernel_only, int is_writeable) {
  if(!page) {
    // Page doesn't exist.
    return;
  }
  if(page->frame != 0) {
    //Frame already alloted to this page
    return;
  }
  //printk("Here\n"); 
  u32 idx = first_frame();
  set_frame(idx * 0x1000);
  page->present = 1;
  page->rw = (is_writeable)?1:0;
  page->user = (is_kernel_only)?0:1;
  page->frame = (idx * 0x1000) >> 12;
}

void free_frame(page_t* page) {
  if(!page->frame) {
    //This page doesn't have a frame
    return;
  }
  clear_frame(page->frame);
  page->frame = 0x0;
}

void init_paging() {
  // Assuming last memory address to be 0xffffffff (4 GB).
  u32 last_memory_addr = 0xFFFFFFFF;
  // Initializing frames bitset
  nframes = last_memory_addr / 0x1000;
  frames = (u32*)kmalloc(INDEX_FROM_BIT(nframes), 1);
  memset(frames, 0, INDEX_FROM_BIT(nframes));
  // Initializing page directory
  u32 phys_addr;
  kernel_directory = 
    (page_directory_t*)kmalloc_p(sizeof(page_directory_t), 1, &phys_addr);
  memset(kernel_directory, 0, sizeof(page_directory_t));
  kernel_directory->physicalAddr = phys_addr;
  current_directory = kernel_directory;
  // Mapping kheap pages.
  u32 j;
  for(j = KHEAP_START; j < KHEAP_START + KHEAP_INITIAL_SIZE; j += 0x1000) {
    get_page(j,1,kernel_directory);
  }
  // Identity mapping from 0x0 to currently used memory (which ends at
  // free_memory_addr).
  u32 i = 0;
  while(i < free_memory_addr + 0x1000) {
    // Kernel code readable but not writable from user space.
    page_t* page = get_page(i, 1, kernel_directory);
    alloc_frame(page, 0, 1);
    i += 0x1000;
  }

  //Allocating the earlier mapped kheap pages.
  for(j = KHEAP_START; j < KHEAP_START + KHEAP_INITIAL_SIZE; j+=0x1000) {
    alloc_frame(get_page(j, 1, kernel_directory), 1, 1);
  }
  
  //Register interrupt handler for page fault
  register_interrupt_handler(14, &page_fault);
  //Enable paging
  switch_page_directory(kernel_directory);

  //Init the kernel heap.
  kheap = create_heap(KHEAP_START, 
                      KHEAP_START + KHEAP_INITIAL_SIZE, 
                      0xCFFFF000, 0, 0);
}

void switch_page_directory(page_directory_t* new_dir) {
  current_directory = new_dir;
  asm __volatile__("mov %0, %%cr3":: "r"(&new_dir->tablesPhysical));
  u32 cr0;
  asm __volatile__("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000;
  asm __volatile__("mov %0, %%cr0":: "r"(cr0));
}

page_t* get_page(u32 address, int make, page_directory_t* dir) {
  //Getting index from address
  u32 idx = address / 0x1000;
  
  //Page table containing this address. Every page table has 1024 entries.
  u32 table_idx = idx / 1024;
  u32 table_offset = idx % 1024;
  if(dir->tables[table_idx]) {
    return &(dir->tables[table_idx]->pages[table_offset]);
  }
  else if(make) {
    u32 tmp;
    // Creating page table
    dir->tables[table_idx] = 
      (page_table_t*)kmalloc_p(sizeof(page_table_t), 1, &tmp);
    memset(dir->tables[table_idx], 0, sizeof(page_table_t));
    
    // Setting up physical address of this page table
    dir->tablesPhysical[table_idx] = tmp | 0x7; //Present, RW, US
    return &(dir->tables[table_idx]->pages[table_offset]);
  }
  return 0;
}

void page_fault(registers_t regs) {
  //The faulting address is stored in cr2 register.
  u32 faulting_addr;
  asm __volatile__("mov %%cr2, %0" : "=r" (faulting_addr));

  //Error code gives the details of what happened
  int present = !(regs.err_code & 0x1); //Page not present
  int rw = regs.err_code & 0x2; //Write operation?
  int us = regs.err_code & 0x4; //Processor in user mode?
  int reserved = regs.err_code & 0x8; //Over-written CPU reserved bits?
  int id = regs.err_code & 0x10; //Caused by instruction fetch?

  printk("Page fault occured!\n");
  if(present) {
    printk("Page not present\n");
  }
  if(rw) {
    printk("Tried writing to read-only page\n");
  }
  if(us) {
    printk("User-mode process tries for kernel page\n");
  }
  if(reserved) {
    printk("Trying to overwrite reserved bits\n");
  }
  printk("Faulting address : ");
  char* str;
  hex_to_ascii(faulting_addr, str);
  printk(str);
  printk("\n");

  PANIC();
}

