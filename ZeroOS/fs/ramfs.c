#include "ramfs.h"
#include "../heap.h"

extern void terminal_writestring(const char* data);

static ramfs_file_t files[RAMFS_MAX_FILES];
static int file_count = 0;
static uint32_t mock_time = 0;

static int strcmp_fs(const char* a, const char* b) {
    while(*a && *b && *a==*b){a++;b++;} return *a-*b;
}
static void strcpy_fs(char* d, const char* s){ while((*d++=*s++)); }
static int strlen_fs(const char* s){ int i=0; while(s[i])i++; return i; }

void ramfs_init(void) {
    for(int i=0;i<RAMFS_MAX_FILES;i++) {
        files[i].used = 0;
        files[i].size = 0;
        files[i].capacity = 0;
        files[i].data = 0;
        files[i].name[0]=0;
    }
    file_count = 0;
    mock_time = 1000;

    // Create default files - Zero OS welcome
    ramfs_create("README", FS_FILE);
    int fd = ramfs_find("README");
    ramfs_write(fd, "Welcome to Zero OS v0.4 - ZeroFS\nZero Bloat. Zero Tracking.\n\nFiles:\n- README\n- zero.txt\n- config.zero\n", 95);

    ramfs_create("zero.txt", FS_FILE);
    fd = ramfs_find("zero.txt");
    ramfs_write(fd, "Zero OS v0.4\nStart from Zero, Build Everything\n", 45);

    ramfs_create("config.zero", FS_FILE);
    fd = ramfs_find("config.zero");
    ramfs_write(fd, "theme=zero-dark\nring=active\ncolor=cyan\nversion=0.4\n", 48);

    ramfs_create("docs", FS_DIRECTORY);
}

int ramfs_find(const char* name) {
    for(int i=0;i<RAMFS_MAX_FILES;i++) {
        if(files[i].used && strcmp_fs(files[i].name, name)==0) return i;
    }
    return -1;
}

int ramfs_create(const char* name, uint8_t type) {
    if(strlen_fs(name) >= RAMFS_MAX_FILENAME) return -1;
    if(ramfs_find(name) != -1) return -1; // exists
    for(int i=0;i<RAMFS_MAX_FILES;i++) {
        if(!files[i].used) {
            files[i].used = 1;
            strcpy_fs(files[i].name, name);
            files[i].type = type;
            files[i].size = 0;
            files[i].created_time = mock_time++;
            files[i].modified_time = mock_time;
            if(type==FS_FILE) {
                files[i].capacity = RAMFS_MAX_FILESIZE;
                files[i].data = (uint8_t*)kmalloc(RAMFS_MAX_FILESIZE);
                if(!files[i].data) { files[i].used=0; return -1; }
                for(int j=0;j<RAMFS_MAX_FILESIZE;j++) files[i].data[j]=0;
            } else {
                files[i].data = 0;
                files[i].capacity = 0;
            }
            file_count++;
            return i;
        }
    }
    return -1;
}

int ramfs_write(int fd, const char* data, uint32_t size) {
    if(fd<0||fd>=RAMFS_MAX_FILES) return -1;
    if(!files[fd].used) return -1;
    if(files[fd].type!=FS_FILE) return -1;
    if(size > files[fd].capacity) size = files[fd].capacity;
    for(uint32_t i=0;i<size;i++) files[fd].data[i]=data[i];
    files[fd].size = size;
    files[fd].modified_time = mock_time++;
    return size;
}

int ramfs_read(int fd, char* buffer, uint32_t size) {
    if(fd<0||fd>=RAMFS_MAX_FILES) return -1;
    if(!files[fd].used) return -1;
    if(size > files[fd].size) size = files[fd].size;
    for(uint32_t i=0;i<size;i++) buffer[i]=files[fd].data[i];
    return size;
}

int ramfs_delete(const char* name) {
    int idx = ramfs_find(name);
    if(idx==-1) return -1;
    if(files[idx].data) {
        kfree(files[idx].data);
    }
    files[idx].used = 0;
    files[idx].size = 0;
    files[idx].data = 0;
    file_count--;
    return 0;
}

void ramfs_list(void) {
    terminal_writestring("\n [ZeroFS] ");
    { char buf[12]; int n=file_count; int i=0; if(n==0)buf[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)buf[i++]=rev[--r];} buf[i]=0; terminal_writestring(buf); }
    terminal_writestring(" files:\n\n");
    for(int i=0;i<RAMFS_MAX_FILES;i++) {
        if(files[i].used) {
            if(files[i].type==FS_DIRECTORY) {
                terminal_writestring("  📁 ");
                terminal_writestring(files[i].name);
                terminal_writestring("/\n");
            } else {
                terminal_writestring("  📄 ");
                terminal_writestring(files[i].name);
                terminal_writestring(" (");
                { uint32_t sz=files[i].size; char buf[12]; int ii=0; if(sz==0)buf[ii++]='0'; else {char rev[12];int r=0;uint32_t n=sz;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)buf[ii++]=rev[--r];} buf[ii]=0; terminal_writestring(buf); }
                terminal_writestring(" bytes)\n");
            }
        }
    }
    terminal_writestring("\n");
}

int ramfs_get_count(void) { return file_count; }
ramfs_file_t* ramfs_get_file(int idx) {
    if(idx<0||idx>=RAMFS_MAX_FILES) return 0;
    if(!files[idx].used) return 0;
    return &files[idx];
}
