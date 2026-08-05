#ifndef FB_H
#define FB_H

#include "../io.h"

#define FB_WIDTH 1024
#define FB_HEIGHT 768

typedef struct {
    uint32_t* address;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint8_t bpp;
    uint8_t type;
    uint8_t is_available;
    uint8_t is_text_mode;
} framebuffer_t;

void fb_init(uint32_t mb_info);
void fb_clear(uint32_t color);
void fb_put_pixel(int x, int y, uint32_t color);
void fb_fill_rect(int x, int y, int w, int h, uint32_t color);
void fb_draw_circle(int cx, int cy, int r, uint32_t color);
void fb_draw_hline(int x, int y, int w, uint32_t color);
void fb_draw_vline(int x, int y, int h, uint32_t color);
uint32_t fb_color(uint8_t r, uint8_t g, uint8_t b);

extern framebuffer_t fb;

#endif
