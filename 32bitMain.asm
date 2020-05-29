;Boot sector that enters 32 bits protected mode
[org 0x7c00]

mov bp, 0x9000 ;Setting the stack
mov sp, bp

mov bx, MSG_REAL_MODE
call print16bit

call switch_to_pm ;Function call to switch to 32 bits protected mode

jmp $

%include "print16bit.asm"
%include "loadGDT.asm"
%include "print32bit.asm"
%include "32bitSwitch.asm"

[bits 32]
begin_pm: ;We arrive here after switching to 32 bits protected mode
	mov ebx, MSG_PROT_MODE
	call print32bit
	
	jmp $ 

MSG_REAL_MODE:
	db "Started in 16-bit mode", 0
MSG_PROT_MODE:
	db "Successfully landed in 32-bit mode", 0

;BootSector padding
times 510 - ($-$$) db 0
dw 0xaa55
