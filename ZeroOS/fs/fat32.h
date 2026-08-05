#ifndef FAT32_H
#define FAT32_H

#include "../io.h"

typedef struct {
    uint8_t jmp[3];
    char oem[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t num_fats;
    uint16_t root_entries;
    uint16_t total_sectors_small;
    uint8_t media;
    uint16_t fat_size_small;
    uint16_t sectors_per_track;
    uint16_t num_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_large;
    uint32_t fat_size_large;
    uint16_t ext_flags;
    uint16_t fs_version;
    uint32_t root_cluster;
    uint16_t fs_info;
    uint16_t backup_boot;
    uint8_t reserved[12];
    uint8_t drive_number;
    uint8_t reserved1;
    uint8_t boot_signature;
    uint32_t volume_id;
    char volume_label[11];
    char fs_type[8];
} __attribute__((packed)) fat32_boot_sector_t;

void fat32_init(void);
int fat32_mount(uint32_t lba_start);
void fat32_ls(void);
int fat32_read_file(const char* name, char* buffer, uint32_t max);

#endif
