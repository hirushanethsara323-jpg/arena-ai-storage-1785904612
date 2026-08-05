#ifndef AC97_H
#define AC97_H

#include "io.h"

#define AC97_CLASS 0x04
#define AC97_SUBCLASS_AUDIO 0x01

void ac97_init(void);
void ac97_play_beep(uint32_t freq, uint32_t duration_ms);
int ac97_is_found(void);

#endif
