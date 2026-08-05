#include "vfs.h"
#include "ramfs.h"
#include "ata.h"
#include "../heap.h"

extern void terminal_writestring(const char*);

static int vfs_initialized = 0;

void vfs_init(void) {
    if(vfs_initialized) return;
    ramfs_init();
    ata_init();
    vfs_initialized = 1;
}

int vfs_create(const char* name) {
    return ramfs_create(name, 0); // file
}

int vfs_write_file(const char* name, const char* data) {
    int fd = ramfs_find(name);
    if(fd==-1) {
        fd = ramfs_create(name, 0);
        if(fd==-1) return -1;
    }
    int len=0; while(data[len]) len++;
    return ramfs_write(fd, data, len);
}

int vfs_read_file(const char* name, char* buffer, uint32_t max) {
    int fd = ramfs_find(name);
    if(fd==-1) return -1;
    return ramfs_read(fd, buffer, max);
}

int vfs_delete(const char* name) {
    return ramfs_delete(name);
}

void vfs_ls(void) {
    ramfs_list();
}

int vfs_exists(const char* name) {
    return ramfs_find(name) != -1;
}
