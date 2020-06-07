#ifndef KHEAP_H
#define KHEAP_H

#include "../cpu/types.h"
#include "../libc/ordered_array.h"
#include "../mmu/paging.h"

#define KHEAP_START 0xC0000000
#define KHEAP_INITIAL_SIZE 0x100000
#define HEAP_INDEX_SIZE 0x20000
#define HEAP_MAGIC 0xCAFEBABE
#define HEAP_MIN_SIZE 0x70000

// Size information of a hole/block.

typedef struct header {
  u32 magic; // Used for error-checking and identification
  u8 is_hole; // 1 if hole
  u32 size; // Size of the hole/block, including header and footer
} header_t;

typedef struct footer {
  u32 magic;
  header_t* header;
} footer_t;

typedef struct heap {
  ordered_array_t index; // The heap index which stores pointers to holes. 
                         // Sorted by size.
  u32 start_address; // The start of allocated space. Inclusive.
  u32 end_address; // The end of allocated space. Can be expanded. Exclusive.
  u32 max_address; // The address upto which heap can be expanded.
  u8 supervisor; // Should the extra pages requested be supervisor only?
  u8 readonly; // Should the extra pages requested be read only?
} heap_t;

// Kernel heap pointer.
heap_t* kheap;

// Placement address to be used before kernel heap pages are mapped into the
// page table.
u32 free_memory_addr;
// Kernel malloc function.
u32 kmalloc_stub(u32 size, int align, u32* phys);
u32 kmalloc_p(u32 size, int align, u32* phys);
u32 kmalloc(u32 size, int align);
//Freeing memory.
void kfree(u32 addr);

// To create a heap.
heap_t* create_heap(u32 start, 
                    u32 end,
                    u32 max,
                    u8 supervisor,
                    u8 readonly);

// To allocate contiguous memory. If align == 1, then start at page boundary.
void* alloc(u32 size, u8 page_align, heap_t* heap);

// Free a block allocated using alloc.
void free(void* p, heap_t* heap);

#endif
