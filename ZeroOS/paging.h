#ifndef PAGING_H
#define PAGING_H

#include "io.h"

#define PAGE_SIZE 4096
#define PAGE_TABLE_ENTRIES 1024
#define PAGE_DIR_ENTRIES 1024

#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4
#define PAGE_4MB 0x80

typedef uint32_t page_dir_t[1024];
typedef uint32_t page_table_t[1024];

void paging_init(void);
void paging_enable(void);
void paging_map(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags);
void paging_unmap(uint32_t virtual_addr);
uint32_t paging_get_phys(uint32_t virtual_addr);
void paging_dump(void);

extern page_dir_t* page_directory;

#endif
