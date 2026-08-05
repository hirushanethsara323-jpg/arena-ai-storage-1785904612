CC = gcc
CFLAGS = -ffreestanding -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -O2 -std=gnu99 -c
BOOT_CFLAGS = -ffreestanding -c

LDFLAGS = -n -T linker.ld
OBJECTS = boot.o kernel.o keyboard.o shell.o gdt.o idt.o pic.o pmm.o heap.o pit.o paging.o speaker.o smp.o context.o pci.o usb.o net.o fs/ata.o fs/ramfs.o fs/vfs.o fs/zerofs2.o fs/fat32.o gui/fb.o gui/mouse.o gui/compositor.o userland/elf.o userland/syscall.o userland/task.o userland/app.o
KERNEL = zero-kernel.elf

all: kernel

boot.o: boot.S
	$(CC) $(BOOT_CFLAGS) boot.S -o boot.o

context.o: context.S
	$(CC) $(BOOT_CFLAGS) context.S -o context.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

keyboard.o: keyboard.c keyboard.h io.h
	$(CC) $(CFLAGS) keyboard.c -o keyboard.o

shell.o: shell.c shell.h io.h keyboard.h heap.h pmm.h fs/vfs.h gui/compositor.h userland/task.h userland/app.h paging.h pci.h
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

pit.o: pit.c pit.h idt.h io.h
	$(CC) $(CFLAGS) pit.c -o pit.o

paging.o: paging.c paging.h pmm.h heap.h io.h
	$(CC) $(CFLAGS) paging.c -o paging.o

speaker.o: speaker.c speaker.h pit.h io.h
	$(CC) $(CFLAGS) speaker.c -o speaker.o

smp.o: smp.c smp.h io.h
	$(CC) $(CFLAGS) smp.c -o smp.o

pci.o: pci.c pci.h io.h
	$(CC) $(CFLAGS) pci.c -o pci.o

usb.o: usb.c usb.h pci.h io.h
	$(CC) $(CFLAGS) usb.c -o usb.o

net.o: net.c net.h pci.h io.h
	$(CC) $(CFLAGS) net.c -o net.o

fs/ata.o: fs/ata.c fs/ata.h io.h
	$(CC) $(CFLAGS) fs/ata.c -o fs/ata.o

fs/ramfs.o: fs/ramfs.c fs/ramfs.h heap.h io.h
	$(CC) $(CFLAGS) fs/ramfs.c -o fs/ramfs.o

fs/vfs.o: fs/vfs.c fs/vfs.h fs/ramfs.h fs/ata.h heap.h io.h
	$(CC) $(CFLAGS) fs/vfs.c -o fs/vfs.o

fs/zerofs2.o: fs/zerofs2.h fs/zerofs2.c fs/ata.h heap.h pmm.h io.h
	$(CC) $(CFLAGS) fs/zerofs2.c -o fs/zerofs2.o

fs/fat32.o: fs/fat32.h fs/fat32.c fs/ata.h heap.h io.h
	$(CC) $(CFLAGS) fs/fat32.c -o fs/fat32.o

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

userland/app.o: userland/app.h userland/app.c heap.h io.h
	$(CC) $(CFLAGS) userland/app.c -o userland/app.o

kernel: $(OBJECTS)
	ld $(LDFLAGS) -o $(KERNEL) $(OBJECTS)
	@echo "✅ Zero OS v1.2 Performance+ Build: $(KERNEL)"
	@ls -lh $(KERNEL)
	@file $(KERNEL) || true
	@echo "Size: $$(du -h $(KERNEL) | cut -f1)"

clean:
	rm -f *.o $(KERNEL) zero-os.iso fs/*.o gui/*.o userland/*.o
	rm -rf iso
