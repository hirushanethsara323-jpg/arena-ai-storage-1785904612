#ifndef FONT_H
#define FONT_H

#include "../io.h"
#include "fb.h"

void font_draw_char(int x, int y, char c, uint32_t fg, uint32_t bg);
void font_draw_string(int x, int y, const char* str, uint32_t fg, uint32_t bg);
void font_draw_string_scale(int x, int y, const char* str, uint32_t fg, int scale);

#endif
