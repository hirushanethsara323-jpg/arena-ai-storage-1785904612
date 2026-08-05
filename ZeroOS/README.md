# Zero OS 🌀 v1.0 STABLE - Daily Driver Ready

> **Zero Bloat. Zero Tracking. Zero Limits.**
> **Start from Zero, Build Everything**

### v1.0 STABLE - 49K kernel (52K on disk)

#### What is Zero OS?
A real bootable OS from scratch, like Windows/macOS but with unique Zero identity:
- **Zero Ring** - Circular dock, not taskbar
- **ZeroFS** - 32 files, 4KB each, infinite canvas concept
- **Zero AI** - Local AI built-in (planned)

#### Bootable Kernel Features
- **Boot:** Multiboot + GRUB, 1MB load, stack 16KB
- **HAL:** GDT 5 entries, IDT 256, PIC remap 32/40, PIT 100Hz
- **Memory:** PMM bitmap 16MB free, Heap 1MB @0x200000 kmalloc/kfree, Paging 16MB identity + higher half prep
- **Drivers:** VGA 80x25 scroll, PS/2 Keyboard polling, PS/2 Mouse 3-byte, ATA PIO read/write
- **FS:** ZeroFS RamFS + VFS + ATA, default README, zero.txt, config.zero, docs/
- **GUI:** FB 1024x768x32 LFB or VGA fallback, Mouse, Compositor Zero Ring 16 wins, pixel/circle/rect primitives
- **Userland:** Tasking 16 tasks 8K stack round-robin, ELF loader 32-bit exec check, Syscalls int 0x80 exit/write/zero, App Store 6 apps
- **Shell:** 18 commands - help, clear, echo, zero, uname, mem, ls, cat, touch, rm, write, gui, ps, ticks, spawn, exec, apps, launch, store, paging, reboot, history

#### Commands
```
zero@zero-os:~$ help
zero@zero-os:~$ ls
zero@zero-os:~$ cat README
zero@zero-os:~$ mem
zero@zero-os:~$ ps
zero@zero-os:~$ ticks
zero@zero-os:~$ spawn mytask
zero@zero-os:~$ apps
zero@zero-os:~$ launch terminal
zero@zero-os:~$ store
zero@zero-os:~$ paging
zero@zero-os:~$ gui
```

#### Build
```bash
cd ZeroOS
make kernel
# 49K zero-kernel.elf
# qemu-system-i386 -kernel zero-kernel.elf  (or -cdrom zero-os.iso with grub-mkrescue)
```

#### Timeline
- v0.1 8.1K Genesis - boot + VGA
- v0.2 13K Shell - kbd
- v0.3 25K Memory - GDT+IDT+PMM+Heap
- v0.4 32K ZeroFS - RamFS+ATA+VFS
- v0.5 38K GUI - FB+Mouse+Compositor
- v0.6 45K Userland - PIT+Tasking+ELF+Syscalls
- **v1.0 49K STABLE - Paging+App Store+Polish** ✅

#### Web Preview
`web/index.html` - Full Zero Ring UI simulation, click center 0 for orbit apps, drag windows, drop to center to close. This is the unique design that real kernel will boot to when VESA available.

#### Unique Design
- Colors: #0A0A0F bg, #15151F surface, #00FFD1 cyan primary, #8B5CF6 purple secondary
- No sharp corners, all 12-24px radius or circle
- Zero Ring center, apps orbit
- Windows glow cyan when focused

#### Repo
https://github.com/hirushanethsara323-jpg/arena-ai-storage-1785904612

Built from zero with ❤️
