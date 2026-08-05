#include "paging.h"
#include "pmm.h"
#include "heap.h"

extern void terminal_writestring(const char*);

page_dir_t* page_directory = 0;
static page_table_t* page_tables[1024] = {0};

static uint32_t* paging_alloc_frame(void) {
    uint32_t* frame = pmm_alloc_block();
    if(frame) {
        // zero it
        uint32_t* ptr = frame;
        for(int i=0;i<1024;i++) ptr[i]=0;
    }
    return frame;
}

void paging_init(void) {
    // Allocate page directory
    page_directory = (page_dir_t*)pmm_alloc_block();
    if(!page_directory) {
        terminal_writestring("  [PAGING] Failed alloc dir!\n");
        return;
    }
    for(int i=0;i<1024;i++) {
        (*page_directory)[i] = 0;
        page_tables[i]=0;
    }

    // Identity map first 16MB (0x00000000 - 0x01000000)
    // Using 4MB pages for simplicity if PSE enabled, else 4KB pages
    for(uint32_t addr=0; addr<0x01000000; addr+=PAGE_SIZE) {
        paging_map(addr, addr, PAGE_PRESENT | PAGE_WRITE);
    }

    // Map kernel higher half: 0xC0000000 -> 0x00000000 (16MB) for future
    // For now, we map 0xC0100000 (kernel at 1M) to 0x100000
    // This is preparation for higher half, but we keep identity as well
    for(uint32_t i=0;i<4;i++) {
        uint32_t virt = 0xC0000000 + i*0x400000; // 0xC0000000, C0400000, etc
        uint32_t phys = i*0x400000;
        // For now, map with 4MB pages if possible
        // We'll use 4KB for simplicity in this build
    }

    terminal_writestring("  [PAGING] PD @0x");
    {
        uint32_t a=(uint32_t)page_directory;
        char hex[]="0123456789ABCDEF";
        char out[9]; for(int i=0;i<8;i++){ out[7-i]=hex[a&0xF]; a>>=4;} out[8]=0;
        terminal_writestring(out);
    }
    terminal_writestring(" 16MB identity mapped\n");
}

void paging_map(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags) {
    uint32_t pd_idx = virtual_addr >> 22;
    uint32_t pt_idx = (virtual_addr >> 12) & 0x3FF;

    if(!page_tables[pd_idx]) {
        uint32_t* table = paging_alloc_frame();
        if(!table) return;
        page_tables[pd_idx] = (page_table_t*)table;
        (*page_directory)[pd_idx] = ((uint32_t)table) | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
    }

    page_table_t* table = page_tables[pd_idx];
    (*table)[pt_idx] = (physical_addr & ~0xFFF) | (flags & 0xFFF) | PAGE_PRESENT;
}

void paging_unmap(uint32_t virtual_addr) {
    uint32_t pd_idx = virtual_addr >> 22;
    uint32_t pt_idx = (virtual_addr >> 12) & 0x3FF;
    if(!page_tables[pd_idx]) return;
    (*page_tables[pd_idx])[pt_idx] = 0;
}

uint32_t paging_get_phys(uint32_t virtual_addr) {
    uint32_t pd_idx = virtual_addr >> 22;
    uint32_t pt_idx = (virtual_addr >> 12) & 0x3FF;
    if(!page_tables[pd_idx]) return 0;
    uint32_t entry = (*page_tables[pd_idx])[pt_idx];
    if(!(entry & PAGE_PRESENT)) return 0;
    return (entry & ~0xFFF) | (virtual_addr & 0xFFF);
}

void paging_enable(void) {
    // Load page directory into CR3
    __asm__ volatile ("mov %0, %%cr3" : : "r"(page_directory));
    // Enable paging: set PG bit in CR0
    uint32_t cr0;
    __asm__ volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile ("mov %0, %%cr0" : : "r"(cr0));
    terminal_writestring("  [PAGING] Enabled (CR0.PG=1, CR3 loaded)\n");
}

void paging_dump(void) {
    terminal_writestring("\n [PAGING] Directory:\n");
    for(int i=0;i<4;i++) {
        if((*page_directory)[i]) {
            terminal_writestring("  PD[");
            { char b[4]; b[0]='0'+i; b[1]=']'; b[2]=0; terminal_writestring(b); }
            terminal_writestring(" = 0x");
            uint32_t v=(*page_directory)[i];
            char hex[]="0123456789ABCDEF";
            char out[9]; for(int j=0;j<8;j++){ out[7-j]=hex[v&0xF]; v>>=4; } out[8]=0;
            terminal_writestring(out);
            terminal_writestring("\n");
        }
    }
}
