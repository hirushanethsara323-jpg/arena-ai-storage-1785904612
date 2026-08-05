/* Zero OS Kernel - v0.3 Memory Build */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint32_t size_t;
typedef int32_t int32_t;

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
    terminal_writestring("            %@@@@@@@@@@@=     v0.3.0      =@@@@@@@@@@@%              \n");
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

    terminal_writestring("  > Zero Ring: [//////////] 100%\n\n");

    draw_zero_logo();

    terminal_writestring("\n");
    terminal_writestring("  ------------------------------------------------------------\n");
    terminal_writestring("   Zero OS v0.3.0 - Memory Build\n");
    terminal_writestring("   GDT+IDT+PIC+PMM+Heap Ready\n");
    terminal_writestring("   Zero Bloat. Zero Tracking. Zero Limits.\n");
    terminal_writestring("  ------------------------------------------------------------\n");
    terminal_writestring("\n");
    terminal_writestring("   New in v0.3:\n");
    terminal_writestring("   - PMM: 16MB free, bitmap @0x10000\n");
    terminal_writestring("   - Heap: 1MB @0x200000, kmalloc/kfree\n");
    terminal_writestring("   - Type 'mem' in shell to see stats\n");
    terminal_writestring("\n");

    // enable interrupts
    __asm__ volatile ("sti");

    shell_run();

    while(1) __asm__ volatile ("hlt");
}
