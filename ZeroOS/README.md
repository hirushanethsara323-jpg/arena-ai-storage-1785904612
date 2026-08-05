# Zero OS 🌀 v0.5 - Zero Ring GUI Build

> **Zero Bloat. Zero Tracking. Zero Limits.**

### Current: v0.5 GUI ✅ (38K kernel, 40K on disk)

**Real Kernel:** `zero-kernel.elf` - 38K
- VGA + scroll + backspace
- PS/2 Keyboard driver (polling + interrupt ready)
- Zero Shell 13 commands
- GDT+IDT+PIC+PMM+Heap
- ZeroFS RamFS 32 files + ATA PIO
- **NEW v0.5:**
  - FB - Framebuffer 1024x768x32 LFB or VGA text fallback
  - Mouse - PS/2 3-byte packet, x/y/buttons
  - Compositor - Zero Ring + 16 windows max, backbuffers, focus, close

```
zero@zero-os:~$ ls / gui / mem
zero@zero-os:~$ touch test.zero
zero@zero-os:~$ gui  # draws compositor if graphics mode
```

### Build
make kernel -> 38K

### Done
- ✅ Phase 1: Genesis 8.1K
- ✅ Phase 2: Shell 13K
- ✅ Phase 3: Memory 25K
- ✅ Phase 4: ZeroFS 32K
- ✅ Phase 5: GUI 38K FB+Mouse+Compositor

### Next: Phase 6 Userland
- ELF loader, syscalls, scheduler, processes
- Then v1.0 daily driver

### Web Preview
web/index.html - Zero Ring UI live, now matches real kernel GUI concept. Click center 0 to open orbit.

Repo: https://github.com/hirushanethsara323-jpg/arena-ai-storage-1785904612
