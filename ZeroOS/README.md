# Zero OS 🌀 v0.6 - Userland Build

> **Zero Bloat. Zero Tracking. Zero Limits.**

### Current: v0.6 Userland ✅ (45K kernel)

**Real Kernel:** `zero-kernel.elf` - 45K
- All previous: VGA, KBD, Shell, GDT, IDT, PIC, PMM, Heap, VFS, FB, Mouse, Compositor
- **NEW v0.6:**
  - PIT - 100Hz timer, IRQ0, ticks, sleep
  - Tasking - 16 tasks max, round-robin, create/yield/exit, stack 8K
  - ELF - loader for 32-bit exec, check magic, load program headers
  - Syscalls - int 0x80, exit, write, zero, handler registry

```
zero@zero-os:~$ ps
 [Tasks] 2 running:
  0 - zero-kernel [RUN]
  1 - shell [READY] (future)

zero@zero-os:~$ ticks
 [PIT] Ticks: 1234 (100Hz)

zero@zero-os:~$ spawn mytask
 [TASK] Spawned mytask id=1

zero@zero-os:~$ exec program.elf
 [ELF] Loading...
```

### Build
make kernel -> 45K

### Timeline
- v0.1 Genesis 8.1K - boot
- v0.2 Shell 13K - kbd+shell
- v0.3 Memory 25K - GDT+IDT+PMM+Heap
- v0.4 ZeroFS 32K - RamFS+ATA+VFS
- v0.5 GUI 38K - FB+Mouse+Compositor Zero Ring
- v0.6 Userland 45K - PIT+Tasking+ELF+Syscalls ✅ NOW

### Next: v1.0 Polish
- Paging + higher half
- Real context switch asm
- VFS exec + process isolation
- Shell improvements + apps
- Boot to GUI by default

Repo: https://github.com/hirushanethsara323-jpg/arena-ai-storage-1785904612
Web: Zero Ring UI preview in web/index.html
