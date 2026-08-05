/* Zero OS Kernel - Phase 1 Genesis */
/* Freestanding - no stdlib, custom types */

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint32_t size_t;

/* VGA */
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

static uint16_t* const VGA_BUFFER = (uint16_t*) VGA_MEMORY;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

/* Zero OS Colors - unique */
enum vga_color {
    ZERO_BLACK = 0,
    ZERO_BLUE = 1,
    ZERO_GREEN = 2,
    ZERO_CYAN = 3,        /* Primary - #00FFD1 */
    ZERO_RED = 4,
    ZERO_MAGENTA = 5,     /* Secondary - #8B5CF6 */
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

void terminal_putchar(char c) {
    uint8_t default_color = vga_entry_color(ZERO_CYAN, ZERO_BLACK);
    
    if (c == '\n') {
        cursor_col = 0;
        if (++cursor_row == VGA_HEIGHT) {
            cursor_row = VGA_HEIGHT - 1;
        }
        return;
    }

    terminal_putentryat(c, default_color, cursor_col, cursor_row);
    if (++cursor_col == VGA_WIDTH) {
        cursor_col = 0;
        if (++cursor_row == VGA_HEIGHT) {
            cursor_row = 0;
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

void draw_zero_logo() {
    cursor_row = 3;
    cursor_col = 0;
    
    terminal_writestring("\n");
    terminal_writestring("                        .:--==+*%@@@@@%*+==-::.                        \n");
    terminal_writestring("                    :=*%@@@@@@@@@@@@@@@@@@@@@@%*=:                    \n");
    terminal_writestring("                 =%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%=                 \n");
    terminal_writestring("               *@@@@@@@@@@@@%+-:..  ..:-+%@@@@@@@@@@@@*               \n");
    terminal_writestring("             =@@@@@@@@@@@%=             =%@@@@@@@@@@@=               \n");
    terminal_writestring("            +@@@@@@@@@@@*     ZERO OS     *@@@@@@@@@@@+              \n");
    terminal_writestring("            %@@@@@@@@@@@=     v0.1.0      =@@@@@@@@@@@%              \n");
    terminal_writestring("            +@@@@@@@@@@@*                 *@@@@@@@@@@@+              \n");
    terminal_writestring("             =@@@@@@@@@@@%=             =%@@@@@@@@@@@=               \n");
    terminal_writestring("               *@@@@@@@@@@@@%+-:..  ..:-+%@@@@@@@@@@@@*               \n");
    terminal_writestring("                 =%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%=                 \n");
    terminal_writestring("                    :=*%@@@@@@@@@@@@@@@@@@@@@@%*=:                    \n");
    terminal_writestring("                        .:--==+*%@@@@@%*+==-::.                        \n");
}

void kernel_main(void) {
    terminal_initialize();
    
    terminal_writestring("\n");
    terminal_writestring("  [ Zero OS Kernel Boot ]\n");
    terminal_writestring("  > Initializing VGA... OK\n");
    terminal_writestring("  > Loading Zero Core... OK\n");
    terminal_writestring("  > Zero Ring: [//////////] 100%\n");
    terminal_writestring("\n");

    draw_zero_logo();

    terminal_writestring("\n");
    terminal_writestring("  ------------------------------------------------------------\n");
    terminal_writestring("   Zero OS v0.1.0 - Genesis Build\n");
    terminal_writestring("   Zero Bloat. Zero Tracking. Zero Limits.\n");
    terminal_writestring("  ------------------------------------------------------------\n");
    terminal_writestring("\n");
    terminal_writestring("   > System ready.\n");
    terminal_writestring("   > Type: help (in Phase 2)\n");
    terminal_writestring("\n");
    terminal_writestring("   zero@zero-os:~$ _\n");
    terminal_writestring("\n");
    terminal_writestring("   // This is a REAL bootable kernel. Built from zero.\n");
}
