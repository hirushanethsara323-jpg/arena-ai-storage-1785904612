#ifndef MOUSE_H
#define MOUSE_H

#include "../io.h"

typedef struct {
    int x;
    int y;
    int buttons; // 1=left,2=right,4=middle
    int dx;
    int dy;
} mouse_state_t;

void mouse_init(void);
int mouse_poll(mouse_state_t* state);
extern mouse_state_t mouse;

#endif
