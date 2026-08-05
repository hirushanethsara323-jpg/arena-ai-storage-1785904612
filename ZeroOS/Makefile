CC = gcc
CFLAGS = -ffreestanding -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -O2 -std=gnu99 -c
BOOT_CFLAGS = -ffreestanding -c

LDFLAGS = -n -T linker.ld
OBJECTS = boot.o kernel.o keyboard.o shell.o gdt.o idt.o pic.o pmm.o heap.o pit.o fs/ata.o fs/ramfs.o fs/vfs.o gui/fb.o gui/mouse.o gui/compositor.o userland/elf.o userland/syscall.o userland/task.o
KERNEL = zero-kernel.elf

all: kernel

boot.o: boot.S
	$(CC) $(BOOT_CFLAGS) boot.S -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

keyboard.o: keyboard.c keyboard.h io.h
	$(CC) $(CFLAGS) keyboard.c -o keyboard.o

shell.o: shell.c shell.h io.h keyboard.h heap.h pmm.h fs/vfs.h gui/compositor.h userland/task.h
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

pit.o: pit.c pit.h io.h
	$(CC) $(CFLAGS) pit.c -o pit.o

fs/ata.o: fs/ata.c fs/ata.h io.h
	$(CC) $(CFLAGS) fs/ata.c -o fs/ata.o

fs/ramfs.o: fs/ramfs.c fs/ramfs.h heap.h io.h
	$(CC) $(CFLAGS) fs/ramfs.c -o fs/ramfs.o

fs/vfs.o: fs/vfs.c fs/vfs.h fs/ramfs.h fs/ata.h heap.h io.h
	$(CC) $(CFLAGS) fs/vfs.c -o fs/vfs.o

gui/fb.o: gui/fb.h gui/fb.c io.h
	$(CC) $(CFLAGS) gui/fb.c -o gui/fb.o

gui/mouse.o: gui/mouse.h gui/mouse.c io.h
	$(CC) $(CFLAGS) gui/mouse.c -o gui/mouse.o

gui/compositor.o: gui/compositor.h gui/compositor.c gui/fb.h heap.h io.h
	$(CC) $(CFLAGS) gui/compositor.c -o gui/compositor.o

userland/elf.o: userland/elf.h userland/elf.c heap.h pmm.h io.h
	$(CC) $(CFLAGS) userland/elf.c -o userland/elf.o

userland/syscall.o: userland/syscall.h userland/syscall.c fs/vfs.h io.h
	$(CC) $(CFLAGS) userland/syscall.c -o userland/syscall.o

userland/task.o: userland/task.h userland/task.c heap.h pmm.h io.h
	$(CC) $(CFLAGS) userland/task.c -o userland/task.o

kernel: $(OBJECTS)
	ld $(LDFLAGS) -o $(KERNEL) $(OBJECTS)
	@echo "✅ Zero OS v0.6 Kernel built: $(KERNEL)"
	@ls -lh $(KERNEL)
	@file $(KERNEL) || true
	@echo "Size: $$(du -h $(KERNEL) | cut -f1)"

clean:
	rm -f *.o $(KERNEL) zero-os.iso fs/*.o gui/*.o userland/*.o
	rm -rf iso
