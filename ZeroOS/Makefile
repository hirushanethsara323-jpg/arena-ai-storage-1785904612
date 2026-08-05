CC = gcc
CFLAGS = -ffreestanding -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -O2 -std=gnu99 -c
BOOT_CFLAGS = -ffreestanding -c

LDFLAGS = -n -T linker.ld
OBJECTS = boot.o kernel.o keyboard.o shell.o gdt.o idt.o pic.o pmm.o heap.o
KERNEL = zero-kernel.elf

all: kernel

boot.o: boot.S
	$(CC) $(BOOT_CFLAGS) boot.S -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

keyboard.o: keyboard.c keyboard.h io.h
	$(CC) $(CFLAGS) keyboard.c -o keyboard.o

shell.o: shell.c shell.h io.h keyboard.h heap.h pmm.h
	$(CC) $(CFLAGS) shell.c -o shell.o

gdt.o: gdt.c gdt.h io.h
	$(CC) $(CFLAGS) gdt.c -o gdt.o

idt.o: idt.c idt.h io.h
	$(CC) $(CFLAGS) idt.c -o idt.o

pic.o: pic.c pic.h io.h
	$(CC) $(CFLAGS) pic.c -o pic.o

pmm.o: pmm.c pmm.h io.h
	$(CC) $(CFLAGS) pmm.c -o pmm.o

heap.o: heap.c heap.h io.h
	$(CC) $(CFLAGS) heap.c -o heap.o

kernel: $(OBJECTS)
	ld $(LDFLAGS) -o $(KERNEL) $(OBJECTS)
	@echo "✅ Zero OS v0.3 Kernel built: $(KERNEL)"
	@ls -lh $(KERNEL)
	@file $(KERNEL) || true
	@echo "Size: $$(du -h $(KERNEL) | cut -f1)"
	@echo "Objects: $(OBJECTS)"
	@echo ""
	@echo "Symbols:"
	@nm $(KERNEL) | grep -E "kernel_main|gdt_|idt_|pmm_|kmalloc|shell_" | head -n 30

clean:
	rm -f *.o $(KERNEL) zero-os.iso
	rm -rf iso
