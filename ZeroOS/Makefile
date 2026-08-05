CC = gcc
CFLAGS = -ffreestanding -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -O2 -std=gnu99 -c
BOOT_CFLAGS = -ffreestanding -c

LDFLAGS = -n -T linker.ld
OBJECTS = boot.o kernel.o keyboard.o shell.o
KERNEL = zero-kernel.elf

all: kernel

boot.o: boot.S
	$(CC) $(BOOT_CFLAGS) boot.S -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

keyboard.o: keyboard.c keyboard.h io.h
	$(CC) $(CFLAGS) keyboard.c -o keyboard.o

shell.o: shell.c shell.h io.h keyboard.h
	$(CC) $(CFLAGS) shell.c -o shell.o

kernel: $(OBJECTS)
	ld $(LDFLAGS) -o $(KERNEL) $(OBJECTS)
	@echo "✅ Zero OS v0.2 Kernel built: $(KERNEL)"
	@ls -lh $(KERNEL)
	@file $(KERNEL) || true
	@echo "Size: $$(du -h $(KERNEL) | cut -f1)"
	@echo "Objects: $(OBJECTS)"

clean:
	rm -f *.o $(KERNEL) zero-os.iso
	rm -rf iso

# GitHub sync helper
push:
	cd /home/user && ./github-sync.sh || git add ZeroOS && git commit -m "Zero OS v0.2 - Shell + KBD" && git push origin main
