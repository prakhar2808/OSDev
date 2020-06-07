#ifndef PAGING_H
#define PAGING_H

#include "../cpu/types.h"
#include "framesBitset.h"
#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kheap.h"

// Structure for a page entry in a page table
typedef struct page {
  u32 present :1; //Page present in memory
  u32 rw :1; //0 - Read only, 1 - Read/Write
  u32 user :1; //0 - Kernel only, 1 - User 
  u32 accessed :1; //Page accessed since last refresh?
  u32 dirty :1; //Modified since last refresh?
  u32 unused :7; //Unused and reserved bits
  u32 frame :20; //Frame address (shifted 12 bits right -> offset within frame)
}__attribute__((packed)) page_t;

// Structure for a page table
typedef struct page_table {
  page_t pages[1024];
}__attribute__((aligned(4096))) page_table_t;

// Structure for page directory
typedef struct page_directory {
  
  // Array of pointers (virtual addresses) to page tables
  page_table_t* tables[1024];

  // Array of physical address of page tables
  u32 tablesPhysical[1024];

  // Physical address of tablesPhysical member
  u32 physicalAddr;
}__attribute((aligned(4096))) page_directory_t;

// Kernel directory
page_directory_t* kernel_directory;
// Current directory
page_directory_t* current_directory;

void alloc_frame(page_t* page, int is_kernel_only, int is_writeable);

void free_frame(page_t* page);

// To set up everything and enable paging
void init_paging();

// To load specified page directory in cr3 register
void switch_page_directory(page_directory_t* new_dir);

// Retrieves pointer to page entry in the page table of the page required.
// If make = 1, create page table for this particular page, if not present.
page_t* get_page(u32 address, int make, page_directory_t* dir);

// Page fault handler
void page_fault(registers_t reg);

#endif
