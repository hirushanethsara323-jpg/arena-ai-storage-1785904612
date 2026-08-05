#include "fb.h"

framebuffer_t fb;

uint32_t fb_color(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 16) | (g << 8) | b;
}

void fb_init(uint32_t mb_info) {
    (void)mb_info;
    fb.is_available = 0;
    fb.is_text_mode = 1;
    fb.width = 80;
    fb.height = 25;
    fb.address = (uint32_t*)0xB8000;
    fb.pitch = 0;
    fb.bpp = 16;
}

void fb_clear(uint32_t color) {
    if(fb.is_text_mode) {
        // Clear VGA text
        uint16_t* vga = (uint16_t*)0xB8000;
        uint16_t entry = (0x0F << 8) | ' ';
        for(int i=0;i<80*25;i++) vga[i] = entry;
    } else {
        // Graphical clear
        for(uint32_t y=0;y<fb.height;y++) {
            for(uint32_t x=0;x<fb.width;x++) {
                fb.address[y*(fb.pitch/4)+x] = color;
            }
        }
    }
}

void fb_put_pixel(int x, int y, uint32_t color) {
    if(fb.is_text_mode) return; // no pixel in text mode
    if(x<0||y<0|| (uint32_t)x>=fb.width || (uint32_t)y>=fb.height) return;
    fb.address[y*(fb.pitch/4)+x] = color;
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
