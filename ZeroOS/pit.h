#ifndef PIT_H
#define PIT_H

#include "io.h"

void pit_init(uint32_t frequency_hz);
void pit_handler(void);
uint32_t pit_get_ticks(void);
void pit_sleep(uint32_t ms);

#endif
