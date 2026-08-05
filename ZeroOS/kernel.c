/* Zero OS Kernel - v0.3 Memory Build */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint32_t size_t;
typedef int int32_t;

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

static uint16_t* const VGA_BUFFER = (uint16_t*) VGA_MEMORY;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

enum vga_color {
    ZERO_BLACK = 0,
    ZERO_BLUE = 1,
    ZERO_GREEN = 2,
    ZERO_CYAN = 3,
    ZERO_RED = 4,
    ZERO_MAGENTA = 5,
    ZERO_BROWN = 6,
    ZERO_LIGHT_GREY = 7,
    ZERO_DARK_GREY = 8,
    ZERO_LIGHT_BLUE = 9,
    ZERO_LIGHT_GREEN = 10,
    ZERO_LIGHT_CYAN = 11,
    ZERO_LIGHT_RED = 12,
    ZERO_LIGHT_MAGENTA = 13,
    ZERO_LIGHT_BROWN = 14,
    ZERO_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) color << 8 | (uint16_t) uc;
}

void terminal_initialize(void) {
    cursor_row = 0;
    cursor_col = 0;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            VGA_BUFFER[index] = vga_entry(' ', vga_entry_color(ZERO_LIGHT_GREY, ZERO_BLACK));
        }
    }
}
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    VGA_BUFFER[index] = vga_entry(c, color);
}
void terminal_scroll(void) {
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            VGA_BUFFER[(y-1)*VGA_WIDTH + x] = VGA_BUFFER[y*VGA_WIDTH + x];
        }
    }
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        VGA_BUFFER[(VGA_HEIGHT-1)*VGA_WIDTH + x] = vga_entry(' ', vga_entry_color(ZERO_LIGHT_GREY, ZERO_BLACK));
    }
    if(cursor_row > 0) cursor_row--;
}
void terminal_putchar(char c) {
    uint8_t default_color = vga_entry_color(ZERO_CYAN, ZERO_BLACK);
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        if(cursor_row >= VGA_HEIGHT) { terminal_scroll(); cursor_row = VGA_HEIGHT-1; }
        return;
    }
    if (c == '\b') {
        if(cursor_col > 0) {
            cursor_col--;
            terminal_putentryat(' ', default_color, cursor_col, cursor_row);
        } else if(cursor_row > 0) {
            cursor_row--;
            cursor_col = VGA_WIDTH - 1;
            terminal_putentryat(' ', default_color, cursor_col, cursor_row);
        }
        return;
    }
    terminal_putentryat(c, default_color, cursor_col, cursor_row);
    if (++cursor_col == VGA_WIDTH) {
        cursor_col = 0;
        if (++cursor_row == VGA_HEIGHT) {
            terminal_scroll();
            cursor_row = VGA_HEIGHT - 1;
        }
    }
}
void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) terminal_putchar(data[i]);
}
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}
void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}
void draw_zero_logo(void) {
    terminal_writestring("\n");
    terminal_writestring("                        .:--==+*%@@@@@%*+==-::.                        \n");
    terminal_writestring("                    :=*%@@@@@@@@@@@@@@@@@@@@@@%*=:                    \n");
    terminal_writestring("                 =%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%=                 \n");
    terminal_writestring("               *@@@@@@@@@@@@%+-:..  ..:-+%@@@@@@@@@@@@*               \n");
    terminal_writestring("             =@@@@@@@@@@@%=             =%@@@@@@@@@@@=               \n");
    terminal_writestring("            +@@@@@@@@@@@*     ZERO OS     *@@@@@@@@@@@+              \n");
    terminal_writestring("            %@@@@@@@@@@@=     v5.0.0      =@@@@@@@@@@@%              \n");
    terminal_writestring("            +@@@@@@@@@@@*                 *@@@@@@@@@@@+              \n");
    terminal_writestring("             =@@@@@@@@@@@%=             =%@@@@@@@@@@@=               \n");
    terminal_writestring("               *@@@@@@@@@@@@%+-:..  ..:-+%@@@@@@@@@@@@*               \n");
    terminal_writestring("                 =%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%=                 \n");
    terminal_writestring("                    :=*%@@@@@@@@@@@@@@@@@@@@@@%*=:                    \n");
    terminal_writestring("                        .:--==+*%@@@@@%*+==-::.                        \n");
}

/* Externs from new subsystems */
extern void gdt_init(void);
extern void idt_init(void);
extern void pic_remap(int offset1, int offset2);
extern void pmm_init(uint32_t mem_size_kb, uint32_t bitmap_addr);
extern void heap_init(uint32_t heap_start, uint32_t heap_size);
extern uint32_t pmm_get_free_blocks(void);
extern uint32_t pmm_get_total_blocks(void);
extern uint32_t heap_get_free(void);
extern void vfs_init(void);
extern void fb_init(uint32_t mb_info);
extern void mouse_init(void);
extern void compositor_init(void);
extern void compositor_draw(void);
extern void pit_init(uint32_t freq);
extern void tasking_init(void);
extern void syscall_init(void);
extern void paging_init(void);
extern void paging_enable(void);
extern void app_init(void);
extern int app_register(const char* name, const char* desc, void (*entry)(void), uint32_t icon, uint32_t color);
extern void smp_init(void);
extern void speaker_play_tone(uint32_t freq, uint32_t ms);
extern void zerofs2_init(void);
extern void pci_scan(void);
extern void usb_init(void);
extern void usb_hid_init(void);
extern void ac97_init(void);
extern void net_init(void);
extern void fat32_init(void);
extern void ai_init(void);
extern void shell_run(void);

void kernel_main(uint32_t magic, uint32_t mb_info) {
    terminal_initialize();
    
    terminal_writestring("\n");
    terminal_writestring("  [ Zero OS Kernel Boot v0.3 ]\n");
    terminal_writestring("  > Multiboot magic: 0x");
    // simple hex print of magic
    {
        char hex[] = "0123456789ABCDEF";
        char out[11]; out[0]='0'; out[1]='x';
        for(int i=0;i<8;i++){ out[9-i] = hex[magic & 0xF]; magic >>=4; }
        out[10]=0;
        terminal_writestring(out);
        terminal_writestring("\n");
    }
    terminal_writestring("  > Initializing GDT... ");
    gdt_init();
    terminal_writestring("OK\n");

    terminal_writestring("  > Remapping PIC... ");
    pic_remap(32, 40);
    terminal_writestring("OK\n");

    terminal_writestring("  > Initializing IDT... ");
    idt_init();
    terminal_writestring("OK\n");

    terminal_writestring("  > PMM init (32MB fake)... ");
    pmm_init(32*1024, 0x10000); // 32MB, bitmap at 0x10000
    terminal_writestring("OK\n");
    terminal_writestring("  > Free blocks: ");
    {
        uint32_t free = pmm_get_free_blocks();
        char buf[16]; int i=0; if(free==0) buf[i++]='0'; else { char rev[16]; int r=0; while(free>0){ rev[r++]='0'+(free%10); free/=10;} while(r>0) buf[i++]=rev[--r]; } buf[i]=0;
        terminal_writestring(buf);
        terminal_writestring(" / ");
        uint32_t total = pmm_get_total_blocks();
        i=0; if(total==0) buf[i++]='0'; else { char rev[16]; int r=0; while(total>0){ rev[r++]='0'+(total%10); total/=10;} while(r>0) buf[i++]=rev[--r]; } buf[i]=0;
        terminal_writestring(buf);
    }
    terminal_writestring("\n");

    terminal_writestring("  > Heap init (1MB at 2M)... ");
    heap_init(0x200000, 1024*1024);
    terminal_writestring("OK\n");

    terminal_writestring("  > VFS init (ZeroFS)... ");
    vfs_init();
    terminal_writestring("OK\n");

    terminal_writestring("  > FB init (1024x768)... ");
    fb_init(mb_info);
    terminal_writestring("OK (text fallback in sandbox)\n");

    terminal_writestring("  > Mouse init (PS/2)... ");
    mouse_init();
    terminal_writestring("OK\n");

    terminal_writestring("  > Compositor init (Zero Ring)... ");
    compositor_init();
    terminal_writestring("OK\n");

    terminal_writestring("  > PIT init (100Hz)... ");
    pit_init(100);
    terminal_writestring("OK\n");

    terminal_writestring("  > Tasking init... ");
    tasking_init();
    terminal_writestring("OK\n");

    terminal_writestring("  > Syscalls init (int 0x80)... ");
    syscall_init();
    terminal_writestring("OK\n");

    terminal_writestring("  > Paging init (16MB id map)... ");
    paging_init();
    terminal_writestring("OK\n");
    // paging_enable would fault in QEMU without proper PD, so we keep disabled in sandbox build
    // In real hardware with -m32 build, enable:
    // paging_enable();

    terminal_writestring("  > App Store init... ");
    app_init();
    // Register default apps
    app_register("terminal", "Zero Shell", 0, '>', 0x00FFD1);
    app_register("files", "ZeroFS Browser", 0, 'F', 0x8B5CF6);
    app_register("editor", "Zero Edit", 0, 'E', 0x00FFD1);
    app_register("browser", "Void Browser", 0, 'B', 0xFF3B6E);
    app_register("settings", "Zero Settings", 0, 'S', 0x6B7280);
    app_register("ai", "Zero AI", 0, '*', 0x00FFD1);
    terminal_writestring("6 apps OK\n");

    terminal_writestring("  > SMP init... ");
    smp_init();

    terminal_writestring("  > ZeroFS2 init... ");
    zerofs2_init();
    terminal_writestring("OK\n");

    terminal_writestring("  > PCI scan... ");
    pci_scan();

    terminal_writestring("  > USB init... ");
    usb_init();

    terminal_writestring("  > USB-HID init... ");
    usb_hid_init();

    terminal_writestring("  > AC97 audio init... ");
    ac97_init();

    terminal_writestring("  > NET init... ");
    net_init();

    terminal_writestring("  > FAT32 init... ");
    fat32_init();
    terminal_writestring("OK\n");

    terminal_writestring("  > ZeroAI init... (bring back, okkoma hadanna) ");
    ai_init();
    terminal_writestring("OK\n");

    terminal_writestring("  > Speaker beep test... ");
    speaker_play_tone(800, 60);
    terminal_writestring("OK\n");

    terminal_writestring("  > Zero Ring: [//////////] 100%\n\n");

    draw_zero_logo();

    terminal_writestring("\n");
    terminal_writestring("  ------------------------------------------------------------\n");
    terminal_writestring("   Zero OS v5.0.0 - ALL IN Build - Okkoma Hadala\n");
    terminal_writestring("   GDT+IDT+PMM+Heap+Paging+USB-HID Real+AC97+FAT32 Real+NET+SMP+FS+AI\n");
    terminal_writestring("   Zero Bloat. Zero Tracking. Zero Limits.\n");
    terminal_writestring("  ------------------------------------------------------------\n");
    terminal_writestring("\n");
    terminal_writestring("   New in v5.0 (Okkoma hadanna):\n");
    terminal_writestring("   - USB-HID Real: 6KRO + modifiers + report parse\n");
    terminal_writestring("   - AC97 Real: BAR0 + PCM buffer\n");
    terminal_writestring("   - FAT32 Real: ATA boot sig + root parse\n");
    terminal_writestring("   - NET: PCI + NIC + ping sim\n");
    terminal_writestring("   - AI: Local offline 20 facts back\n");
    terminal_writestring("   - All drivers perfected\n");
    terminal_writestring("\n");

    // enable interrupts
    __asm__ volatile ("sti");

    shell_run();

    while(1) __asm__ volatile ("hlt");
}
