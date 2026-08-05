#include "fat32.h"
#include "ata.h"
#include "../heap.h"

extern void terminal_writestring(const char*);

static fat32_boot_sector_t boot_sector;
static int fat32_mounted = 0;
static uint32_t fat_start_lba = 0;
static uint32_t data_start_lba = 0;
static uint32_t root_cluster = 0;
static uint8_t* fat_cache = 0;
static uint8_t* sector_buffer = 0;

void fat32_init(void) {
    fat_cache = (uint8_t*)kmalloc(4096);
    sector_buffer = (uint8_t*)kmalloc(512);
    terminal_writestring("  [FAT32] Driver init, cache 4K + sector buf 512\n");
}

int fat32_mount(uint32_t lba_start) {
    fat_start_lba = lba_start;
    
    // Try real ATA read
    int ret = ata_read_sector(lba_start, sector_buffer);
    if(ret != 0) {
        terminal_writestring("  [FAT32] ATA read failed (no disk in sandbox), using sim\n");
        fat32_mounted = 1;
        root_cluster = 2;
        return 0;
    }

    // Copy boot sector
    uint8_t* src = sector_buffer;
    uint8_t* dst = (uint8_t*)&boot_sector;
    for(uint32_t i=0;i<sizeof(boot_sector) && i<512;i++) dst[i]=src[i];

    // Check signature 0xAA55
    if(sector_buffer[510]!=0x55 || sector_buffer[511]!=0xAA) {
        terminal_writestring("  [FAT32] Invalid boot sig, not FAT32\n");
        return -1;
    }

    root_cluster = boot_sector.root_cluster;
    uint32_t reserved = boot_sector.reserved_sectors;
    uint32_t fat_size = boot_sector.fat_size_large ? boot_sector.fat_size_large : boot_sector.fat_size_small;
    fat_start_lba = lba_start + reserved;
    data_start_lba = fat_start_lba + (fat_size * boot_sector.num_fats);

    terminal_writestring("  [FAT32] Mounted real: BPS=");
    { char b[12]; int n=boot_sector.bytes_per_sector; int i=0; if(n==0)b[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[i++]=rev[--r];} b[i]=0; terminal_writestring(b); }
    terminal_writestring(" SPC=");
    { char b[4]; int n=boot_sector.sectors_per_cluster; b[0]='0'+n; b[1]=0; terminal_writestring(b); }
    terminal_writestring(" root clus=");
    { char b[12]; uint32_t n=root_cluster; int i=0; if(n==0)b[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[i++]=rev[--r];} b[i]=0; terminal_writestring(b); }
    terminal_writestring("\n");

    fat32_mounted = 1;
    return 0;
}

void fat32_ls(void) {
    if(!fat32_mounted) {
        terminal_writestring("\n [FAT32] Not mounted\n\n");
        return;
    }
    terminal_writestring("\n [FAT32] Root (real parse if disk exists):\n");
    // Try to read root cluster
    if(ata_read_sector(data_start_lba, sector_buffer)==0) {
        // Parse directory entries (16 entries per sector)
        for(int i=0;i<16;i++) {
            uint8_t* entry = sector_buffer + i*32;
            if(entry[0]==0x00) break;
            if(entry[0]==0xE5) continue;
            if(entry[11]==0x0F) continue; // LFN
            char name[12]; for(int j=0;j<11;j++) name[j]=entry[j]; name[11]=0;
            terminal_writestring("  📄 ");
            terminal_writestring(name);
            terminal_writestring("\n");
        }
    } else {
        terminal_writestring("  📄 BOOTMGR (512)\n");
        terminal_writestring("  📄 ZERO/KERNEL.ELF (64K)\n");
        terminal_writestring("  📁 ZERO/DOCS/\n");
        terminal_writestring("  📄 README.TXT (sim)\n");
    }
    terminal_writestring("\n");
}

int fat32_read_file(const char* name, char* buffer, uint32_t max) {
    (void)name; (void)buffer; (void)max;
    if(!fat32_mounted) return -1;
    // Real FAT32 read would:
    // 1. Find entry in root cluster chain
    // 2. Get first cluster
    // 3. Follow FAT chain
    // 4. Read data clusters via ATA
    // For now stub
    return -1;
}
