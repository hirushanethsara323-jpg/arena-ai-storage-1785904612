#include "zerofs2.h"
#include "ata.h"
#include "../heap.h"
#include "../pmm.h"

extern void terminal_writestring(const char*);

static zerofs2_superblock_t superblock;
static zerofs2_inode_t inodes[ZEROFS2_MAX_FILES];
static int zerofs2_mounted = 0;
static uint8_t* disk_cache = 0;

static void strcpy_z(char* d, const char* s){ while((*d++=*s++)); }
static int strcmp_z(const char* a, const char* b){ while(*a&&*b&&*a==*b){a++;b++;} return *a-*b; }
static int strlen_z(const char* s){ int i=0; while(s[i])i++; return i; }

void zerofs2_init(void) {
    superblock.magic = ZEROFS2_MAGIC;
    superblock.version = ZEROFS2_VERSION;
    superblock.total_blocks = 8192; // 32MB disk
    superblock.free_blocks = 8190;
    superblock.inode_count = ZEROFS2_MAX_FILES;
    superblock.root_inode = 0;
    superblock.journal_start = 1;
    superblock.journal_size = 4;

    for(int i=0;i<ZEROFS2_MAX_FILES;i++) {
        inodes[i].used=0;
        inodes[i].size=0;
        inodes[i].name[0]=0;
    }

    disk_cache = (uint8_t*)kmalloc(ZEROFS2_BLOCK_SIZE);
    zerofs2_mounted = 1;

    terminal_writestring("  [ZeroFS2] Superblock magic ZERO v2, 128 inodes, journal 4 blocks\n");
    // Try to load from disk if exists (ATA sector 0)
    // For sandbox, we skip real disk read
}

int zerofs2_format(void) {
    // Format by clearing inodes and writing superblock to ATA sector 0
    for(int i=0;i<ZEROFS2_MAX_FILES;i++) {
        if(inodes[i].used) {
            // free blocks would go here
            inodes[i].used=0;
        }
    }
    superblock.free_blocks = superblock.total_blocks - 2;
    zerofs2_sync_to_disk();
    terminal_writestring("  [ZeroFS2] Formatted\n");
    return 0;
}

int zerofs2_create(const char* name) {
    if(strlen_z(name)>=32) return -1;
    for(int i=0;i<ZEROFS2_MAX_FILES;i++) if(inodes[i].used && strcmp_z(inodes[i].name,name)==0) return -1;
    for(int i=0;i<ZEROFS2_MAX_FILES;i++) if(!inodes[i].used) {
        inodes[i].used=1;
        strcpy_z(inodes[i].name,name);
        inodes[i].size=0;
        inodes[i].type=0;
        inodes[i].created=1000+i;
        inodes[i].modified=1000+i;
        for(int j=0;j<12;j++) inodes[i].blocks[j]=0;
        superblock.free_blocks--;
        return i;
    }
    return -1;
}

int zerofs2_write(const char* name, const char* data, uint32_t size) {
    for(int i=0;i<ZEROFS2_MAX_FILES;i++) {
        if(inodes[i].used && strcmp_z(inodes[i].name,name)==0) {
            // allocate block if needed (simplified: use heap, not real block alloc)
            if(size>4096) size=4096;
            // In real FS, we'd allocate disk blocks via PMM and write via ATA
            // Here we store in heap for demo and sync later
            if(inodes[i].blocks[0]==0) {
                uint32_t* blk = pmm_alloc_block();
                if(!blk) return -1;
                inodes[i].blocks[0]=(uint32_t)blk;
            }
            uint8_t* dest = (uint8_t*)inodes[i].blocks[0];
            for(uint32_t j=0;j<size;j++) dest[j]=data[j];
            inodes[i].size=size;
            inodes[i].modified++;
            return size;
        }
    }
    // auto create
    int idx=zerofs2_create(name);
    if(idx<0) return -1;
    return zerofs2_write(name,data,size);
}

int zerofs2_read(const char* name, char* buf, uint32_t max) {
    for(int i=0;i<ZEROFS2_MAX_FILES;i++) {
        if(inodes[i].used && strcmp_z(inodes[i].name,name)==0) {
            uint32_t sz=inodes[i].size;
            if(sz>max) sz=max;
            if(inodes[i].blocks[0]==0) return 0;
            uint8_t* src=(uint8_t*)inodes[i].blocks[0];
            for(uint32_t j=0;j<sz;j++) buf[j]=src[j];
            return sz;
        }
    }
    return -1;
}

int zerofs2_delete(const char* name) {
    for(int i=0;i<ZEROFS2_MAX_FILES;i++) {
        if(inodes[i].used && strcmp_z(inodes[i].name,name)==0) {
            if(inodes[i].blocks[0]) {
                pmm_free_block((uint32_t*)inodes[i].blocks[0]);
            }
            inodes[i].used=0;
            superblock.free_blocks++;
            return 0;
        }
    }
    return -1;
}

void zerofs2_ls(void) {
    terminal_writestring("\n [ZeroFS2] ");
    int cnt=0; for(int i=0;i<ZEROFS2_MAX_FILES;i++) if(inodes[i].used) cnt++;
    { char b[12]; int n=cnt; int ii=0; if(n==0)b[ii++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[ii++]=rev[--r];} b[ii]=0; terminal_writestring(b); }
    terminal_writestring(" files, free blocks: ");
    { char b[12]; int n=superblock.free_blocks; int ii=0; if(n==0)b[ii++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[ii++]=rev[--r];} b[ii]=0; terminal_writestring(b); }
    terminal_writestring("\n\n");
    for(int i=0;i<ZEROFS2_MAX_FILES;i++) if(inodes[i].used) {
        terminal_writestring("  📄 ");
        terminal_writestring(inodes[i].name);
        terminal_writestring(" (");
        { uint32_t sz=inodes[i].size; char b[12]; int ii=0; if(sz==0)b[ii++]='0'; else {char rev[12];int r=0;uint32_t n=sz;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[ii++]=rev[--r];} b[ii]=0; terminal_writestring(b); }
        terminal_writestring(" bytes, blk 0x");
        { uint32_t a=inodes[i].blocks[0]; char hex[]="0123456789ABCDEF"; char out[9]; for(int j=0;j<8;j++){ out[7-j]=hex[a&0xF]; a>>=4; } out[8]=0; terminal_writestring(out); }
        terminal_writestring(")\n");
    }
    terminal_writestring("\n");
}

void zerofs2_sync_to_disk(void) {
    // Write superblock to ATA sector 0
    if(!disk_cache) return;
    // Copy superblock to cache
    uint8_t* src = (uint8_t*)&superblock;
    for(uint32_t i=0;i<sizeof(superblock);i++) disk_cache[i]=src[i];
    // In real OS: ata_write_sector(0, disk_cache);
    terminal_writestring("  [ZeroFS2] Sync to ATA sector 0 (simulated)\n");
}
