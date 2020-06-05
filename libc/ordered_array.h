#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H

#include "../cpu/types.h"
#include "../libc/mem.h"

// The array is sorted always. It can store anything that can be cast to void*.
typedef void* type_t;

// Returns 0 if second argument is less than first argument, else return
// anything non-zero.
typedef s8 (*lessthan_predicate_t)(type_t, type_t);

typedef struct ordered_array{
  type_t* array;
  u32 size;
  u32 max_size;
  lessthan_predicate_t less_than;
} ordered_array_t;

// Standard less than predicate
s8 standard_lessthan_predicate(type_t a, type_t b);

// Create ordered array
ordered_array_t create_ordered_array(u32 max_size, 
                                     lessthan_predicate_t less_than);
ordered_array_t place_ordered_array(void* addr,
                                    u32 max_size,
                                    lessthan_predicate_t less_than);

// Destroy ordered array
void destroy_ordered_array(ordered_array_t* array);

// Insert an item into the array
void insert_ordered_array(type_t item, ordered_array_t* array);

// Lookup the item at index i
type_t lookup_ordered_array(u32 i, ordered_array_t* array);

// Deletes the item at index i
type_t delete_ordered_array(u32 i, ordered_array_t* array);

#endif
