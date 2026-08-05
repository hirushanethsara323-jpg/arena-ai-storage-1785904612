#include "heap.h"

// Simple first-fit heap - Phase 3
// Zero overhead philosophy: no fragmentation tracking for v0.3, bump allocator + free list

typedef struct heap_block {
    uint32_t size;
    uint8_t free;
    struct heap_block* next;
} heap_block_t;

static heap_block_t* heap_start_block = 0;
static uint32_t heap_total = 0;
static uint32_t heap_used = 0;

void heap_init(uint32_t heap_start_addr, uint32_t heap_size) {
    heap_start_block = (heap_block_t*)heap_start_addr;
    heap_start_block->size = heap_size - sizeof(heap_block_t);
    heap_start_block->free = 1;
    heap_start_block->next = 0;
    heap_total = heap_size;
    heap_used = 0;
}

void* kmalloc(uint32_t size) {
    // align to 16
    if(size % 16) size = (size + 15) & ~15;

    heap_block_t* curr = heap_start_block;
    while(curr) {
        if(curr->free && curr->size >= size) {
            // split if large enough
            if(curr->size > size + sizeof(heap_block_t) + 16) {
                heap_block_t* new_block = (heap_block_t*)((uint32_t)curr + sizeof(heap_block_t) + size);
                new_block->size = curr->size - size - sizeof(heap_block_t);
                new_block->free = 1;
                new_block->next = curr->next;
                curr->next = new_block;
                curr->size = size;
            }
            curr->free = 0;
            heap_used += curr->size + sizeof(heap_block_t);
            return (void*)((uint32_t)curr + sizeof(heap_block_t));
        }
        curr = curr->next;
    }
    return 0; // OOM
}

void kfree(void* ptr) {
    if(!ptr) return;
    heap_block_t* block = (heap_block_t*)((uint32_t)ptr - sizeof(heap_block_t));
    if(block->free) return; // double free
    block->free = 1;
    heap_used -= block->size + sizeof(heap_block_t);

    // coalesce next
    if(block->next && block->next->free) {
        block->size += block->next->size + sizeof(heap_block_t);
        block->next = block->next->next;
    }
}

uint32_t heap_get_free(void) {
    return heap_total - heap_used;
}

void heap_dump(void) {
    // For debugging via terminal_writestring if needed
}
