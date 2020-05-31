C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

#TODO: Make source dep on all header files

OBJ = ${C_SOURCES:.c=.o}

all: os-image

run: os-image
	qemu os-image

os-image: boot/bootSector.bin kernel.bin
	cat $^ > os-image

kernel.bin: boot/kernelEntry.o ${OBJ}
	ld -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: %.c ${HEADERS}
	gcc -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf64 -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

#clean:
#	rm -fr *.bin *.dis *.o os-image *.map

SUBDIR_ROOTS := kernel boot drivers
DIRS := . $(shell find $(SUBDIR_ROOTS) -type d)
GARBAGE_PATTERNS := *.o *.bin *.dis os-image *.map
GARBAGE := $(foreach DIR,$(DIRS),$(addprefix $(DIR)/,$(GARBAGE_PATTERNS)))

clean:
	rm -rf $(GARBAGE)
