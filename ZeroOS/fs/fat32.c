#include "fat32.h"
#include "ata.h"
#include "../heap.h"

extern void terminal_writestring(const char*);

static fat32_boot_sector_t boot_sector;
static int fat32_mounted = 0;
static uint32_t fat_start_lba = 0;
static uint8_t* fat_cache = 0;

void fat32_init(void) {
    fat_cache = (uint8_t*)kmalloc(4096);
    terminal_writestring("  [FAT32] Driver init, cache 4K allocated\n");
}

int fat32_mount(uint32_t lba_start) {
    // Read boot sector from ATA
    // In real OS: ata_read_sector(lba_start, fat_cache)
    // For sandbox, simulate
    // Check signature
    // memcpy boot sector
    // For now, stub
    fat_start_lba = lba_start;
    fat32_mounted = 1;
    terminal_writestring("  [FAT32] Mounted at LBA ");
    { char b[12]; int n=lba_start; int i=0; if(n==0)b[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[i++]=rev[--r];} b[i]=0; terminal_writestring(b); }
    terminal_writestring(" (simulated)\n");
    return 0;
}

void fat32_ls(void) {
    if(!fat32_mounted) {
        terminal_writestring("\n [FAT32] Not mounted, mount first\n\n");
        return;
    }
    terminal_writestring("\n [FAT32] Root directory (simulated):\n");
    terminal_writestring("  📄 BOOTMGR (512 bytes)\n");
    terminal_writestring("  📄 ZERO/KERNEL.ELF (54K)\n");
    terminal_writestring("  📁 ZERO/DOCS/\n");
    terminal_writestring("  📄 README.TXT\n\n");
}

int fat32_read_file(const char* name, char* buffer, uint32_t max) {
    (void)name; (void)buffer; (void)max;
    if(!fat32_mounted) return -1;
    // stub
    return -1;
}
