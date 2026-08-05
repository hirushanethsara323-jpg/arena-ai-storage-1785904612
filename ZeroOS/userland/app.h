#ifndef APP_H
#define APP_H

#include "../io.h"

#define MAX_APPS 16
#define APP_NAME_MAX 24

typedef struct {
    char name[APP_NAME_MAX];
    char desc[64];
    uint8_t used;
    void (*entry)(void);
    uint32_t icon; // unicode char
    uint32_t color;
} app_t;

void app_init(void);
int app_register(const char* name, const char* desc, void (*entry)(void), uint32_t icon, uint32_t color);
void app_list(void);
int app_launch(const char* name);
void app_store(void);

#endif
