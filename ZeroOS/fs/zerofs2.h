#ifndef ZEROFS2_H
#define ZEROFS2_H

#include "../io.h"

#define ZEROFS2_MAGIC 0x5A45524F // 'ZERO'
#define ZEROFS2_VERSION 2
#define ZEROFS2_BLOCK_SIZE 4096
#define ZEROFS2_MAX_FILES 128

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t total_blocks;
    uint32_t free_blocks;
    uint32_t inode_count;
    uint32_t root_inode;
    uint32_t journal_start;
    uint32_t journal_size;
} zerofs2_superblock_t;

typedef struct {
    char name[32];
    uint32_t size;
    uint32_t blocks[12]; // direct blocks
    uint32_t indirect;
    uint8_t type;
    uint8_t used;
    uint32_t created;
    uint32_t modified;
} zerofs2_inode_t;

void zerofs2_init(void);
int zerofs2_format(void);
int zerofs2_create(const char* name);
int zerofs2_write(const char* name, const char* data, uint32_t size);
int zerofs2_read(const char* name, char* buf, uint32_t max);
int zerofs2_delete(const char* name);
void zerofs2_ls(void);
void zerofs2_sync_to_disk(void); // write to ATA

#endif
