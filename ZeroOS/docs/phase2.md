# Zero OS Phase 2 - Shell + Keyboard

## What we built

### keyboard.c - Polling driver
- No interrupts yet (IDT in Phase 3)
- Polls status port 0x64, data port 0x60
- Scancode set 1 US QWERTY
- Handles shift, capslock, backspace, enter
- Functions:
  - keyboard_init() - clears buffer
  - keyboard_has_key() - check status
  - keyboard_poll() - non-blocking
  - keyboard_getchar() - blocking with hlt

### shell.c - Zero Shell
- Buffer 128 bytes
- Commands:
  - help: list commands
  - clear: scroll clear
  - zero/logo: ASCII art
  - uname: version
  - echo: print
  - history: count
  - reboot: pulse 0xFE via 8042
- Handles backspace, enter
- Never returns (infinite loop)

### kernel.c improvements
- Added scroll when reach bottom
- Added backspace handling (erase char)
- Now calls shell_run() after logo

## How to test (real hardware / QEMU locally)
```bash
qemu-system-i386 -kernel zero-kernel.elf
# In QEMU window, you can type:
# help, echo hello zero, zero, uname, clear
```

## Next: Phase 3 Memory
We need:
1. GDT - for 64-bit long mode or 32-bit flat
2. IDT - 256 entries, ISR stubs for keyboard IRQ1
3. PIC remap - to avoid conflict
4. PMM - bitmap from multiboot memory map
5. Paging - 4KB pages, higher half kernel?
6. kmalloc - simple first-fit

Then we can have interrupt-driven keyboard (no polling hlt loop) + timer.

## Web simulator sync
Web terminal should match real shell commands.
