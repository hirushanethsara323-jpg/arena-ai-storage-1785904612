#ifndef HEAP_H
#define HEAP_H

#include "io.h"

void heap_init(uint32_t heap_start, uint32_t heap_size);
void* kmalloc(uint32_t size);
void kfree(void* ptr);
uint32_t heap_get_free(void);
void heap_dump(void);

#endif
