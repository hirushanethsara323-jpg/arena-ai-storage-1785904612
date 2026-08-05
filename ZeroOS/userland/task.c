#include "task.h"
#include "../heap.h"
#include "../pmm.h"

extern void terminal_writestring(const char*);

static task_t tasks[MAX_TASKS];
static task_t* current_task = 0;
static task_t* ready_queue = 0;
static uint32_t next_tid = 1;
static int tasking_enabled = 0;

static void strcpy_t(char* d, const char* s){ while((*d++=*s++)); }

void tasking_init(void) {
    for(int i=0;i<MAX_TASKS;i++) {
        tasks[i].id = 0;
        tasks[i].state = TASK_STATE_DEAD;
        tasks[i].stack = 0;
    }
    // Create kernel task (id 0)
    tasks[0].id = 0;
    tasks[0].state = TASK_STATE_RUNNING;
    strcpy_t(tasks[0].name, "zero-kernel");
    tasks[0].esp = 0; // current esp
    tasks[0].ebp = 0;
    tasks[0].ticks = 0;
    current_task = &tasks[0];
    ready_queue = 0;
    tasking_enabled = 1;
    terminal_writestring("  > Tasking init: kernel task 0\n");
}

int task_create(const char* name, void (*entry)(void)) {
    for(int i=1;i<MAX_TASKS;i++) {
        if(tasks[i].state == TASK_STATE_DEAD) {
            tasks[i].stack = (uint8_t*)kmalloc(TASK_STACK_SIZE);
            if(!tasks[i].stack) return -1;

            // Setup stack for iret context
            uint32_t* stack_top = (uint32_t*)(tasks[i].stack + TASK_STACK_SIZE);
            // For simplicity, we push initial registers for context switch
            // Real task creation needs more setup with eflags, cs, eip etc.
            *--stack_top = 0x202; // eflags (interrupts enabled)
            *--stack_top = 0x08;  // cs
            *--stack_top = (uint32_t)entry; // eip
            *--stack_top = 0; // err, int_no dummy
            *--stack_top = 0; // eax etc - pusha would push 8 regs
            *--stack_top = 0;
            *--stack_top = 0;
            *--stack_top = 0;
            *--stack_top = 0;
            *--stack_top = 0;
            *--stack_top = 0;
            *--stack_top = 0;
            *--stack_top = 0x10; // ds

            tasks[i].esp = (uint32_t)stack_top;
            tasks[i].ebp = 0;
            tasks[i].eip = (uint32_t)entry;
            tasks[i].state = TASK_STATE_READY;
            tasks[i].id = next_tid++;
            strcpy_t(tasks[i].name, name);
            tasks[i].ticks = 0;

            // Add to ready queue
            tasks[i].next = ready_queue;
            ready_queue = &tasks[i];

            terminal_writestring("  > Task created: ");
            terminal_writestring(name);
            terminal_writestring(" id=");
            { char b[12]; int n=tasks[i].id; int ii=0; if(n==0)b[ii++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[ii++]=rev[--r];} b[ii]=0; terminal_writestring(b); }
            terminal_writestring("\n");
            return tasks[i].id;
        }
    }
    return -1;
}

void task_yield(void) {
    if(!tasking_enabled) return;
    task_schedule();
}

void task_schedule(void) {
    if(!ready_queue) return;
    // Simple round-robin
    task_t* prev = current_task;
    task_t* next = ready_queue;
    ready_queue = ready_queue->next;

    if(prev->state == TASK_STATE_RUNNING) {
        prev->state = TASK_STATE_READY;
        // push to ready queue tail (simple: add to front for now)
        prev->next = ready_queue;
        ready_queue = prev;
    }

    next->state = TASK_STATE_RUNNING;
    current_task = next;

    // Context switch would happen here via assembly:
    // mov esp, current->esp etc.
    // For sandbox build, we just simulate
}

task_t* task_get_current(void) {
    return current_task;
}

void task_exit(int code) {
    (void)code;
    if(current_task->id == 0) {
        terminal_writestring("\n [TASK] Kernel cannot exit!\n");
        return;
    }
    terminal_writestring("\n [TASK] Exit: ");
    terminal_writestring(current_task->name);
    terminal_writestring("\n");
    if(current_task->stack) kfree(current_task->stack);
    current_task->state = TASK_STATE_DEAD;
    current_task->stack = 0;
    task_yield();
    while(1) __asm__ volatile ("hlt");
}

void task_list(void) {
    terminal_writestring("\n [Tasks] ");
    int count=0; for(int i=0;i<MAX_TASKS;i++) if(tasks[i].state!=TASK_STATE_DEAD) count++;
    { char b[12]; int n=count; int ii=0; if(n==0)b[ii++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[ii++]=rev[--r];} b[ii]=0; terminal_writestring(b); }
    terminal_writestring(" running:\n");
    for(int i=0;i<MAX_TASKS;i++) {
        if(tasks[i].state!=TASK_STATE_DEAD) {
            terminal_writestring("  ");
            { char b[12]; int n=tasks[i].id; int ii=0; if(n==0)b[ii++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[ii++]=rev[--r];} b[ii]=0; terminal_writestring(b); }
            terminal_writestring(" - ");
            terminal_writestring(tasks[i].name);
            if(tasks[i].state==TASK_STATE_RUNNING) terminal_writestring(" [RUN]");
            else if(tasks[i].state==TASK_STATE_READY) terminal_writestring(" [READY]");
            terminal_writestring("\n");
        }
    }
    terminal_writestring("\n");
}
