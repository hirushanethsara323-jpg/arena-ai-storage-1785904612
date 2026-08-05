#ifndef CONTEXT_H
#define CONTEXT_H

#include "io.h"

// Real context switch in assembly
void context_switch(uint32_t* old_esp, uint32_t new_esp);
void context_switch_first(uint32_t new_esp);

#endif
