#include "pmm.h"

// Simple bitmap PMM - Phase 3
// For now, assumes 32MB RAM starting at 1MB, bitmap at 0x10000

static uint32_t pmm_memory_size = 0;
static uint32_t pmm_used_blocks = 0;
static uint32_t pmm_max_blocks = 0;
static uint32_t* pmm_bitmap = 0;

static void pmm_set_bit(int bit) {
    pmm_bitmap[bit/32] |= (1 << (bit % 32));
}
static void pmm_unset_bit(int bit) {
    pmm_bitmap[bit/32] &= ~(1 << (bit % 32));
}
static int pmm_test_bit(int bit) {
    return pmm_bitmap[bit/32] & (1 << (bit % 32));
}

static int pmm_first_free(void) {
    for(uint32_t i=0;i<pmm_max_blocks/32;i++) {
        if(pmm_bitmap[i] != 0xFFFFFFFF) {
            for(int j=0;j<32;j++) {
                int bit = i*32 + j;
                if(!(pmm_bitmap[bit/32] & (1 << (bit%32)))) return bit;
            }
        }
    }
    return -1;
}

void pmm_init(uint32_t mem_size_kb, uint32_t bitmap_addr) {
    pmm_memory_size = mem_size_kb * 1024;
    pmm_bitmap = (uint32_t*)bitmap_addr;
    pmm_max_blocks = pmm_memory_size / PMM_BLOCK_SIZE;
    pmm_used_blocks = pmm_max_blocks;

    // clear bitmap - all used
    for(uint32_t i=0;i<pmm_max_blocks/32;i++) pmm_bitmap[i]=0xFFFFFFFF;

    // For now, we will init 16MB after 1MB as free (simulate)
    // 1MB = 256 blocks, so free from 256 to 256+4096
    pmm_init_region(0x100000, 16*1024*1024);

    // reserve first MB
    pmm_deinit_region(0x0, 0x100000);
    // reserve bitmap itself
    pmm_deinit_region(bitmap_addr, 4096);
}

void pmm_init_region(uint32_t base, uint32_t size) {
    int align = base / PMM_BLOCK_SIZE;
    int blocks = size / PMM_BLOCK_SIZE;
    for(int i=0;i<blocks;i++) {
        if(align+i < (int)pmm_max_blocks) {
            pmm_unset_bit(align+i);
            pmm_used_blocks--;
        }
    }
}

void pmm_deinit_region(uint32_t base, uint32_t size) {
    int align = base / PMM_BLOCK_SIZE;
    int blocks = size / PMM_BLOCK_SIZE;
    for(int i=0;i<blocks;i++) {
        if(align+i < (int)pmm_max_blocks) {
            if(!pmm_test_bit(align+i)) {
                pmm_set_bit(align+i);
                pmm_used_blocks++;
            }
        }
    }
}

uint32_t* pmm_alloc_block(void) {
    if(pmm_max_blocks - pmm_used_blocks <= 0) return 0;
    int frame = pmm_first_free();
    if(frame==-1) return 0;
    pmm_set_bit(frame);
    pmm_used_blocks++;
    uint32_t addr = frame * PMM_BLOCK_SIZE;
    return (uint32_t*)addr;
}

void pmm_free_block(uint32_t* addr) {
    uint32_t frame = (uint32_t)addr / PMM_BLOCK_SIZE;
    pmm_unset_bit(frame);
    pmm_used_blocks--;
}

uint32_t pmm_get_free_blocks(void) { return pmm_max_blocks - pmm_used_blocks; }
uint32_t pmm_get_total_blocks(void) { return pmm_max_blocks; }
