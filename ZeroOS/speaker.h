#ifndef SPEAKER_H
#define SPEAKER_H

#include "io.h"

void speaker_beep(uint32_t frequency_hz);
void speaker_nobeep(void);
void speaker_play_tone(uint32_t freq, uint32_t ms);

#endif
