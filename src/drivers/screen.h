#ifndef SCREEN_H
#define SCREEN_H

#include "../libc/mem.h"
#include "../cpu/ports.h"

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

//Attribute byte for color schemes
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

//Screen device IO ports
//Write 14 and 15 to REG_SCREEN_CTRL memory address to get the upper 
//and lower bytes respectively of the cursor_offset/2 in REG_SCREEN_DATA
//memory address.
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

//Public kernel functions
void clear_screen();
void printk_at(char* text, int col, int row);
void printk(char* text);
void printk_backspace();

#endif
