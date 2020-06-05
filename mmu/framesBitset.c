#include "framesBitset.h"

void set_frame(u32 frame_addr) {
  u32 frame_no = frame_addr / 0x1000; //Each frame is 0x1000 long.
  u32 idx = INDEX_FROM_BIT(frame_no);
  u32 offset = OFFSET_FROM_BIT(frame_no);
  frames[idx] |= (0x1 << offset);
}

void clear_frame(u32 frame_addr) {
  u32 frame_no = frame_addr / 0x1000;
  u32 idx = INDEX_FROM_BIT(frame_no);
  u32 offset = OFFSET_FROM_BIT(frame_no);
  frames[idx] &= ~(0x1 << offset);
}

u32 test_frame(u32 frame_addr) {
  u32 frame_no = frame_addr / 0x1000;
  u32 idx = INDEX_FROM_BIT(frame_no);
  u32 offset = OFFSET_FROM_BIT(frame_no);
  return (frames[idx] & (0x1 << offset));
}

u32 first_frame() {
  u32 i,j;
  for(i = 0; i < nframes / 32; i++) {
    if(frames[i] == 0xFFFFFFFF) {
      //No frame free
      continue;
    }
    for(j = 0; j < 32; j++) {
      if(!(frames[i] & (0x1 << j))) {
          return (i * 32 + j);
          break;
      }
    }
  }
  //No free frame
  // TODO: Interrupt and return
  return 0;
}

