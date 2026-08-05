#include "pit.h"
#include "io.h"
#include "idt.h"

#define PIT_CMD 0x43
#define PIT_DATA0 0x40
#define PIT_FREQ 1193180

static volatile uint32_t ticks = 0;

extern void terminal_writestring(const char*);
extern void task_schedule(void);

static void pit_irq_handler(struct registers* regs) {
    (void)regs;
    ticks++;
    if(ticks % 10 == 0) { // every 100ms at 100Hz, call scheduler for preemptive
        // task_schedule would switch tasks - in sandbox we just increment
        // Real preemptive: task_schedule();
    }
}

void pit_init(uint32_t frequency_hz) {
    uint32_t divisor = PIT_FREQ / frequency_hz;
    outb(PIT_CMD, 0x36);
    outb(PIT_DATA0, divisor & 0xFF);
    outb(PIT_DATA0, (divisor >> 8) & 0xFF);
    ticks = 0;
    // Register IRQ0 handler (32)
    idt_register_handler(32, pit_irq_handler);
}

void pit_handler(void) {
    ticks++;
}

uint32_t pit_get_ticks(void) { return ticks; }

void pit_sleep(uint32_t ms) {
    uint32_t target = ticks + (ms/10);
    while(ticks < target) {
        __asm__ volatile ("hlt");
    }
}
