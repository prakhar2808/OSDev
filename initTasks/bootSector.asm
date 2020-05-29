[org 0x7c00]

mov bx, hello_string
call print
call print_newline

mov bp, 0x8000
mov sp, bp ;Getting the stack out of the way

mov bx, 0x9000 ;Load sectors in es:bx
mov dh, 5 ;Number of sectors to load
mov dl, [BOOT_DRIVE]
call disk_load

mov dx, [0x9000] ;Print the first word loaded from the first sector
call print_hex
call print_newline

mov dx, [0x9000 + 512] ;Print the first word from the next sector
call print_hex
call print_newline

mov bx, bye_string
call print
call print_newline

;infinite loop
jmp $

;Functions included
%include "printString.asm"
%include "printHexString.asm"
%include "diskLoad.asm"

;Global data
hello_string:
	db 'Hello World!',0

bye_string:
	db 'Goodbye World!',0

BOOT_DRIVE:
	db 0

; Fill zeros for the next bytes until the last 2 bytes
times 510-($-$$) db 0

; Magic number
dw 0xaa55

; Filling the next two sectors
times 256 dw 0xdada
times 256 dw 0xface
