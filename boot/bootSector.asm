; Boot sector to load a C kernel in 32-bit protected mode.
[org 0x7c00] ;The memory before this is occupied by BIOS routines.

KERNEL_OFFSET equ 0x8000 ;The memory offset where we will load the kernel

mov [BOOT_DRIVE], dl ;BIOS stores the boot_drive in dl.
										 ;Remembering it for later.

mov bp, 0x9000 ;Setting up the stack
mov sp,bp

mov bx, MSG_REAL_MODE ;Announce that we are starting in 16-bit mode.
call print16bit
call print_newline

call load_kernel ;Load the kernel into the memory

call switch_to_pm ;Switch to 32-bit protected mode, won't return from there

jmp $

; Including useful assembly routines
%include "boot/print16bit.asm"
%include "boot/print32bit.asm"
%include "boot/32bitSwitch.asm"
%include "boot/loadGDT.asm"
%include "boot/diskLoad.asm"

[bits 16] ;Kernel is being loaded while in bits 16
load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print16bit
	call print_newline
  
  ;Kernel is loaded at [es:bx]. We load the kernel at memory addres 0x100000.
  ;For that, es = 0xf800 and bx = 0x8000.
  ;Final address = (es<<4) + bx = 0x100000.
  mov ax, 0xf800
  mov es, ax
	mov bx, KERNEL_OFFSET ;Memory init address to load kernel.
	mov dh, 100 ;Number of sectors to load -- 100*512 bytes will be loaded.
	mov dl, [BOOT_DRIVE] ;Boot drive from where to load
	call disk_load

	ret

[bits 32] ;We are now in 32 bits mode. We arrive here after switching to pm.
begin_pm:
	mov ebx, MSG_PROT_MODE
	call print32bit

  ;Kernel is loaded at 0x100000
	jmp 0x100000 ;Jump to the first instruction in the kernel code
	
	jmp $

;Global variables
BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit mode.",0
MSG_PROT_MODE db "Successfully landed in 32-bit mode.",0
MSG_LOAD_KERNEL db "Loading kernel into memory.",0

;Bootsector padding
times 510 - ($-$$) db 0
dw 0xaa55

