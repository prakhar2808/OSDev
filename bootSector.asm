[org 0x7c00]

mov bx, hello_string
call print

call print_newline

mov bx, bye_string
call print

call print_newline

mov dx, 0x1f2e
call print_hex

call print_newline

;infinite loop
jmp $

%include "printString.asm"
%include "printHexString.asm"

hello_string:
	db 'Hello World!',0

bye_string:
	db 'Goodbye World!',0

; Fill zeros for the next bytes until the last 2 bytes
times 510-($-$$) db 0

; Magic number
dw 0xaa55
