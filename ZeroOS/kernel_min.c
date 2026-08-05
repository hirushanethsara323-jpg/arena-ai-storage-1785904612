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
static uint8_t cursor_row=0, cursor_col=0;
enum vga_color {ZERO_BLACK=0,ZERO_LIGHT_GREY=7,ZERO_CYAN=3};
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){return fg|bg<<4;}
static inline uint16_t vga_entry(unsigned char uc, uint8_t color){return (uint16_t)color<<8|(uint16_t)uc;}
void terminal_initialize(void){cursor_row=0;cursor_col=0;for(size_t y=0;y<VGA_HEIGHT;y++) for(size_t x=0;x<VGA_WIDTH;x++) VGA_BUFFER[y*VGA_WIDTH+x]=vga_entry(' ',vga_entry_color(ZERO_LIGHT_GREY,ZERO_BLACK));}
void terminal_putentryat(char c,uint8_t color,size_t x,size_t y){VGA_BUFFER[y*VGA_WIDTH+x]=vga_entry(c,color);}
void terminal_putchar(char c){uint8_t def=vga_entry_color(ZERO_CYAN,ZERO_BLACK); if(c=='\n'){cursor_col=0;cursor_row++;if(cursor_row>=VGA_HEIGHT)cursor_row=VGA_HEIGHT-1;return;} terminal_putentryat(c,def,cursor_col,cursor_row); if(++cursor_col==VGA_WIDTH){cursor_col=0;cursor_row++;}}
void terminal_write(const char* d,size_t s){for(size_t i=0;i<s;i++)terminal_putchar(d[i]);}
size_t strlen(const char* s){size_t l=0;while(s[l])l++;return l;}
void serial_writestring(const char* s);
void terminal_writestring(const char* d){terminal_write(d,strlen(d)); serial_writestring(d);}
extern void serial_init(void);
extern void gdt_init(void);
extern void idt_init(void);
extern void pic_remap(int,int);
extern void pmm_init(uint32_t,uint32_t);
extern void heap_init(uint32_t,uint32_t);
extern void vfs_init(void);
extern void pit_init(uint32_t);
extern void tasking_init(void);
extern void shell_run(void);
void kernel_main(uint32_t magic, uint32_t mb_info){
    serial_init(); terminal_initialize();
    terminal_writestring("\n[Zero OS MINIMAL TEST] Booting...\n");
    gdt_init(); terminal_writestring("GDT OK\n");
    pic_remap(32,40); terminal_writestring("PIC OK\n");
    idt_init(); terminal_writestring("IDT OK\n");
    pmm_init(32*1024,0x10000); terminal_writestring("PMM OK\n");
    heap_init(0x200000,1024*1024); terminal_writestring("Heap OK\n");
    vfs_init(); terminal_writestring("VFS OK\n");
    pit_init(100); terminal_writestring("PIT OK\n");
    tasking_init(); terminal_writestring("Tasking OK\n");
    terminal_writestring("Going to shell...\n");
    __asm__ volatile ("sti");
    shell_run();
    while(1) __asm__ volatile ("hlt");
}
