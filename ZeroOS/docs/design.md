# Zero OS — Unique Design Spec 🌀

## Core Identity
**Name: Zero OS**
Why Zero?
- Zero is circle, perfect, infinite
- Start from Zero (clean slate)
- Zero bloat, zero ads, zero tracking
- In math, zero is both nothing and everything

## Visual Language

### Colors
- Background: #0A0A0F (almost black, slightly blue)
- Surface: #15151F
- Primary: #00FFD1 (Zero Cyan - electric)
- Secondary: #8B5CF6 (Zero Purple)
- Accent: #FF3B6E (Error / energy)
- Text: #E8E8EC

### Typography
- Mono: JetBrains Mono for terminal / code
- Sans: Space Grotesk / Outfit for UI

### Shapes
- NO sharp 90degree corners. All 12-24px radius.
- Primary shape is circle / pill
- Windows are not rectangles - they are floating orbs with soft glow

### Zero Ring — The Main Interaction
Instead of Taskbar/Dock:

Imagine a thin glowing ring in center of screen when idle.
- Click center -> App Launcher (circular menu, like radial)
- Drag from ring -> Create new window
- Ring pulses with CPU usage

**Apps orbit the ring.**

### Window System (Phase 5)
- No title bar. Title is inside glow.
- Windows breathe (subtle scale 0.99-1.01)
- Close: swipe to center of ring (suck into zero)
- Focus: window glows cyan, others dim purple

### Boot Screen
Black screen -> single white dot appears in center -> expands to circle -> circle fills with Zero OS logo ASCII (from kernel) -> ring animation -> desktop fades in.

Sound: Low sine wave that rises (zero to one).

### File Manager — Zero Files
No folders icon. Instead:
- Files are dots on infinite canvas
- Proximity = related
- Search is primary, browse secondary
- Everything is tag + time based, not folder based

### Terminal — Zero Shell (zsh)
Prompt:
```
zero@zero:~$  ◍
```
◍ is cursor, pulsing.

Commands:
- `zero help`
- `zero apps`
- `zero files`
- `zero ai "do something"`

---

## Feature Ideas (You suggested "tell me")

### Must Have for v1
1. **Zero Shell** with AI built-in (local tiny LLM)
2. **Orbit Launcher** — circular app launcher
3. **Void Browser** — minimal browser, no tabs, just stack
4. **Note Dot** — notes are dots you drop anywhere
5. **Zero Search** — spotlight but launches everything
6. **Privacy First** — everything encrypted with zero-knowledge

### Wild Ideas
- **Zero Time:** No clock on screen. Time is shown only when you search "time"
- **Zero Desktop:** Desktop is empty, always. Files live in Zero Files canvas
- **Breathing UI:** UI elements move subtly based on CPU, like living
- **Sound Reactive:** UI glow reacts to microphone (optional)

---

## Tech Stack (Real OS)

- Boot: Multiboot1 + GRUB
- Kernel: C (later Rust for drivers)
- Graphics: Start VGA text, then VESA LFB, then custom compositor
- FS: ZeroFS (log-structured, crypto by default)
- Driver model: Message passing, like microkernel but monolithic for speed

## Release Plan

v0.1 Genesis (now) — boots, prints
v0.2 Input — keyboard + shell
v0.3 Memory — malloc + paging
v0.4 Storage — ramfs + disk
v0.5 Halo — first graphical ring UI
v0.6 Alpha — apps can run
v1.0 Zero — usable daily driver for devs
