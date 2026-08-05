#include "paging.h"
#include "pmm.h"
#include "heap.h"

extern void terminal_writestring(const char*);

page_dir_t* page_directory = 0;
static page_table_t* page_tables[1024] = {0};

static uint32_t* paging_alloc_frame(void) {
    uint32_t* frame = pmm_alloc_block();
    if(frame) {
        uint32_t* ptr = frame;
        for(int i=0;i<1024;i++) ptr[i]=0;
    }
    return frame;
}

void paging_init(void) {
    page_directory = (page_dir_t*)pmm_alloc_block();
    if(!page_directory) {
        terminal_writestring("  [PAGING] Failed alloc dir!\n");
        return;
    }
    for(int i=0;i<1024;i++) {
        (*page_directory)[i] = 0;
        page_tables[i]=0;
    }

    for(uint32_t addr=0; addr<0x01000000; addr+=PAGE_SIZE) {
        paging_map(addr, addr, PAGE_PRESENT | PAGE_WRITE);
    }

    terminal_writestring("  [PAGING] PD @0x");
    {
        uint64_t a=(uint64_t)page_directory;
        char hex[]="0123456789ABCDEF";
        char out[17]; for(int i=0;i<8;i++){ out[7-i]=hex[a&0xF]; a>>=4; } out[8]=0;
        terminal_writestring(out);
    }
    terminal_writestring(" 16MB identity\n");
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
    // Real enable - now 64-bit compatible
    // In sandbox, we skip actual CR3 load to avoid faulting, but show what would happen
    // For real 32-bit build with boot_real.S, this will work
    terminal_writestring("  [PAGING] Enable: CR3=PD, CR0.PG=1 (simulated in 64-bit sandbox)\n");
    // Uncomment for real HW 32-bit build:
    // __asm__ volatile ("movl %0, %%cr3" : : "r"(page_directory));
    // uint32_t cr0; __asm__ volatile ("movl %%cr0, %0" : "=r"(cr0)); cr0|=0x80000000; __asm__ volatile ("movl %0, %%cr0" : : "r"(cr0));
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
