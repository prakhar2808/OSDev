; GDT with two completely overlapping segments - code and data.
gdt_start:

;Filling the first 8 bytes with zeros
gdt_null:
	dd 0x0 ;dd fills double word (4 bytes). A word is 16 bits (2 bytes).
	dd 0x0

gdt_code:
;base = 0x0 (32 bits), limit = 0xfffff (20 bits)
;flags - (present)1 privilege(00) (descriptor type)1 == 1001b
;type flags - (code)1 (conforming)0 (readable)1 (accessed)0 == 1010b
;flags - (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0 == 1100b

;Filling as per the structure
	dw 0xffff
	dw 0x0
	db 0x0
	db 10011010b
	db 11001111b
	db 0x0

gdt_data:
;Same as above
;type flags - (code)0 (expand_down)0 (writable)1 (accessed)0 == 0010b

;Filling as per the structure
	dw 0xffff
	dw 0x0
	db 0x0
	db 10010010b
	db 11001111b
	db 0x0

gdt_end: ;Put only to calculate the size of the GDT for the GDT descriptor

gdt_descriptor:
	dw gdt_end - gdt_start - 1 ;Size of the gdt
	dd gdt_start ;Start address of gdt

;Useful constants
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

