#include "serial.h"

void serial_init(void) {
    outb(SERIAL_COM1 + 1, 0x00); // disable interrupts
    outb(SERIAL_COM1 + 3, 0x80); // enable DLAB
    outb(SERIAL_COM1 + 0, 0x03); // divisor low 38400 baud
    outb(SERIAL_COM1 + 1, 0x00); // divisor high
    outb(SERIAL_COM1 + 3, 0x03); // 8 bits, no parity, one stop
    outb(SERIAL_COM1 + 2, 0xC7); // enable FIFO
    outb(SERIAL_COM1 + 4, 0x0B); // IRQs enabled
}

static int serial_is_transmit_empty(void) {
    return inb(SERIAL_COM1 + 5) & 0x20;
}

void serial_putchar(char c) {
    while(!serial_is_transmit_empty());
    outb(SERIAL_COM1, c);
    if(c=='\n') {
        while(!serial_is_transmit_empty());
        outb(SERIAL_COM1, '\r');
    }
}

void serial_writestring(const char* str) {
    while(*str) {
        serial_putchar(*str++);
    }
}
