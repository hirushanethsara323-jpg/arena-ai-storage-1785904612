#ifndef KEYBOARD_H
#define KEYBOARD_H

// Polling keyboard driver - Phase 2
// Reads from PS/2 port 0x60 without interrupts (for simplicity)

void keyboard_init(void);
char keyboard_poll(void);        // non-blocking, 0 if no key
char keyboard_getchar(void);     // blocking
int keyboard_has_key(void);

#endif
