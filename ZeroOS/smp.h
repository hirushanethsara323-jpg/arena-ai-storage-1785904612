#ifndef SMP_H
#define SMP_H

#include "io.h"

void smp_init(void);
int smp_get_cpu_count(void);
void smp_apic_init(void);
uint32_t smp_get_lapic_id(void);

#endif
