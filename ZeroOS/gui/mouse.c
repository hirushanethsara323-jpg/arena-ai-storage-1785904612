#include "mouse.h"
#include "../io.h"

mouse_state_t mouse;

#define MOUSE_DATA 0x60
#define MOUSE_STATUS 0x64
#define MOUSE_CMD 0x64

static void mouse_wait(uint8_t type) {
    int timeout=100000;
    if(type==0) {
        while(timeout--){ if((inb(MOUSE_STATUS)&1)==1) return; }
    } else {
        while(timeout--){ if((inb(MOUSE_STATUS)&2)==0) return; }
    }
}

static void mouse_write(uint8_t write) {
    mouse_wait(1);
    outb(MOUSE_CMD, 0xD4);
    mouse_wait(1);
    outb(MOUSE_DATA, write);
}

static uint8_t mouse_read(void) {
    mouse_wait(0);
    return inb(MOUSE_DATA);
}

void mouse_init(void) {
    mouse.x = 512;
    mouse.y = 384;
    mouse.buttons = 0;
    mouse.dx = 0;
    mouse.dy = 0;

    // Enable mouse
    uint8_t status;
    // Enable aux
    mouse_wait(1);
    outb(MOUSE_CMD, 0xA8);
    mouse_wait(1);
    outb(MOUSE_CMD, 0x20);
    mouse_wait(0);
    status = inb(MOUSE_DATA) | 2;
    mouse_wait(1);
    outb(MOUSE_CMD, 0x60);
    mouse_wait(1);
    outb(MOUSE_DATA, status);
    // Use default settings
    mouse_write(0xF6);
    mouse_read();
    mouse_write(0xF4);
    mouse_read();
}

int mouse_poll(mouse_state_t* state) {
    if(!(inb(MOUSE_STATUS) & 1)) return 0;
    // For simplicity, we check if aux data
    if(!(inb(MOUSE_STATUS) & 0x20)) return 0; // not mouse

    uint8_t b = mouse_read();
    uint8_t x = mouse_read();
    uint8_t y = mouse_read();

    int dx = x;
    if(dx && (b & (1<<4))) dx -= 256;
    int dy = y;
    if(dy && (b & (1<<5))) dy -= 256;

    mouse.dx = dx;
    mouse.dy = dy;
    mouse.x += dx;
    mouse.y -= dy;
    if(mouse.x < 0) mouse.x = 0;
    if(mouse.x > 1023) mouse.x = 1023;
    if(mouse.y < 0) mouse.y = 0;
    if(mouse.y > 767) mouse.y = 767;
    mouse.buttons = b & 0x07;

    if(state) *state = mouse;
    return 1;
}
