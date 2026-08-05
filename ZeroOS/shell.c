#include "io.h"
#include "keyboard.h"
#include "shell.h"

// From kernel.c VGA functions - we will extern them
extern void terminal_writestring(const char* data);
extern void terminal_putchar(char c);
extern void terminal_initialize(void);

#define SHELL_BUFFER_SIZE 128
static char buffer[SHELL_BUFFER_SIZE];
static int buffer_pos = 0;
static int command_history_count = 0;

static void print_prompt(void) {
    terminal_writestring("zero@zero-os:~$ ");
}

static int strcmp(const char* a, const char* b) {
    while(*a && *b && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

static int strncmp(const char* a, const char* b, int n) {
    for(int i=0;i<n;i++) {
        if(a[i]!=b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
        if(a[i]==0) return 0;
    }
    return 0;
}

static void strcpy(char* dst, const char* src) {
    while((*dst++ = *src++));
}

static int strlen_shell(const char* s) {
    int i=0; while(s[i]) i++; return i;
}

static void clear_screen(void) {
    terminal_initialize();
}

extern void task_list(void);
extern int task_create(const char* name, void (*entry)(void));
extern void task_yield(void);
extern uint32_t pit_get_ticks(void);
extern void app_list(void);
extern int app_launch(const char* name);
extern void app_store(void);
extern void paging_dump(void);

static void dummy_task1(void) {
    while(1) {
        // would do work
        task_yield();
    }
}

static void cmd_help(void) {
    terminal_writestring("\n Zero OS Shell v1.0 - Commands:\n");
    terminal_writestring("  help, clear, echo, zero, uname, mem\n");
    terminal_writestring("  ls, cat, touch, rm, write - ZeroFS\n");
    terminal_writestring("  gui, ps, ticks, spawn, exec\n");
    terminal_writestring("  apps, launch <app>, store - App Store\n");
    terminal_writestring("  paging, reboot, history\n");
    terminal_writestring("\n  v1.0 STABLE: 6 apps, paging, 45K->51K kernel\n");
    terminal_writestring("  Type 'store' for App Store, 'apps' to list\n\n");
}

static void cmd_zero(void) {
    terminal_writestring("\n");
    terminal_writestring("   ____  _____ ____   ___     ___  ____\n");
    terminal_writestring("  |_  / | ____|  _ \\ / _ \\   / _ \\/ ___|\n");
    terminal_writestring("   / /  |  _| | |_) | | | | | | | \\___ \\\n");
    terminal_writestring("  / /_  | |___|  _ <| |_| | | |_| |___) |\n");
    terminal_writestring(" /____| |_____|_| \\_\\\\___/   \\___/|____/\n");
    terminal_writestring("\n  Zero Bloat. Zero Tracking. Zero Limits.\n");
    terminal_writestring("  v0.2 Genesis + Shell\n\n");
}

extern uint32_t pmm_get_free_blocks(void);
extern uint32_t pmm_get_total_blocks(void);
extern uint32_t heap_get_free(void);
extern void* kmalloc(uint32_t size);
extern void kfree(void* ptr);
extern void vfs_ls(void);
extern int vfs_create(const char* name);
extern int vfs_write_file(const char* name, const char* data);
extern int vfs_read_file(const char* name, char* buffer, uint32_t max);
extern int vfs_delete(const char* name);
extern int vfs_exists(const char* name);
extern void compositor_draw(void);

static void cmd_uname(void) {
    terminal_writestring("\n Zero OS 0.3.0 x86_64 Memory Build\n");
    terminal_writestring(" Kernel: zero-kernel.elf 13K -> now 24K+\n");
    terminal_writestring(" Build: GDT+IDT+PIC+PMM+Heap+VGA+KBD+Shell\n");
    terminal_writestring(" Ring: Active\n\n");
}

static void cmd_mem(void) {
    terminal_writestring("\n [Memory]\n");
    uint32_t total = pmm_get_total_blocks();
    uint32_t free = pmm_get_free_blocks();
    uint32_t used = total - free;
    
    terminal_writestring("  PMM Total blocks: ");
    { char buf[16]; int i=0; if(total==0) buf[i++]='0'; else { char rev[16]; int r=0; uint32_t n=total; while(n>0){ rev[r++]='0'+(n%10); n/=10;} while(r>0) buf[i++]=rev[--r]; } buf[i]=0; terminal_writestring(buf); }
    terminal_writestring("\n  PMM Free blocks: ");
    { char buf[16]; int i=0; if(free==0) buf[i++]='0'; else { char rev[16]; int r=0; uint32_t n=free; while(n>0){ rev[r++]='0'+(n%10); n/=10;} while(r>0) buf[i++]=rev[--r]; } buf[i]=0; terminal_writestring(buf); }
    terminal_writestring("\n  PMM Used blocks: ");
    { char buf[16]; int i=0; if(used==0) buf[i++]='0'; else { char rev[16]; int r=0; uint32_t n=used; while(n>0){ rev[r++]='0'+(n%10); n/=10;} while(r>0) buf[i++]=rev[--r]; } buf[i]=0; terminal_writestring(buf); }
    
    terminal_writestring("\n  Heap free: ");
    { uint32_t hf = heap_get_free(); char buf[16]; int i=0; if(hf==0) buf[i++]='0'; else { char rev[16]; int r=0; uint32_t n=hf; while(n>0){ rev[r++]='0'+(n%10); n/=10;} while(r>0) buf[i++]=rev[--r]; } buf[i]=0; terminal_writestring(buf); terminal_writestring(" bytes"); }
    
    terminal_writestring("\n  Block size: 4096 bytes\n");
    terminal_writestring("  Heap: 1MB @0x200000\n\n");

    terminal_writestring("  Testing kmalloc(64)... ");
    void* test = kmalloc(64);
    if(test) {
        terminal_writestring("OK @0x");
        uint32_t addr = (uint32_t)test;
        char hex[] = "0123456789ABCDEF";
        char out[9]; for(int i=0;i<8;i++){ out[7-i]=hex[addr & 0xF]; addr>>=4; } out[8]=0;
        terminal_writestring(out);
        terminal_writestring("\n  Freeing... ");
        kfree(test);
        terminal_writestring("OK\n\n");
    } else {
        terminal_writestring("FAIL OOM\n\n");
    }
}

static void execute_command(void) {
    buffer[buffer_pos] = 0;
    
    if(buffer_pos == 0) {
        terminal_writestring("\n");
        print_prompt();
        return;
    }

    command_history_count++;

    if(strcmp(buffer, "help") == 0) {
        cmd_help();
    } else if(strcmp(buffer, "clear") == 0) {
        clear_screen();
        cmd_zero();
    } else if(strcmp(buffer, "zero") == 0 || strcmp(buffer, "logo") == 0) {
        cmd_zero();
    } else if(strcmp(buffer, "uname") == 0 || strcmp(buffer, "uname -a") == 0) {
        cmd_uname();
    } else if(strcmp(buffer, "mem") == 0) {
        cmd_mem();
    } else if(strcmp(buffer, "ls") == 0) {
        vfs_ls();
    } else if(strcmp(buffer, "ps") == 0) {
        task_list();
    } else if(strcmp(buffer, "ticks") == 0) {
        terminal_writestring("\n [PIT] Ticks: ");
        { uint32_t t=pit_get_ticks(); char b[12]; int n=t; int i=0; if(n==0)b[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[i++]=rev[--r];} b[i]=0; terminal_writestring(b); terminal_writestring(" (100Hz)\n\n"); }
    } else if(strncmp(buffer, "spawn ", 6)==0) {
        char* name = buffer+6;
        while(*name==' ') name++;
        if(*name==0) name="dummy";
        int id = task_create(name, dummy_task1);
        if(id>=0) { terminal_writestring("\n [TASK] Spawned "); terminal_writestring(name); terminal_writestring(" id="); { char b[12]; int n=id; int i=0; if(n==0)b[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[i++]=rev[--r];} b[i]=0; terminal_writestring(b); } terminal_writestring("\n\n"); }
        else { terminal_writestring("\n [TASK] Failed, max tasks\n\n"); }
    } else if(strncmp(buffer, "exec ", 5)==0) {
        terminal_writestring("\n [ELF] Exec - needs paging enabled on real HW\n\n");
    } else if(strcmp(buffer, "apps") == 0) {
        app_list();
    } else if(strncmp(buffer, "launch ", 7)==0) {
        char* name = buffer+7;
        while(*name==' ') name++;
        app_launch(name);
    } else if(strcmp(buffer, "store") == 0) {
        app_store();
    } else if(strcmp(buffer, "paging") == 0) {
        paging_dump();
        terminal_writestring("\n");
    } else if(strcmp(buffer, "gui") == 0) {
        terminal_writestring("\n [GUI] Drawing Zero Ring compositor...\n");
        compositor_draw();
        terminal_writestring(" If in graphics mode, you'd see ring + windows.\n In text mode, shell continues.\n\n");
    } else if(strncmp(buffer, "touch ", 6) == 0) {
        char* fname = buffer+6;
        while(*fname==' ') fname++;
        if(*fname==0) {
            terminal_writestring("\n Usage: touch <filename>\n\n");
        } else {
            int ret = vfs_create(fname);
            if(ret>=0) { terminal_writestring("\n Created: "); terminal_writestring(fname); terminal_writestring("\n\n"); }
            else { terminal_writestring("\n Failed to create (exists or full)\n\n"); }
        }
    } else if(strncmp(buffer, "rm ", 3) == 0) {
        char* fname = buffer+3;
        while(*fname==' ') fname++;
        if(*fname==0) {
            terminal_writestring("\n Usage: rm <filename>\n\n");
        } else {
            int ret = vfs_delete(fname);
            if(ret==0) { terminal_writestring("\n Deleted: "); terminal_writestring(fname); terminal_writestring("\n\n"); }
            else { terminal_writestring("\n File not found\n\n"); }
        }
    } else if(strncmp(buffer, "cat ", 4) == 0) {
        char* fname = buffer+4;
        while(*fname==' ') fname++;
        if(*fname==0) {
            terminal_writestring("\n Usage: cat <filename>\n\n");
        } else {
            char buf[512];
            int r = vfs_read_file(fname, buf, 511);
            if(r>=0) {
                buf[r]=0;
                terminal_writestring("\n--- ");
                terminal_writestring(fname);
                terminal_writestring(" ---\n");
                terminal_writestring(buf);
                terminal_writestring("\n--- end ---\n\n");
            } else {
                terminal_writestring("\n File not found\n\n");
            }
        }
    } else if(strncmp(buffer, "write ", 6) == 0) {
        // write <file> <text>
        char* p = buffer+6;
        while(*p==' ') p++;
        char* fname = p;
        while(*p && *p!=' ') p++;
        if(*p==0) {
            terminal_writestring("\n Usage: write <file> <text>\n\n");
        } else {
            *p=0; p++;
            while(*p==' ') p++;
            if(*p==0) {
                terminal_writestring("\n Usage: write <file> <text>\n\n");
            } else {
                int ret = vfs_write_file(fname, p);
                if(ret>=0) { terminal_writestring("\n Wrote "); 
                    { char nb[12]; int n=ret; int i=0; if(n==0)nb[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)nb[i++]=rev[--r];} nb[i]=0; terminal_writestring(nb); }
                    terminal_writestring(" bytes to "); terminal_writestring(fname); terminal_writestring("\n\n"); }
                else { terminal_writestring("\n Write failed\n\n"); }
            }
        }
    } else if(strcmp(buffer, "reboot") == 0) {
        terminal_writestring("\n Rebooting Zero OS...\n");
        // Try reboot via 8042
        uint8_t temp;
        do {
            temp = inb(0x64);
            if((temp & 2) == 0) break;
        } while(1);
        outb(0x64, 0xFE); // pulse reset line
        // if fails, halt
        terminal_writestring(" Reboot failed, halting.\n");
        while(1) __asm__ volatile ("hlt");
    } else if(strcmp(buffer, "history") == 0) {
        terminal_writestring("\n Commands executed: ");
        // simple itoa
        char num[12]; int n=command_history_count; int i=0;
        if(n==0) num[i++]='0';
        else {
            char rev[12]; int r=0;
            while(n>0){ rev[r++]='0'+(n%10); n/=10; }
            while(r>0) num[i++]=rev[--r];
        }
        num[i]=0;
        terminal_writestring(num);
        terminal_writestring("\n\n");
    } else if(strncmp(buffer, "echo ", 5) == 0) {
        terminal_writestring("\n ");
        terminal_writestring(buffer+5);
        terminal_writestring("\n\n");
    } else if(buffer[0] != 0) {
        terminal_writestring("\n sh: command not found: ");
        terminal_writestring(buffer);
        terminal_writestring("\n Type 'help' for commands.\n\n");
    }

    buffer_pos = 0;
    print_prompt();
}

void shell_init(void) {
    buffer_pos = 0;
    command_history_count = 0;
    keyboard_init();
}

void shell_handle_input(char c) {
    if(c == '\n' || c == '\r') {
        terminal_putchar('\n');
        execute_command();
    } else if(c == '\b' || c == 127) {
        if(buffer_pos > 0) {
            buffer_pos--;
            // erase char on screen: backspace, space, backspace
            terminal_putchar('\b');
            terminal_putchar(' ');
            terminal_putchar('\b');
        }
    } else if(c >= 32 && c <= 126) {
        if(buffer_pos < SHELL_BUFFER_SIZE-1) {
            buffer[buffer_pos++] = c;
            terminal_putchar(c);
        }
    }
}

void shell_run(void) {
    shell_init();
    terminal_writestring("\n");
    terminal_writestring(" [Shell] Zero Shell initialized\n");
    terminal_writestring(" [KBD] PS/2 polling driver active\n");
    terminal_writestring("\n");
    print_prompt();

    while(1) {
        char c = keyboard_getchar();
        shell_handle_input(c);
    }
}
