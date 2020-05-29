[bits 32]

;Defining constants
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

;asm code to print a null-terminated string
print_without_bios:
	pusha
	mov edx, VIDEO_MEMORY ;edx is the memory address where the current
												;character and its attributes will be written
												;and further read by the display hardware

begin_print_without_bios:
	mov al, [ebx]
	mov ah, WHITE_ON_BLACK

	cmp al,0
	jmp end_print_without_bios
	
	mov [edx], ax ;Writing in memory at address edx
	add edx, 2 ;Going to next address
	
	add ebx, 1 ;Address of next character in the string

end_print_without_bios:
	popa
	ret
