#include "fb.h"

framebuffer_t fb;

uint32_t fb_color(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 16) | (g << 8) | b;
}

void fb_init(uint32_t mb_info) {
    fb.is_available = 0;
    fb.is_text_mode = 1;
    fb.width = 80;
    fb.height = 25;
    fb.address = (uint32_t*)0xB8000;
    fb.pitch = 0;
    fb.bpp = 16;

    extern void terminal_writestring(const char*);
    terminal_writestring("  [FB] mb_info=0x");
    {
        uint32_t a=mb_info;
        char hex[]="0123456789ABCDEF";
        char out[9]; for(int i=0;i<8;i++){ out[7-i]=hex[a&0xF]; a>>=4; } out[8]=0;
        terminal_writestring(out);
        terminal_writestring(" flags=0x");
        uint32_t flags = mb_info ? *(uint32_t*)mb_info : 0;
        a=flags;
        for(int i=0;i<8;i++){ out[7-i]=hex[a&0xF]; a>>=4; }
        terminal_writestring(out);
        terminal_writestring("\n");
        if(mb_info) {
            terminal_writestring("  [FB] flags bit12 framebuffer? ");
            terminal_writestring((flags & (1<<12)) ? "YES\n" : "NO, text mode fallback\n");
        }
    }

    if(mb_info==0) return;
    uint32_t flags = *(uint32_t*)mb_info;
    if(!(flags & (1<<12))) {
        terminal_writestring("  [FB] No FB flag, fallback to VGA text 80x25\n");
        return;
    }
    uint8_t* b = (uint8_t*)mb_info;
    uint32_t fb_addr_low = *(uint32_t*)(b + 88);
    uint32_t fb_pitch = *(uint32_t*)(b + 96);
    uint32_t fb_width = *(uint32_t*)(b + 100);
    uint32_t fb_height = *(uint32_t*)(b + 104);
    uint8_t fb_bpp = *(uint8_t*)(b + 108);
    uint8_t fb_type = *(uint8_t*)(b + 109);

    terminal_writestring("  [FB] VESA raw: addr=0x");
    {
        uint32_t a=fb_addr_low;
        char hex[]="0123456789ABCDEF";
        char out[9]; for(int i=0;i<8;i++){ out[7-i]=hex[a&0xF]; a>>=4; } out[8]=0;
        terminal_writestring(out);
        terminal_writestring(" pitch=");
        char buf[12]; int n=fb_pitch; int i=0; if(n==0)buf[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)buf[i++]=rev[--r];} buf[i]=0; terminal_writestring(buf);
        terminal_writestring(" ");
        n=fb_width; i=0; char b2[12]; if(n==0)b2[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b2[i++]=rev[--r];} b2[i]=0; terminal_writestring(b2);
        terminal_writestring("x");
        n=fb_height; i=0; char b3[12]; if(n==0)b3[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b3[i++]=rev[--r];} b3[i]=0; terminal_writestring(b3);
        terminal_writestring(" bpp=");
        n=fb_bpp; i=0; char b4[12]; if(n==0)b4[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b4[i++]=rev[--r];} b4[i]=0; terminal_writestring(b4);
        terminal_writestring("\n");
    }

    if(fb_width==0 || fb_height==0 || fb_addr_low==0) {
        terminal_writestring("  [FB] Invalid dims or addr 0, fallback\n");
        return;
    }

    fb.address = (uint32_t*)fb_addr_low;
    fb.pitch = fb_pitch;
    fb.width = fb_width;
    fb.height = fb_height;
    fb.bpp = fb_bpp;
    fb.type = fb_type;
    fb.is_available = 1;
    fb.is_text_mode = 0;

    terminal_writestring("  [FB] VESA OK! Booting to GUI Zero Ring\n");
    fb_clear(0x0A0A0F);
}

void fb_clear(uint32_t color) {
    if(fb.is_text_mode) {
        uint16_t* vga = (uint16_t*)0xB8000;
        uint16_t entry = (0x0F << 8) | ' ';
        for(int i=0;i<80*25;i++) vga[i] = entry;
    } else {
        uint32_t* addr = fb.address;
        uint32_t pixels = fb.pitch / 4;
        for(uint32_t y=0;y<fb.height;y++) {
            for(uint32_t x=0;x<fb.width;x++) {
                addr[y*pixels + x] = color;
            }
        }
    }
}

void fb_put_pixel(int x, int y, uint32_t color) {
    if(fb.is_text_mode) return;
    if(x<0||y<0|| (uint32_t)x>=fb.width || (uint32_t)y>=fb.height) return;
    uint32_t* addr = fb.address;
    uint32_t pixels = fb.pitch / 4;
    addr[y*pixels + x] = color;
}

void fb_fill_rect(int x, int y, int w, int h, uint32_t color) {
    if(fb.is_text_mode) return;
    for(int yy=y; yy<y+h; yy++) {
        for(int xx=x; xx<x+w; xx++) {
            fb_put_pixel(xx, yy, color);
        }
    }
}
void fb_draw_hline(int x, int y, int w, uint32_t color){ for(int i=0;i<w;i++) fb_put_pixel(x+i,y,color); }
void fb_draw_vline(int x, int y, int h, uint32_t color){ for(int i=0;i<h;i++) fb_put_pixel(x,y+i,color); }
void fb_draw_circle(int cx, int cy, int r, uint32_t color){
    if(fb.is_text_mode) return;
    int x=r,y=0,err=0;
    while(x>=y){
        fb_put_pixel(cx+x,cy+y,color); fb_put_pixel(cx+y,cy+x,color); fb_put_pixel(cx-y,cy+x,color); fb_put_pixel(cx-x,cy+y,color);
        fb_put_pixel(cx-x,cy-y,color); fb_put_pixel(cx-y,cy-x,color); fb_put_pixel(cx+y,cy-x,color); fb_put_pixel(cx+x,cy-y,color);
        y+=1; err+=1+2*y; if(2*(err-x)+1>0){ x-=1; err+=1-2*x; }
    }
}
