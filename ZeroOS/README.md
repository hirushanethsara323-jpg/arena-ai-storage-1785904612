# Zero OS 🌀 v1.1 Performance Build

> **Zero Bloat. Zero Tracking. Zero Limits.**

### v1.1 Performance - 54K kernel (56K disk)

#### New in v1.1 Performance

**Performance + Drivers focus:**
- **Paging** - Improved PD, 16MB identity map, PD dump, enable stub for 64-bit sandbox + real 32-bit boot_real.S
- **Context Switch** - Real asm `context.S` with pusha/popa + 64-bit version, `context_switch()` + `context_switch_first()`
- **SMP** - CPUID HTT, LAPIC detection @0xFEE00000, cpu count, `smp` command
- **Speaker** - PC speaker 0x61 + PIT ch2, `speaker_beep(freq)`, `beep <freq> <ms>` command
- **ZeroFS2** - 128 inodes, journal 4 blocks, superblock magic 'ZERO' v2, direct blocks 12 + indirect, ATA sector 0 sync, `ls2/touch2`

#### All Features (v0.1 - v1.1)
- Boot: Multiboot, GDT5, IDT256, PIC remap, PIT 100Hz
- Memory: PMM bitmap 16MB, Heap 1MB kmalloc/kfree, Paging 16MB + higher half prep
- Drivers: VGA scroll, KBD PS/2, Mouse PS/2 3-byte, ATA PIO, Speaker
- FS: ZeroFS v1 32 files + ZeroFS2 v2 128 inodes journal + VFS + ATA
- GUI: FB 1024x768 LFB, Compositor Zero Ring 16wins, circle/rect/pixel
- Userland: Tasking 16tasks 8K stack round-robin, ELF loader, Syscalls int 0x80, App Store 6 apps, Context switch
- SMP: CPU count, APIC

#### Commands v1.1 (23)
```
help, clear, echo, zero, uname, mem, ls, cat, touch, rm, write,
ls2, touch2, gui, ps, ticks, spawn, exec, apps, launch, store, paging, smp, beep, reboot
```

#### Build
```bash
make kernel -> 54K
qemu-system-i386 -kernel zero-kernel.elf
```

#### Timeline
- v0.1 8.1K Genesis
- v0.2 13K Shell
- v0.3 25K Memory
- v0.4 32K ZeroFS
- v0.5 38K GUI
- v0.6 45K Userland
- v1.0 49K STABLE
- **v1.1 54K Performance** ✅ NOW - SMP+FS2+Speaker+Context

#### Next v1.2
- USB UHCI driver
- Audio AC97
- FAT32 driver
- Network NE2000 + TCP/IP stub
- Real preemptive scheduler with PIT IRQ0 calling task_schedule

Repo: https://github.com/hirushanethsara323-jpg/arena-ai-storage-1785904612
