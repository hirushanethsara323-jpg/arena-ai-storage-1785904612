#ifndef NET_H
#define NET_H

#include "io.h"

#define NET_CLASS 0x02

void net_init(void);
int net_send(const char* data, uint32_t len);
int net_receive(char* buffer, uint32_t max);
void net_list(void);

#endif
