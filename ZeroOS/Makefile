CC = gcc
# Try to be compatible with sandbox without multilib
CFLAGS = -ffreestanding -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -O2 -std=gnu99 -c
# For boot we try m32 if possible, else fallback
BOOT_CFLAGS = -ffreestanding -c

LDFLAGS = -n -T linker.ld
OBJECTS = boot.o kernel.o
KERNEL = zero-kernel.elf
ISO = zero-os.iso

all: kernel

boot.o: boot.S
	$(CC) $(BOOT_CFLAGS) boot.S -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

kernel: $(OBJECTS)
	ld $(LDFLAGS) -o $(KERNEL) $(OBJECTS)
	@echo "✅ Zero OS Kernel built: $(KERNEL)"
	@ls -lh $(KERNEL)
	@file $(KERNEL) || true
	@echo "Multiboot check:"
	@hexdump -C $(KERNEL) | grep -i "BADB002" || echo "No multiboot magic found in hex (will check with objdump)"
	@objdump -h $(KERNEL) | head -n 20 || true

clean:
	rm -f *.o $(KERNEL) $(ISO)
	rm -rf iso
