print_hex:
	pusha
	mov cx, 0

begin_print_hex:
	cmp cx, 4
	je end_print_hex
	mov ax, dx
	and ax, 0x000f ;Getting last bit
	add al, 0x30 ;Converting to ascii
	cmp al, 0x39 
	jle next_step_print_hex
	add al, 7 ;If double digit, converting to letter

next_step_print_hex:
	mov bx, hex_out+5
	sub bx, cx ;Index in hex_out that is to be modified
	mov [bx], al
	ror dx, 4 ;Rotating to bring the first bit to the end
	
	add cx,1
	jmp begin_print_hex

end_print_hex:
	mov bx, hex_out
	call print
	popa
	ret

hex_out:
	db '0x0000',0
		
