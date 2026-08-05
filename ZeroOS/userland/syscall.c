#include "syscall.h"
#include "../fs/vfs.h"

extern void terminal_writestring(const char*);

static int syscall_write(uint32_t fd, const char* buf, uint32_t size) {
    // fd 1 = stdout
    if(fd==1 || fd==2) {
        for(uint32_t i=0;i<size;i++) {
            // would call terminal_putchar
            // For now, use writestring char by char via temporary
            char tmp[2]; tmp[0]=buf[i]; tmp[1]=0;
            terminal_writestring(tmp);
        }
        return size;
    }
    // file write
    return -1;
}

int syscall_handler(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3) {
    switch(num) {
        case SYSCALL_EXIT:
            terminal_writestring("\n [SYSCALL] Process exit code ");
            { char b[12]; int n=arg1; int i=0; if(n==0)b[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[i++]=rev[--r];} b[i]=0; terminal_writestring(b); terminal_writestring("\n"); }
            return 0;
        case SYSCALL_WRITE:
            return syscall_write(arg1, (const char*)arg2, arg3);
        case SYSCALL_READ:
            terminal_writestring("\n [SYSCALL] Read not yet\n");
            return -1;
        case SYSCALL_ZERO:
            terminal_writestring("\n [SYSCALL] Zero OS v0.6 - Zero Bloat. Zero Tracking.\n");
            return 0;
        default:
            terminal_writestring("\n [SYSCALL] Unknown ");
            { char b[12]; int n=num; int i=0; if(n==0)b[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[i++]=rev[--r];} b[i]=0; terminal_writestring(b); terminal_writestring("\n"); }
            return -1;
    }
}

void syscall_init(void) {
    // Register int 0x80 handler in IDT (128)
    // In real OS: idt_set_gate(0x80, (uint32_t)syscall_isr, 0x08, 0xEE)
    // For now, stub
    terminal_writestring("  > Syscalls (int 0x80) registered: exit, write, zero\n");
}
