#include "../kernel/util.h"
#include "ports.h"
#include "screen.h"


/* Private kernel functions declaration */
int print_char(char character, int col, int row, char attribute_type);
int get_cursor_offset();
void set_cursor_offset(int offset);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);
int handle_scrolling(int offset);

/*******************************************************************************
 * Public Kernel Functions *
 ******************************************************************************/

//To clear the screen
void clear_screen() {
	int screen_size = MAX_COLS * MAX_ROWS;
	char* screen = VIDEO_ADDRESS;
	int i;
	for(i = 0; i < screen_size; i++) {
		screen[i*2] = ' ';
		screen[i*2 + 1] = WHITE_ON_BLACK;
	}
	
	set_cursor_offset(get_offset(0,0));
}

//To print the text at a specific location
void printk_at(char* text, int col, int row) {
	int offset;
	if(col >=0 && row >= 0) {
		offset = get_offset(col,row);
	}
	else {
		offset = get_cursor_offset();
		col = get_offset_col(offset);
		row = get_offset_row(offset);
	}

	int i = 0;
	while(text[i] != '\0') {
		offset = print_char(text[i++], col, row, WHITE_ON_BLACK);
		col = get_offset_col(offset);
		row = get_offset_row(offset);
	}
}

//To print at the location of the cursor
void printk(char* text) {
	printk_at(text, -1, -1);
}

/*******************************************************************************
* Private Kernel Functions *
*******************************************************************************/

//Print a character on screen at (row,col) or cursor position and return
//the offset for the next character.
int print_char(char character, int col, int row, char attribute_byte) {
	//Byte pointer to point to the start of the video memory
	unsigned char* vidmem = (unsigned char*)VIDEO_ADDRESS;

	//If attribute byte is zero, assume default style
	if(!attribute_byte) {
		attribute_byte = WHITE_ON_BLACK;
	}

	//Get the video memory offset.
	int offset;

	if(col >= MAX_COLS || row >= MAX_ROWS) {
		offset = get_offset(MAX_COLS - 1, MAX_ROWS - 1);
		vidmem[offset] = 'E';
		vidmem[offset + 1] = RED_ON_WHITE;
		return (offset + 2);
	}

	//If col and row are non-negative, use them for offset
	if(col >= 0 && row >=0) {
		offset = get_offset(col, row);
	}
	else {
		offset = get_cursor_offset();
	}

	//On a new line character set offset to a new line.
	if(character == '\n') {
		int offset_row = get_offset_row(offset);
		offset = get_offset(0, offset_row+1);
	}
	else {
		vidmem[offset] = character;
		vidmem[offset+1] = attribute_byte;
		//Updating offset for next character
		offset += 2;
	}
	//Make scrolling adjustment, for when we reach the bottom of the screen.
	offset = handle_scrolling(offset);
	//Update cursor position
	set_cursor_offset(offset);
	return offset;
}

//To return the offset of the cursor on the screen
int get_cursor_offset() {
	int offset;
	port_byte_out(REG_SCREEN_CTRL, 14);
	offset = port_byte_in(REG_SCREEN_DATA) << 8;
	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA);
	return offset*2;
}

//To set the cursor offset
void set_cursor_offset(int offset) {
	offset /= 2;
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}


//To get offset from col,row
int get_offset(int col, int row) {
	return 2 * (row * MAX_COLS + col);
}

//To get the row number from the offset
int get_offset_row(int offset) {
	offset /= 2;
	return offset / MAX_COLS;
}

//To get the col number from the offset
int get_offset_col(int offset) {
	offset /= 2;
	return offset % MAX_COLS;
}

//To scroll the screen
int handle_scrolling(int offset) {
	if(offset < MAX_ROWS * MAX_COLS * 2) {
		return offset;
	}
	//Copying rows back by 1
	int i;
	for(i = 1; i < MAX_ROWS; i++) {
		memory_copy(VIDEO_ADDRESS + get_offset(0, i), 
								VIDEO_ADDRESS + get_offset(0, i - 1), 
								MAX_COLS * 2);
	}
	char* last_line = VIDEO_ADDRESS + get_offset(0, MAX_ROWS - 1);
	//Blanking last line
	for(i = 0; i < 2 * MAX_COLS; i += 2) {
		last_line[i] = ' ';
		last_line[i + 1] = WHITE_ON_BLACK;
	}
	//Returning cursor offset 1 row back
	offset -= 2 * MAX_COLS;

	return offset;
}
