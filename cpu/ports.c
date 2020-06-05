# include "ports.h"
// To read a byte from a specific port (some memory address directly mapped
// to a device controller's register.
unsigned char port_byte_in(u16 port) {
  // Variable to store the value read and return to the caller function.
  unsigned char result;
  // Syntax : "<operation> <src>, <dest>" 
  //          : <output operand> 
  //          : <input operand 1>, <input operand 2>, ...
  // "=a" (result) : Put the value of al register in result
  // "d" (port) : Put the value of port in edx
  // Example : If port = 0x3F2 then edx := 0x3F2
  __asm__ ("in %%dx, %%al" : "=a" ( result ) : "d" ( port )); 
  return result;
}

// To write a byte to a specific port.
void port_byte_out(u16 port, u8 data) {
  __asm__ ("out %%al, %%dx" :: "a" (data), "d" (port));
}

// To read a word (2 bytes) from a specific port.
unsigned short port_word_in(u16 port) {
  unsigned short result;
  __asm__ ("in %%dx, %%ax" : "=a" (result) : "d" (port));
}

// To write a word (2 bytes) to a specific port.
void port_word_out(u16 port, u16 data) {
  __asm__ ("out %%ax, %%dx" :: "a" (data), "d" (port));
}
