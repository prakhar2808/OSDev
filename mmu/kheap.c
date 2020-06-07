#include "kheap.h"

// Non-public functions.

//To find the smallest hole that is apt for the given size.
s32 find_smallest_hole(u32 size, u8 page_align, heap_t* heap);
//Less than comparator.
s8 header_t_less_than(void* a, void* b);
//To expand the total size of the heap.
void expand(u32 new_size, heap_t* heap);
//To contract the total size of the heap.
u32 contact(u32 new_size, heap_t* heap);

//****************************************************************************

s32 find_smallest_hole(u32 size, u8 page_align, heap_t* heap) {
  u32 i = 0;
  while(i < heap->index.size) {
    header_t* header = (header_t*)lookup_ordered_array(i, &heap->index);
    if(page_align == 1) {
      u32 location = (u32)header;
      s32 offset = 0;
      // Finding the distance to next page boundary after the header.
      if((location + sizeof(header_t)) &&
        (location + sizeof(header_t)) && 0xFFFFF000 != 
        (location + sizeof(header_t))) {
          offset = 0x1000 - (location + sizeof(header_t)) % 0x1000;
      }
      // The size of the hole starting from the page boundary just after the
      // header.
      s32 hole_size = (s32)header->size - offset;
      // Is fitting possible?
      if(hole_size >= (s32)size) {
        break;
      }
    }
    else if(header->size >= size) {
      break;
    }
    ++i;
  }

  if(i == heap->index.size) {
    // No big enough hole found.
    return -1;
  }
  return i;
}

//****************************************************************************

s8 header_t_less_than(void* a, void* b) {
  return (((header_t*)a)->size < ((header_t*)b)->size)?1:0;
}

//****************************************************************************

void expand(u32 new_size, heap_t* heap) {
  // Check if new_size is bigger than the current size.
  if(new_size < heap->end_address - heap->start_address) {
    printk("Request new size for heap expansion is less than current size.\n");
    return;
  }
  // Nearest page boundary length for new_size.
  if(new_size && new_size & 0xFFFFF000 != new_size) {
    new_size &= 0xFFFFF000;
    new_size += 0x1000; 
  }
  // Making sure the new_size amount of memory exists.
  if(heap->max_address - heap->start_address < new_size) {
    printk("Request new size for heap expansion is greater than the allowed size after page align.\n");
    return;
  }
  // Both old_size and new_size are page aligned.
  u32 old_size = heap->end_address - heap->start_address;
  u32 i = old_size;
  while(i < new_size) {
    alloc_frame(get_page(heap->start_address + i, 1, kernel_directory),
                (heap->supervisor)?1:0,
                (heap->readonly)?0:1);
    i += 0x1000;
  }
  heap->end_address = heap->start_address + new_size;
}

//****************************************************************************

u32 contract(u32 new_size, heap_t* heap) {
  if(new_size > heap->end_address - heap->start_address) {
    printk("New size is greater than the current heap size for contract.\n");
    return heap->end_address - heap->start_address;
  }

  if(new_size && new_size & 0xFFFFF000 != new_size) {
    new_size &= 0xFFFFF000;
    new_size += 0x1000;
  }
  
  // Not contracting too much.
  if(new_size < HEAP_MIN_SIZE) {
    printk("New size less than HEAP_MIN_SIZE. Setting new_size to HEAP_MIN_SIZE\n");
    new_size = HEAP_MIN_SIZE;
  }

  u32 old_size = heap->end_address - heap->start_address;
  u32 i = old_size - 0x1000;
  while(new_size < i) {
    free_frame(get_page(heap->start_address + i, 0, kernel_directory));
    i -= 0x1000;
  }
  heap->end_address = heap->start_address + new_size;
  return new_size;
}
/******************************************************************************
 **********************Public API Functions************************************
 *****************************************************************************/
u32 kmalloc_stub(u32 size, int align, u32* phys) {
  u32 ret;
  if(kheap == 0) {
    //Check if alignment needed, and next free address is available.
    if(align == 1 && 
        free_memory_addr &&
        ((free_memory_addr & 0xFFFFF000) != free_memory_addr)) {
      free_memory_addr &= 0xFFFFF000;
      free_memory_addr += 0x1000;
    }
    if(free_memory_addr < 0x400000) {
      free_memory_addr = 0x400000;
    }
    if(phys != 0) {
      *phys = free_memory_addr;
    }
    ret = free_memory_addr;
    free_memory_addr += size;
  }
  else {
    ret = (u32)alloc(size, align, kheap); 
    if(phys != 0) {
      page_t *page = get_page(ret, 0, kernel_directory);
      *phys = page->frame * 0x1000 + (ret & 0xFFF);
    }
  }
  return ret;
}

//*****************************************************************************

u32 kmalloc_p(u32 size, int align, u32* phys) {
  return kmalloc_stub(size, align, phys);
}

//*****************************************************************************

u32 kmalloc(u32 size, int align) {
  return kmalloc_stub(size, align, 0);
}

//*****************************************************************************

void kfree(u32 addr) {
  free((void*)addr, kheap);
}

//*****************************************************************************

heap_t* create_heap(u32 start,
                    u32 end,
                    u32 max,
                    u8 supervisor,
                    u8 readonly) {

  // Start address and end address have to be page-aligned.
  if(start % 0x1000 != 0) {
    printk("Start address for the heap not page-aligned.\n");
    return 0;
  }
  if(end % 0x1000 != 0) {
    printk("End address for the heap not page-aligned.\n");
    return 0;
  }
  heap_t* heap = (heap_t*)kmalloc(sizeof(heap_t), 1);
  //Initializing the index. Index of the heap should begin at start.
  heap->index = place_ordered_array((void*)start, 
                                    HEAP_INDEX_SIZE, 
                                    &header_t_less_than);
  
  //Moving start forward to resemble where we can put data (the end of the
  //index).
  start += sizeof(type_t) * HEAP_INDEX_SIZE;

  //Now page-aligning the start index.
  if(start && (start & 0xFFFFF000 != start)) {
    start &= 0xFFFFF000;
    start += 0x1000;
  }
  
  //Filling other members of the heap.
  heap->start_address = start;
  heap->end_address = end;
  heap->max_address = max;
  heap->supervisor = supervisor;
  heap->readonly = readonly;

  //Initially there is just 1 large hole.
  header_t* hole = (header_t*)start;
  hole->size = end - start;
  hole->magic = HEAP_MAGIC;
  hole->is_hole = 1;
  insert_ordered_array((void*)hole, &heap->index);

  return heap;
}

//****************************************************************************

void* alloc(u32 size, u8 page_align, heap_t* heap) {
  // Taking header/footer into account.
  u32 size_with_hf = size + sizeof(header_t) + sizeof(footer_t);
  // Finding the smallest hole to fit this.
  u32 iter = find_smallest_hole(size_with_hf, page_align, heap);
  // If iter == -1, then no suitable hole found.
  if(iter == -1) {
    // We can expand the size of the heap.
    u32 old_length = heap->end_address - heap->start_address;
    u32 old_end_address = heap->end_address;
    // Need to allocate more space to expand the heap.
    expand(old_length + size_with_hf, heap);
    u32 new_length = heap->end_address - heap->start_address;
    // Find the location of the last header in the heap.
    iter = 0;
    s32 idx = -1;
    u32 value = 0x0;
    while(iter < heap->index.size) {
      u32 tmp = (u32)lookup_ordered_array(iter, &heap->index);
      if(tmp > value) {
        value = tmp;
        idx = iter;
      }
      ++iter;
    }
    // Now idx has the index of the last header in the heap.
    // If we did not find any headers, we need to add one.
    if(idx == -1) {
      header_t* header = (header_t*)old_end_address;
      header->magic = HEAP_MAGIC;
      header->is_hole = 1;
      header->size = new_length - old_length;
      footer_t* footer = 
        (footer_t*)(old_end_address + header->size - sizeof(footer_t));
      footer->magic = HEAP_MAGIC;
      footer->header = header;
      insert_ordered_array((void*)header, &heap->index);
    }
    else {
      // Adjusting the last header.
      header_t* header = lookup_ordered_array(idx, &heap->index);
      header->size += new_length - old_length;
      // Write footer to the new end of the hole.
      footer_t* footer = 
        (footer_t*)((u32)header + header->size - sizeof(footer_t));
      footer->header = header;
      footer->magic = HEAP_MAGIC;
    }
    // Now we can try reallocating as the request will be satisfied by the hole
    // created with increased size.
    return alloc(size, page_align, heap);
  }
  // We found a suitable hole.
  header_t* orig_hole_header = (header_t*)lookup_ordered_array(iter,
                                                               &heap->index);
  u32 orig_hole_pos = (u32)orig_hole_header;
  u32 orig_hole_size = orig_hole_header->size;

  // Check if we can split the hole into two parts, by checking if the remainder
  // part can atleast have a header and a footer in it.
  if(orig_hole_size - size_with_hf <= sizeof(header_t) + sizeof(footer_t)) {
    // No use splitting. Hence, take the whole hole.
    size = orig_hole_size - sizeof(header_t) - sizeof(footer_t);
    size_with_hf = orig_hole_size;
  }

  // If data is to be page-aligned, doing it, and making a hole before the
  // block.
  if(page_align && (orig_hole_pos & 0xFFFFF000)) {
    // The position from where the block's header should start so that the data
    // can then be page-aligned. This leaves a hole before the header of the
    // data block.
    u32 new_pos = orig_hole_pos - (orig_hole_pos & 0xFFF) 
                  + 0x1000 - sizeof(header_t);
    // The header to the hole left before the data block's header.
    header_t* hole_header = (header_t*)orig_hole_pos;
    // The size left for the hole.
    hole_header->size = new_pos - orig_hole_pos;
    hole_header->magic = HEAP_MAGIC;
    hole_header->is_hole = 1;
    // The footer for the hole formed.
    footer_t* hole_footer = (footer_t*)(new_pos - sizeof(footer_t));
    hole_footer->magic = HEAP_MAGIC;
    hole_footer->header = hole_header;
    // Now we have updated the hole before the location where the data block
    // will be placed.
    // These are the details of the hole alloted to fit the data block.
    orig_hole_pos = new_pos;
    orig_hole_size = orig_hole_size - hole_header->size;
  }
  else {
    // We will fill this hole and won't need it, so deleting it from the index.
    delete_ordered_array(iter, &heap->index);
  }

  // Overwriting the original header
  header_t* block_header = (header_t*)orig_hole_pos;
  block_header->magic = HEAP_MAGIC;
  block_header->is_hole = 0;
  block_header->size = size_with_hf;
  // The footer
  footer_t* block_footer = 
    (footer_t*)(orig_hole_pos + size_with_hf - sizeof(footer_t));
  block_footer->magic = HEAP_MAGIC;
  block_footer->header = block_header;

  // We may need to write a new hole after the block, with the space left in the
  // end. We only do this if the new hole has some positive size.
  if(orig_hole_size - size_with_hf > 0) {
    // The header of the hole created after the data block.
    header_t* hole_header = (header_t*)(orig_hole_pos + size_with_hf);
    hole_header->magic = HEAP_MAGIC;
    hole_header->is_hole = 1;
    hole_header->size = orig_hole_size - size_with_hf;
    // The footer.
    footer_t* hole_footer = 
      (footer_t*)((u32)hole_header + hole_header->size - sizeof(footer_t));
    // Checking if we can place the footer.
    if((u32)hole_footer < heap->end_address) {
      hole_footer->magic = HEAP_MAGIC;
      hole_footer->header = hole_header;
    }
    // Put this new hole in the index.
    insert_ordered_array((void*)hole_header, &heap->index);
  }
  // Returing the beginning of the data block's data space.
  return (void*)((u32)block_header + sizeof(header_t) );
}

//****************************************************************************

void free(void* p, heap_t* heap) {
  // If p is null, then nothing is to be done.
  if(p == 0) {
    return;
  }
  // p is the address of the start of data. Header lies before it.
  // Getting the header and footer of the block to be freed.
  header_t* header = (header_t*)(p - sizeof(header_t));
  footer_t* footer = (footer_t*)((u32)header + header->size - sizeof(footer_t));

  // Sanity checks.
  if(header->magic != HEAP_MAGIC || footer->magic != HEAP_MAGIC) {
    printk("Magic number doesn't resemble HEAP_MAGIC.\n");
    return;
  }

  // Making this a hole.
  header->is_hole = 1;
  // If we wish to make this entry in the heap holes index.
  char do_add = 1;

  // Checking if there is a hole adjacent to the left, then this hole can be
  // merged to the left one.
  footer_t* left_footer = (footer_t*)((u32)header - sizeof(footer_t));
  if(left_footer->magic == HEAP_MAGIC && left_footer->header->is_hole == 1) {
    u32 additional_size = header->size;
    header = left_footer->header;
    header->size += additional_size;
    footer->header = header;
    // Now we don't have to put this in the heap index.
    do_add = 0;
  }

  // Checking if there is a hole to right, then the right hole can be merged to
  // this hole.
  header_t* right_header = (header_t*)((u32)header + header->size);
  if(right_header->magic == HEAP_MAGIC && right_header->is_hole == 1) {
    header->size += right_header->size;
    footer_t* right_footer = 
      (footer_t*)((u32)right_header + right_header->size - sizeof(footer_t));
    right_footer->header = header;
    footer = right_footer;
    // We need to find and remove the right_header from the index.
    u32 iter = 0;
    while((iter < heap->index.size) &&
          (lookup_ordered_array(iter, &heap->index) != (void*)right_header)) {
      ++iter;
    }
    // Making sure we found the header
    if(iter >= heap->index.size) {
      printk("Right header not found in the heap while merging in free.\n");
      // Hang
      while(1);
    }
    // Removing from the index.
    delete_ordered_array(iter, &heap->index);
  }
  
  // If the footer address is the end address, contraction of the heap can be
  // done.
  if((u32)footer + sizeof(footer_t) == heap->end_address) {
    u32 old_length = heap->end_address - heap->start_address;
    u32 new_length = contract((u32)header - heap->start_address, heap);
    // Checking the size of the heap after resizing.
    if(header->size - (old_length - new_length) > 0) {
      // The hole will still exist.
      header->size -= (old_length - new_length);
      footer = (footer_t*)((u32)header + header->size - sizeof(footer_t));
      footer->magic = HEAP_MAGIC;
      footer->header = header;
    }
    else {
      // The hole will not exist. Hence remove from the index.
      u32 iter = 0;
      while((iter < heap->index.size) &&
          (lookup_ordered_array(iter, &heap->index) != (void*)header)) {
        ++iter;
      }
      if(iter < heap->index.size) {
        delete_ordered_array(iter, &heap->index);
      }
    }
  }

  // Check if the hole is to be added to the index.
  if(do_add == 1) {
    insert_ordered_array((void*)header, &heap->index);
  }
}

//****************************************************************************
