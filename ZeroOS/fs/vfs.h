#ifndef VFS_H
#define VFS_H

#include "../io.h"

void vfs_init(void);
int vfs_create(const char* name);
int vfs_write_file(const char* name, const char* data);
int vfs_read_file(const char* name, char* buffer, uint32_t max);
int vfs_delete(const char* name);
void vfs_ls(void);
int vfs_exists(const char* name);

#endif
