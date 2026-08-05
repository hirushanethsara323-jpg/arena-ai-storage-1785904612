# Zero OS 🌀 v0.2 - Shell Build

> **Zero Bloat. Zero Tracking. Zero Limits.**

### Current Status: v0.2 Shell ✅

**Real Kernel:** `zero-kernel.elf` - 13K (was 8.1K in v0.1)
- Boots to VGA 80x25
- **NEW:** PS/2 Polling Keyboard Driver (`keyboard.c`)
- **NEW:** Zero Shell with commands (`shell.c`)
- **NEW:** Scrolling terminal + backspace support

```
zero@zero-os:~$ help
 Zero OS Shell v0.2 - Commands:
  help, clear, echo <text>, zero, uname, reboot, history, logo
```

### Build
```bash
cd ZeroOS
make kernel
# Output: zero-kernel.elf 13K
# Run locally: qemu-system-i386 -kernel zero-kernel.elf
```

### What's New in v0.2
- **io.h:** inb/outb low-level
- **keyboard.h/c:** 128 scancode table, shift, capslock, polling via 0x60/0x64
- **shell.h/c:** 128 byte buffer, history count, 8 commands
- **kernel.c:** Now with scroll, backspace, calls shell_run()

### Roadmap

#### ✅ Phase 1: Genesis (Done)
- Multiboot bootloader, VGA, Hello Zero

#### ✅ Phase 2: Shell (NOW - Done)
- [x] Keyboard driver (polling)
- [x] Shell + 7 commands
- [x] Scroll + backspace

#### 🔜 Phase 3: Memory (Next)
- [ ] GDT - flat 0-4GB + 64-bit
- [ ] IDT + PIC remap - interrupt driven keyboard
- [ ] Physical memory manager (bitmap at 1M)
- [ ] kmalloc/kfree heap

#### ⏳ Phase 4: ZeroFS
- [ ] RamFS, ZeroFS log-structured
- [ ] ATA PIO driver

#### 🎨 Phase 5: Zero Ring GUI (Unique)
- Circular dock, orbit apps, void browser
- Web simulator already shows concept (web/index.html)

#### 🚀 Phase 6: Userland
- ELF loader, syscalls, scheduler

### File Structure
```
ZeroOS/
├── boot.S          # Entry
├── kernel.c        # VGA + main -> shell
├── io.h            # inb/outb
├── keyboard.h/c    # PS/2 driver
├── shell.h/c       # Zero Shell
├── linker.ld
├── Makefile
├── zero-kernel.elf # 13K bootable
├── docs/design.md  # Unique design
└── web/index.html  # Web preview of Zero Ring UI
```

### Zero Design
- Background #0A0A0F, Cyan #00FFD1, Purple #8B5CF6
- No sharp corners, all zero/circle
- Zero Ring = central interaction

---
Built from zero. Push to GitHub storage: https://github.com/hirushanethsara323-jpg/arena-ai-storage-1785904612
