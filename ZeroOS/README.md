# Zero OS 🌀 v0.4 - ZeroFS Build

> **Zero Bloat. Zero Tracking. Zero Limits.**

### Current: v0.4 ZeroFS ✅ (32K kernel)

**Real Kernel:** `zero-kernel.elf` - 32K
- VGA + scroll + backspace
- PS/2 Keyboard driver
- Zero Shell (12 commands)
- GDT+IDT+PIC+PMM+Heap
- **NEW v0.4:**
  - ZeroFS - RamFS 32 files max, 4KB each
  - ATA PIO driver - read/write sectors via 0x1F0
  - VFS layer
  - Default files: README, zero.txt, config.zero, docs/

```
zero@zero-os:~$ ls
 [ZeroFS] 4 files:
  📄 README (95 bytes)
  📄 zero.txt (45 bytes)
  📄 config.zero (48 bytes)
  📁 docs/

zero@zero-os:~$ cat README
Welcome to Zero OS v0.4...

zero@zero-os:~$ touch hello.zero
zero@zero-os:~$ write hello.zero Hello Zero World from v0.4!
zero@zero-os:~$ cat hello.zero
zero@zero-os:~$ rm hello.zero
```

### Build
```bash
make kernel
# 32K
# qemu-system-i386 -kernel zero-kernel.elf
```

### Done
- ✅ Phase 1: Genesis 8.1K
- ✅ Phase 2: Shell 13K
- ✅ Phase 3: Memory 25K GDT+IDT+PMM+Heap
- ✅ Phase 4: ZeroFS 32K RamFS+ATA+VFS

### Next: Phase 5 Zero Ring GUI
- VESA LFB
- Compositor + circular dock
- Web preview already shows unique design

### Repo
https://github.com/hirushanethsara323-jpg/arena-ai-storage-1785904612
