#ifndef AI_H
#define AI_H

#include "io.h"

void ai_init(void);
void ai_query(const char* question, char* answer, uint32_t max_len);
void ai_chat(const char* input);

#endif
