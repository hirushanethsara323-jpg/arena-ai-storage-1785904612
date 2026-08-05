#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include "../io.h"
#include "fb.h"

#define MAX_WINDOWS 16
#define WINDOW_TITLE_MAX 32

typedef struct {
    int x, y;
    int w, h;
    int used;
    int focused;
    int minimized;
    char title[WINDOW_TITLE_MAX];
    uint32_t* buffer; // window backbuffer 32-bit
    uint8_t visible;
} window_t;

typedef struct {
    int ring_x, ring_y;
    int ring_radius;
    int ring_active;
    int ring_pulse;
} zero_ring_t;

void compositor_init(void);
void compositor_draw(void);
int compositor_create_window(const char* title, int x, int y, int w, int h);
void compositor_close_window(int id);
void compositor_focus_window(int id);
void compositor_handle_mouse(int mx, int my, int buttons);

extern window_t windows[MAX_WINDOWS];
extern zero_ring_t zero_ring;

#endif
