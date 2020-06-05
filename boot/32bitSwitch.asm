[bits 16]
switch_to_pm:
    cli ;Disable interrupts as we will declare our own in 32 bits mode.
    lgdt [gdt_descriptor] ;Load the GDT descriptor
    mov eax, cr0
    or eax, 0x1 ;Setting 32-bit mode bit in cr0
    mov cr0, eax
    jmp CODE_SEG:init_pm ;Far jump by using a different segment

[bits 32]
init_pm: ; we are now using 32-bit instructions
    mov ax, DATA_SEG ;Update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0xa0000 ;Update the stack right at the top of the free space
    mov esp, ebp

    call begin_pm ;Call a well-known label with useful code
