#include "compositor.h"
#include "fb.h"
#include "../heap.h"
#include "../io.h"

window_t windows[MAX_WINDOWS];
zero_ring_t zero_ring;


void compositor_init(void) {
    for(int i=0;i<MAX_WINDOWS;i++) {
        windows[i].used = 0;
        windows[i].visible = 0;
        windows[i].buffer = 0;
    }
    zero_ring.ring_x = fb.width/2;
    zero_ring.ring_y = fb.height/2;
    zero_ring.ring_radius = 100;
    zero_ring.ring_active = 0;
    zero_ring.ring_pulse = 0;

    // Create default windows for demo - in real GUI they orbit
    compositor_create_window("Zero Shell", 100, 100, 400, 300);
    compositor_create_window("Zero Files", 150, 150, 400, 300);
}

int compositor_create_window(const char* title, int x, int y, int w, int h) {
    for(int i=0;i<MAX_WINDOWS;i++) {
        if(!windows[i].used) {
            windows[i].used = 1;
            windows[i].visible = 1;
            windows[i].x = x;
            windows[i].y = y;
            windows[i].w = w;
            windows[i].h = h;
            windows[i].focused = 1;
            windows[i].minimized = 0;
            { int _ii=0; while(title[_ii] && _ii<WINDOW_TITLE_MAX-1){ windows[i].title[_ii]=title[_ii]; _ii++; } windows[i].title[_ii]=0; }
            // Allocate backbuffer: w*h*4
            uint32_t size = w*h*4;
            windows[i].buffer = (uint32_t*)kmalloc(size);
            if(!windows[i].buffer) { windows[i].used=0; return -1; }
            // Clear buffer with surface color #15151F
            for(int j=0;j<w*h;j++) windows[i].buffer[j] = 0x15151F;
            // Focus handling
            for(int j=0;j<MAX_WINDOWS;j++) if(j!=i) windows[j].focused=0;
            return i;
        }
    }
    return -1;
}

void compositor_close_window(int id) {
    if(id<0||id>=MAX_WINDOWS) return;
    if(!windows[id].used) return;
    if(windows[id].buffer) kfree(windows[id].buffer);
    windows[id].used = 0;
    windows[id].visible = 0;
}

void compositor_focus_window(int id) {
    for(int i=0;i<MAX_WINDOWS;i++) windows[i].focused = (i==id);
}

void compositor_draw(void) {
    if(fb.is_text_mode) {
        // In text mode, we can't draw graphical ring, but we simulate with VGA text
        // For now, do nothing - shell will handle output
        return;
    }

    // Clear screen #0A0A0F
    fb_clear(0x0A0A0F);

    // Draw subtle grid dots
    for(int y=0;y<(int)fb.height;y+=32) {
        for(int x=0;x<(int)fb.width;x+=32) {
            fb_put_pixel(x, y, 0x1A1A2F);
        }
    }

    // Draw Zero Ring
    int cx = zero_ring.ring_x;
    int cy = zero_ring.ring_y;
    int r = zero_ring.ring_radius + (zero_ring.ring_active ? 20 : 0);

    // Glow
    for(int i=0;i<8;i++) {
        uint32_t glow = zero_ring.ring_active ? 0x8B5CF6 : 0x00FFD1;
        // fade glow
        fb_draw_circle(cx, cy, r+i, glow);
    }

    // Ring itself - 2px
    fb_draw_circle(cx, cy, r, 0x00FFD1);
    fb_draw_circle(cx, cy, r-1, 0x00FFD1);
    fb_draw_circle(cx, cy, r+1, 0x8B5CF6);

    // Core
    fb_fill_rect(cx-36, cy-36, 72, 72, 0x00FFD1); // simplified core as rect, real would be circle gradient

    // Draw windows
    for(int i=0;i<MAX_WINDOWS;i++) {
        if(!windows[i].used||!windows[i].visible) continue;
        // Shadow
        fb_fill_rect(windows[i].x+4, windows[i].y+4, windows[i].w, windows[i].h, 0x000000);
        // Border
        uint32_t border = windows[i].focused ? 0x00FFD1 : 0x333333;
        fb_fill_rect(windows[i].x, windows[i].y, windows[i].w, 1, border);
        fb_fill_rect(windows[i].x, windows[i].y+windows[i].h-1, windows[i].w, 1, border);
        fb_fill_rect(windows[i].x, windows[i].y, 1, windows[i].h, border);
        fb_fill_rect(windows[i].x+windows[i].w-1, windows[i].y, 1, windows[i].h, border);
        // Title bar
        fb_fill_rect(windows[i].x, windows[i].y, windows[i].w, 28, 0x15151F);
        // Window content (blit buffer)
        for(int yy=0; yy<windows[i].h-28; yy++) {
            for(int xx=0; xx<windows[i].w; xx++) {
                if(windows[i].x+xx < (int)fb.width && windows[i].y+28+yy < (int)fb.height) {
                    uint32_t col = windows[i].buffer[yy*windows[i].w+xx];
                    fb.address[(windows[i].y+28+yy)*(fb.pitch/4)+windows[i].x+xx] = col;
                }
            }
        }
    }
}

void compositor_handle_mouse(int mx, int my, int buttons) {
    // Check if click on ring
    int dx = mx - zero_ring.ring_x;
    int dy = my - zero_ring.ring_y;
    int dist = dx*dx + dy*dy;
    int r2 = zero_ring.ring_radius*zero_ring.ring_radius;
    if(dist < r2+400 && dist > r2-400) {
        if(buttons & 1) zero_ring.ring_active = !zero_ring.ring_active;
    }

    // Check windows
    if(buttons & 1) {
        for(int i=MAX_WINDOWS-1;i>=0;i--) {
            if(!windows[i].used||!windows[i].visible) continue;
            if(mx >= windows[i].x && mx < windows[i].x+windows[i].w &&
               my >= windows[i].y && my < windows[i].y+windows[i].h) {
                compositor_focus_window(i);
                break;
            }
        }
    }

    // Drag focused window if near title bar
    // TODO
}
