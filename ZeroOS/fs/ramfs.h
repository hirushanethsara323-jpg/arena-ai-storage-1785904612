#ifndef RAMFS_H
#define RAMFS_H

#include "../io.h"

#define RAMFS_MAX_FILES 32
#define RAMFS_MAX_FILENAME 32
#define RAMFS_MAX_FILESIZE 4096

#define FS_FILE 0
#define FS_DIRECTORY 1

typedef struct {
    char name[RAMFS_MAX_FILENAME];
    uint32_t size;
    uint32_t capacity;
    uint8_t type;
    uint8_t used;
    uint8_t* data;
    uint32_t created_time;
    uint32_t modified_time;
} ramfs_file_t;

void ramfs_init(void);
int ramfs_create(const char* name, uint8_t type);
int ramfs_write(int fd, const char* data, uint32_t size);
int ramfs_read(int fd, char* buffer, uint32_t size);
int ramfs_delete(const char* name);
int ramfs_find(const char* name);
void ramfs_list(void);
int ramfs_get_count(void);
ramfs_file_t* ramfs_get_file(int idx);

#endif
