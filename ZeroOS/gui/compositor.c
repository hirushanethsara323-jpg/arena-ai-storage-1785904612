#include "compositor.h"
#include "fb.h"
#include "font.h"
#include "../heap.h"
#include "../io.h"

window_t windows[MAX_WINDOWS];
zero_ring_t zero_ring;

static int mouse_down_x = 0, mouse_down_y = 0;
static int dragging_window = -1;
static int drag_offset_x = 0, drag_offset_y = 0;

void compositor_init(void) {
    for(int i=0;i<MAX_WINDOWS;i++) {
        windows[i].used = 0;
        windows[i].visible = 0;
        windows[i].buffer = 0;
    }
    zero_ring.ring_x = 512;
    zero_ring.ring_y = 384;
    zero_ring.ring_radius = 100;
    zero_ring.ring_active = 0;
    zero_ring.ring_pulse = 0;

    compositor_create_window("Zero Shell", 100, 100, 400, 300);
    compositor_create_window("Zero Files", 150, 150, 400, 300);
    compositor_create_window("Zero Store", 200, 200, 400, 250);
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
            uint32_t size = w*h*4;
            windows[i].buffer = (uint32_t*)kmalloc(size);
            if(!windows[i].buffer) { windows[i].used=0; return -1; }
            for(int j=0;j<w*h;j++) windows[i].buffer[j] = 0x15151F;
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
    if(fb.is_text_mode) return;

    fb_clear(0x0A0A0F);

    // Grid dots
    for(int y=0;y<(int)fb.height;y+=32) {
        for(int x=0;x<(int)fb.width;x+=32) {
            fb_put_pixel(x, y, 0x1A1A2F);
        }
    }

    // Zero Ring
    int cx = zero_ring.ring_x;
    int cy = zero_ring.ring_y;
    int r = zero_ring.ring_radius + (zero_ring.ring_active ? 20 : 0);

    for(int i=0;i<8;i++) {
        uint32_t glow = zero_ring.ring_active ? 0x8B5CF6 : 0x00FFD1;
        fb_draw_circle(cx, cy, r+i, glow);
    }
    fb_draw_circle(cx, cy, r, 0x00FFD1);
    fb_draw_circle(cx, cy, r-1, 0x00FFD1);
    fb_draw_circle(cx, cy, r+1, 0x8B5CF6);

    // Core
    fb_fill_rect(cx-36, cy-36, 72, 72, 0x00FFD1);
    font_draw_string(cx-12, cy-4, "0", 0x000000, 0x00FFD1);

    // Orbit apps when active
    if(zero_ring.ring_active) {
        const char* apps[6] = {"Term", "Files", "Browser", "Editor", "Settings", "AI"};
        int angles[6] = {0, 60, 120, 180, 240, 300};
        for(int i=0;i<6;i++) {
            int ang = angles[i];
            // Simple circular positions
            int ox = cx + (r+60) * ((ang==0?1:ang==180?-1:ang==60||ang==300?1:ang==120||ang==240?-1:0));
            int oy = cy + (r+60) * ((ang==90?1:ang==270?-1:ang==0||ang==180?0:ang<180?1:-1));
            // Approximate with sin/cos via simple
            // Use precomputed rough
            if(ang==0) { ox=cx+r+70; oy=cy; }
            else if(ang==60) { ox=cx+(r+60)/2; oy=cy-(r+60)*0.866; }
            else if(ang==120) { ox=cx-(r+60)/2; oy=cy-(r+60)*0.866; }
            else if(ang==180) { ox=cx-r-70; oy=cy; }
            else if(ang==240) { ox=cx-(r+60)/2; oy=cy+(r+60)*0.866; }
            else if(ang==300) { ox=cx+(r+60)/2; oy=cy+(r+60)*0.866; }
            fb_fill_rect(ox-20, oy-20, 40, 40, 0x15151F);
            fb_draw_circle(ox, oy, 20, 0x00FFD1);
            font_draw_string(ox-12, oy-4, apps[i], 0xE8E8EC, 0x15151F);
        }
    }

    // Windows
    for(int i=0;i<MAX_WINDOWS;i++) {
        if(!windows[i].used||!windows[i].visible) continue;
        fb_fill_rect(windows[i].x+4, windows[i].y+4, windows[i].w, windows[i].h, 0x000000);
        uint32_t border = windows[i].focused ? 0x00FFD1 : 0x333333;
        fb_fill_rect(windows[i].x, windows[i].y, windows[i].w, 1, border);
        fb_fill_rect(windows[i].x, windows[i].y+windows[i].h-1, windows[i].w, 1, border);
        fb_fill_rect(windows[i].x, windows[i].y, 1, windows[i].h, border);
        fb_fill_rect(windows[i].x+windows[i].w-1, windows[i].y, 1, windows[i].h, border);
        fb_fill_rect(windows[i].x, windows[i].y, windows[i].w, 28, 0x15151F);
        font_draw_string(windows[i].x+8, windows[i].y+8, windows[i].title, 0xE8E8EC, 0x15151F);
        // Close button
        fb_fill_rect(windows[i].x+windows[i].w-22, windows[i].y+6, 16, 16, 0xFF5F57);
        font_draw_string(windows[i].x+windows[i].w-18, windows[i].y+8, "x", 0x000000, 0xFF5F57);
        for(int yy=0; yy<windows[i].h-28; yy++) {
            for(int xx=0; xx<windows[i].w; xx++) {
                if(windows[i].x+xx < (int)fb.width && windows[i].y+28+yy < (int)fb.height) {
                    uint32_t col = windows[i].buffer[yy*windows[i].w+xx];
                    fb.address[(windows[i].y+28+yy)*(fb.pitch/4)+windows[i].x+xx] = col;
                }
            }
        }
    }

    // Status bar
    font_draw_string(10, 10, "Zero OS v8.0 Real GUI - Zero Ring Active", 0x6B7280, 0x0A0A0F);

    // Mouse cursor is drawn separately in kernel GUI loop to avoid flicker in compositor
}

extern int mouse_x, mouse_y; // from mouse driver? We'll use global

void compositor_handle_mouse(int mx, int my, int buttons) {
    static int last_buttons = 0;

    // Ring click
    int dx = mx - zero_ring.ring_x;
    int dy = my - zero_ring.ring_y;
    int dist2 = dx*dx + dy*dy;
    int r = zero_ring.ring_radius;
    if(dist2 < (r+10)*(r+10) && dist2 > (r-10)*(r-10)) {
        if((buttons & 1) && !(last_buttons & 1)) {
            zero_ring.ring_active = !zero_ring.ring_active;
        }
    }

    // Check orbit apps click when ring active
    if(zero_ring.ring_active && (buttons & 1) && !(last_buttons & 1)) {
        // Approximate check for 6 orbit positions
        int cx = zero_ring.ring_x, cy = zero_ring.ring_y;
        int orbit_r = r+70;
        // Check each orbit app area
        const char* app_names[6] = {"terminal", "files", "browser", "editor", "settings", "ai"};
        int ox[6], oy[6];
        ox[0]=cx+orbit_r; oy[0]=cy;
        ox[1]=cx+orbit_r/2; oy[1]=cy-(int)(orbit_r*0.866);
        ox[2]=cx-orbit_r/2; oy[2]=cy-(int)(orbit_r*0.866);
        ox[3]=cx-orbit_r; oy[3]=cy;
        ox[4]=cx-orbit_r/2; oy[4]=cy+(int)(orbit_r*0.866);
        ox[5]=cx+orbit_r/2; oy[5]=cy+(int)(orbit_r*0.866);
        for(int i=0;i<6;i++) {
            int ddx=mx-ox[i], ddy=my-oy[i];
            if(ddx*ddx+ddy*ddy < 400) {
                // Launch app as window
                char title[32];
                int k=0; while(app_names[i][k] && k<30){ title[k]=app_names[i][k]; k++; } title[k]=0;
                // Capitalize first
                if(title[0]>='a'&&title[0]<='z') title[0]-=32;
                compositor_create_window(title, 200+i*20, 200+i*20, 400, 300);
                zero_ring.ring_active=0;
                break;
            }
        }
    }

    // Window handling
    if(buttons & 1) {
        if(!(last_buttons & 1)) {
            // Mouse down - check close button and title bar
            for(int i=MAX_WINDOWS-1;i>=0;i--) {
                if(!windows[i].used||!windows[i].visible) continue;
                // Close button
                if(mx >= windows[i].x+windows[i].w-22 && mx < windows[i].x+windows[i].w-6 &&
                   my >= windows[i].y+6 && my < windows[i].y+22) {
                    compositor_close_window(i);
                    break;
                }
                // Title bar drag start
                if(mx >= windows[i].x && mx < windows[i].x+windows[i].w &&
                   my >= windows[i].y && my < windows[i].y+28) {
                    compositor_focus_window(i);
                    dragging_window = i;
                    mouse_down_x = mx;
                    mouse_down_y = my;
                    drag_offset_x = mx - windows[i].x;
                    drag_offset_y = my - windows[i].y;
                    break;
                }
                // Window body focus
                if(mx >= windows[i].x && mx < windows[i].x+windows[i].w &&
                   my >= windows[i].y && my < windows[i].y+windows[i].h) {
                    compositor_focus_window(i);
                    break;
                }
            }
        } else {
            // Mouse held - drag
            if(dragging_window>=0 && dragging_window<MAX_WINDOWS && windows[dragging_window].used) {
                windows[dragging_window].x = mx - drag_offset_x;
                windows[dragging_window].y = my - drag_offset_y;
            }
        }
    } else {
        dragging_window = -1;
    }

    last_buttons = buttons;
}
