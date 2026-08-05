# Zero OS 🌀

> **Zero Bloat. Zero Tracking. Zero Limits.**

A real, bootable operating system from scratch - inspired by Windows/macOS but with a unique Zero identity.

### Vision
- **Name:** Zero OS
- **Tagline:** Start from Zero, Build Everything
- **Design:** Unique - Cyber-minimal, dark with neon cyan/purple accents, circular / zero-inspired UI
- **Philosophy:** Everything is a file, everything is fast, zero unnecessary layers

### Architecture Roadmap

#### 🔹 Phase 1: Genesis [NOW]
- [x] Multiboot bootloader (GAS assembly, no NASM needed)
- [x] Kernel in C (freestanding)
- [x] VGA driver - print to screen
- [x] Boot to "Hello Zero OS"
- Target: Buildable kernel.elf

#### 🔹 Phase 2: Core (Next)
- [ ] GDT (Global Descriptor Table)
- [ ] IDT + Interrupt handling
- [ ] Keyboard driver (PS/2)
- [ ] Simple shell: `zero> help, clear, echo, reboot`
- [ ] Timer (PIT)

#### 🔹 Phase 3: Memory
- [ ] Physical memory manager (bitmap)
- [ ] Virtual memory + Paging
- [ ] Heap (kmalloc / kfree)
- [ ] Zero Memory - zero-copy philosophy

#### 🔹 Phase 4: ZeroFS
- [ ] Initrd / RamFS
- [ ] Custom ZeroFS - zero overhead FS
- [ ] File descriptors

#### 🔹 Phase 5: Zero UI [UNIQUE DESIGN]
- **Concept:** Everything is circular / zero.
- Dock is a ring around screen, not bottom bar
- Windows are borderless with glow
- No icons - zero icons, just search + gestures
- Color: #0A0A0F background, #00FFD1 cyan, #8B5CF6 purple
- Font: JetBrains Mono / Space Grotesk
- Boots directly to command halo, not desktop

#### 🔹 Phase 6: Userland
- [ ] ELF loader
- [ ] Syscalls
- [ ] Processes + scheduler
- [ ] Drivers: VESA graphics, mouse, ATA disk

#### 🔹 Phase 7: Network & AI
- [ ] Network stack
- [ ] Zero AI - local assistant built into kernel shell

---

### Build (in Arena sandbox, no qemu/nasm)

We build using only gcc (Debian).

```bash
cd ZeroOS
make
```

Outputs `zero-kernel.elf` - bootable with GRUB/QEMU.

If you have local QEMU:
```bash
qemu-system-i386 -kernel zero-kernel.elf
# or with ISO:
qemu-system-i386 -cdrom zero-os.iso
```

### Project Structure
```
ZeroOS/
├── boot.S          # Multiboot header + entry (GAS)
├── kernel.c        # Kernel main + VGA driver
├── linker.ld       # Linker script
├── Makefile
├── README.md
└── docs/
    └── design.md   # Unique Zero design spec
```

### Zero Design Principles
1.  **Zero = Circle:** All UI elements circular or rounded. No sharp corners.
2.  **Zero Latency:** No animations >150ms.
3.  **Zero Clutter:** Only 3 things on screen: Halo Prompt, Zero Ring, Status.
4.  **Zero Tracking:** No telemetry, ever.

---
Built with ❤️ from zero.
