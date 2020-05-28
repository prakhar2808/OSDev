; loads dh sectors to es:bx from drive dl
disk_load:
	push dx ;store dx in stack for safety
	mov ah, 0x02 ;Read sector function
	mov al, dh ;Number of sectors to be read
	mov ch, 0x00 ;Cylinder number 0 (base = 0).
	mov dh, 0x00 ;Head number 0 (base = 0).
	mov cl, 0x02 ;Start read from second sector (base = 1).

	int 0x13

	jc disk_error ;Error detected from flags
	
	pop dx
	cmp dh, al ;al has been set to the number of sectors actually read.
						 ;if al != dh, then error.
	jne disk_error
	ret

disk_error:
	mov bx, DISK_ERROR_MSG
	call print
	jmp $ ;Hang here due to error.

;Variables
DISK_ERROR_MSG:
	db 'Error in reading from disk',0
