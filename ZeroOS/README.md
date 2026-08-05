# Zero OS 🌀 v0.3 - Memory Build

> **Zero Bloat. Zero Tracking. Zero Limits.**

### Current: v0.3 Memory ✅ (25K kernel)

**Real Kernel:** `zero-kernel.elf` - 25K (v0.2 was 13K, v0.1 8.1K)
- Boots at 1MB
- VGA 80x25 + scroll
- PS/2 Keyboard driver
- Zero Shell with 8 commands
- **NEW v0.3:**
  - GDT - 5 entries flat (null, code, data, user code, user data)
  - IDT - 256 entries, ISRs 0-31 + IRQs 0-15
  - PIC Remap - 32/40 to avoid CPU exceptions
  - PMM - Bitmap allocator, 16MB free @1MB, 4KB blocks, 8192 blocks total
  - Heap - 1MB @0x200000, kmalloc/kfree first-fit, 16-byte aligned

```
zero@zero-os:~$ mem
 [Memory]
  PMM Total blocks: 8192
  PMM Free blocks: ~4096
  PMM Used blocks: ~4096
  Heap free: 1048576 bytes
  Block size: 4096 bytes
  Testing kmalloc(64)... OK @0x200010
```

### Build
```bash
cd ZeroOS
make kernel
# Output: zero-kernel.elf 25K
# Local: qemu-system-i386 -kernel zero-kernel.elf
```

### Phase 1-3 Done
- ✅ Phase 1: Genesis - boot + VGA
- ✅ Phase 2: Shell + KBD
- ✅ Phase 3: Memory - GDT+IDT+PIC+PMM+Heap

### Next: Phase 4 ZeroFS
- ATA PIO driver
- RamFS / ZeroFS
- ls, cat, touch, rm
- ELF loader prep

### Files
```
boot.S         # Minimal 64-bit sandbox, boot_real.S = full 32-bit version
boot_real.S    # Full GDT flush ljmp + pusha/popa for i686-elf-gcc
kernel.c       # Now calls gdt_init, pic_remap, idt_init, pmm_init, heap_init
gdt.h/c        # 5 entries
idt.h/c        # 256 entries + handler registry
pic.h/c        # Remap 0x20/0xA0
pmm.h/c        # Bitmap PMM
heap.h/c       # kmalloc/kfree
io.h           # inb/outb
keyboard.h/c
shell.h/c      # + mem command
```

### Web Preview
`web/index.html` - Zero Ring UI, unique design, now shows v0.3

---
Repo: https://github.com/hirushanethsara323-jpg/arena-ai-storage-1785904612
