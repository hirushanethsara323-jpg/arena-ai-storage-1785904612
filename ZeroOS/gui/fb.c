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

    if(mb_info==0) return;

    uint32_t flags = *(uint32_t*)mb_info;
    if(!(flags & (1<<12))) return;

    uint8_t* byte_info = (uint8_t*)mb_info;
    uint32_t fb_addr_low = *(uint32_t*)(byte_info + 88);
    uint32_t fb_pitch = *(uint32_t*)(byte_info + 96);
    uint32_t fb_width = *(uint32_t*)(byte_info + 100);
    uint32_t fb_height = *(uint32_t*)(byte_info + 104);
    uint8_t fb_bpp = *(uint8_t*)(byte_info + 108);
    uint8_t fb_type = *(uint8_t*)(byte_info + 109);

    // Basic sanity
    if(fb_width==0 || fb_height==0) return;
    if(fb_addr_low==0) return;

    fb.address = (uint32_t*)fb_addr_low;
    fb.pitch = fb_pitch;
    fb.width = fb_width;
    fb.height = fb_height;
    fb.bpp = fb_bpp;
    fb.type = fb_type;
    fb.is_available = 1;
    fb.is_text_mode = 0;

    // Clear with Zero bg #0A0A0F
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
        // If bpp 32, pitch is bytes, so pixels = pitch/4
        // For simplicity, assume 32bpp
        if(fb.bpp==32) {
            for(uint32_t y=0;y<fb.height;y++) {
                for(uint32_t x=0;x<fb.width;x++) {
                    addr[y*pixels + x] = color;
                }
            }
        } else {
            // fallback
            for(uint32_t y=0;y<fb.height;y++) {
                for(uint32_t x=0;x<fb.width;x++) {
                    addr[y*pixels + x] = color;
                }
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

void fb_draw_hline(int x, int y, int w, uint32_t color) {
    for(int i=0;i<w;i++) fb_put_pixel(x+i, y, color);
}
void fb_draw_vline(int x, int y, int h, uint32_t color) {
    for(int i=0;i<h;i++) fb_put_pixel(x, y+i, color);
}

void fb_draw_circle(int cx, int cy, int r, uint32_t color) {
    if(fb.is_text_mode) return;
    int x = r;
    int y = 0;
    int err = 0;
    while(x>=y) {
        fb_put_pixel(cx+x, cy+y, color);
        fb_put_pixel(cx+y, cy+x, color);
        fb_put_pixel(cx-y, cy+x, color);
        fb_put_pixel(cx-x, cy+y, color);
        fb_put_pixel(cx-x, cy-y, color);
        fb_put_pixel(cx-y, cy-x, color);
        fb_put_pixel(cx+y, cy-x, color);
        fb_put_pixel(cx+x, cy-y, color);
        y+=1;
        err+=1+2*y;
        if(2*(err-x)+1>0) { x-=1; err+=1-2*x; }
    }
}
