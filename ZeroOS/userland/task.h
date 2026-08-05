#ifndef TASK_H
#define TASK_H

#include "../io.h"

#define MAX_TASKS 16
#define TASK_STACK_SIZE 8192

#define TASK_STATE_READY 0
#define TASK_STATE_RUNNING 1
#define TASK_STATE_BLOCKED 2
#define TASK_STATE_DEAD 3

typedef struct task {
    uint32_t id;
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;
    uint32_t state;
    uint8_t* stack;
    char name[32];
    uint32_t ticks;
    struct task* next;
} task_t;

void tasking_init(void);
int task_create(const char* name, void (*entry)(void));
void task_yield(void);
void task_exit(int code);
task_t* task_get_current(void);
void task_schedule(void);
void task_list(void);

#endif
