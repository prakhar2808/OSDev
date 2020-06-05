// Bitset for frames - used or free
#ifndef FRAMES_BITSET_H
#define FRAMES_BITSET_H

#include "../cpu/types.h"
#include "../libc/mem.h"

// The bitset
u32 *frames;
// Number of frames
u32 nframes;

//Defined in mem.h
extern u32 free_memory_addr;

//Macros to be used here
#define INDEX_FROM_BIT(a) (a / 32)
#define OFFSET_FROM_BIT(a) (a % 32)

//To set a bit in frames bitset
void set_frame(u32 frame_addr);

//To clear a bit in frames bitset
void clear_frame(u32 frame_addr);

//To test if a bit is set in frames bitset
u32 test_frame(u32 frame_addr);

//To find first free frame
u32 first_frame();

#endif
