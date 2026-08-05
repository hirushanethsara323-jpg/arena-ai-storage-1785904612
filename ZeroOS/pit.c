#include "pit.h"
#include "io.h"

#define PIT_CMD 0x43
#define PIT_DATA0 0x40
#define PIT_FREQ 1193180

static volatile uint32_t ticks = 0;

void pit_init(uint32_t frequency_hz) {
    uint32_t divisor = PIT_FREQ / frequency_hz;
    outb(PIT_CMD, 0x36); // channel 0, lobyte/hibyte, mode 3
    outb(PIT_DATA0, divisor & 0xFF);
    outb(PIT_DATA0, (divisor >> 8) & 0xFF);
    ticks = 0;
}

void pit_handler(void) {
    ticks++;
    // Call scheduler tick every time
}

uint32_t pit_get_ticks(void) { return ticks; }

void pit_sleep(uint32_t ms) {
    // Sleep using ticks (assume 100Hz = 10ms per tick)
    uint32_t target = ticks + (ms/10);
    while(ticks < target) {
        __asm__ volatile ("hlt");
    }
}
