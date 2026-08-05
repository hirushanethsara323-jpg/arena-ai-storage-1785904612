#ifndef SERIAL_H
#define SERIAL_H

#include "io.h"

#define SERIAL_COM1 0x3F8

void serial_init(void);
void serial_putchar(char c);
void serial_writestring(const char* str);

#endif
