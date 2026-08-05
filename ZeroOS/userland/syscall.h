#ifndef SYSCALL_H
#define SYSCALL_H

#include "../io.h"

#define SYSCALL_EXIT 1
#define SYSCALL_WRITE 4
#define SYSCALL_READ 3
#define SYSCALL_OPEN 5
#define SYSCALL_EXEC 11
#define SYSCALL_FORK 2
#define SYSCALL_YIELD 158
#define SYSCALL_ZERO 100 // custom: zero info

// Syscall handler - int 0x80
void syscall_init(void);
int syscall_handler(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3);

#endif
