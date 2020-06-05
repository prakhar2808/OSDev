#include "ordered_array.h"

s8 standard_lessthan_predicate(type_t a, type_t b) {
  if(a < b) {
    return 1;
  }
  return 0;
}

ordered_array_t create_ordered_array(u32 max_size,
                                     lessthan_predicate_t less_than) {
  ordered_array_t object;
  object.array = (void*)kmalloc(max_size * sizeof(type_t), 0);
  memset(object.array, 0, sizeof(object.array));
  object.size = 0;
  object.max_size = max_size;
  object.less_than = less_than;
  return object;
}

ordered_array_t place_ordered_array(void* addr,
                                    u32 max_size,
                                    lessthan_predicate_t less_than) {
  ordered_array_t object;
  object.array = (type_t*)addr;
  memset(object.array, 0, sizeof(type_t) * max_size);
  object.size = 0;
  object.max_size = max_size;
  object.less_than = less_than;
  return object;
}

void destroy_ordered_array(ordered_array_t* array) {

}

void insert_ordered_array(type_t item, ordered_array_t* array) {
  if(!(array->less_than)) {
    printk("less_than not defined for the ordered array\n");
    return;
  }
  u32 i = 0;
  while(i < array->size && array->less_than(array->array[i], item)) {
    i++;
  }
  if(i == array->size) {
    array->array[array->size] = item;
    ++array->size;
  }
  else {
    type_t tmp = array->array[i];
    array->array[i] = item;
    while(i < array->size) {
      ++i;
      type_t tmp2 = array->array[i];
      array->array[i] = tmp;
      tmp = tmp2;
    }
    ++array->size;
  }
}

type_t lookup_ordered_array(u32 i, ordered_array_t* array) {
  if(i >= array->size) {
    printk("Index greater than ordered array size\n");
    return 0;
  }
  return array->array[i];
}

type_t delete_ordered_array(u32 i, ordered_array_t* array) {
  while(i < array->size - 1) {
    array->array[i] = array->array[i+1];
    i++;
  }
  --array->size;
}
