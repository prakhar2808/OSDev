;Ensures that we jump to the main label of the kernel no matter where it is
;located.

[bits 32]

[extern main] ;Declare that we need the external symbol main, so the linker can
							;substitute the final address

call main ;To invoke the main() function

jmp $
