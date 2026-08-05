#ifndef PMM_H
#define PMM_H

#include "io.h"

#define PMM_BLOCK_SIZE 4096 // 4KB
#define PMM_MAX_BLOCKS 32768 // 128MB / 4KB

void pmm_init(uint32_t mem_size_kb, uint32_t bitmap_addr);
void pmm_init_region(uint32_t base, uint32_t size);
void pmm_deinit_region(uint32_t base, uint32_t size);
uint32_t* pmm_alloc_block(void);
void pmm_free_block(uint32_t* addr);
uint32_t pmm_get_free_blocks(void);
uint32_t pmm_get_total_blocks(void);

#endif
