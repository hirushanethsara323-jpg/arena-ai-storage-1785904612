# Zero OS 🌀 v1.2 Performance+ Build

> **Zero Bloat. Zero Tracking. Zero Limits.**

### v1.2 Performance+ - 59K kernel (60K disk)

#### New in v1.2 Performance+
- **PCI** - Bus scan 0, vendor/device/class, find class, `pci` command lists devices
- **USB** - UHCI/OHCI/EHCI/XHCI detection via PCI class 0x0C, `usb` command
- **NET** - NE2000/E1000 NIC detection class 0x02, TCP/IP minimal stub, `net` command
- **FAT32** - Boot sector struct, mount, ls simulation, `fatls` command
- **Preemptive** - PIT IRQ0 handler calls scheduler stub every 100ms, real task switch asm ready

#### All Features Timeline
- v0.1 8.1K Genesis boot+VGA
- v0.2 13K Shell kbd
- v0.3 25K Memory GDT+IDT+PMM+Heap
- v0.4 32K ZeroFS RamFS+ATA+VFS
- v0.5 38K GUI FB+Mouse+Compositor Zero Ring
- v0.6 45K Userland PIT+Tasking+ELF+Syscalls
- v1.0 49K STABLE Paging+App Store
- v1.1 54K Performance SMP+FS2+Speaker+Context
- **v1.2 59K Performance+ PCI+USB+NET+FAT32+Preemptive** ✅ NOW

#### Commands v1.2 (28)
help, clear, echo, zero, uname, mem, ls, cat, touch, rm, write, ls2, touch2, gui, ps, ticks, spawn, exec, apps, launch, store, paging, smp, beep, pci, usb, net, fatls, reboot

#### Build & Run
make kernel -> 59K
qemu-system-i386 -kernel zero-kernel.elf -net nic,model=e1000 -usb -soundhw pcspk

#### Design
Zero Ring central, orbit apps, no taskbar, #0A0A0F bg, #00FFD1 cyan, #8B5CF6 purple, all rounded

Repo: https://github.com/hirushanethsara323-jpg/arena-ai-storage-1785904612
