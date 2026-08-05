#include "ac97.h"
#include "pci.h"
#include "speaker.h"

extern void terminal_writestring(const char*);

static int ac97_found = 0;
static uint32_t ac97_io_base = 0;

void ac97_init(void) {
    terminal_writestring("  [AC97] Scanning PCI for audio...\n");
    // Find audio device class 0x04 subclass 0x01
    pci_device_t* dev = pci_find_class(0x04, 0x01);
    if(!dev) dev = pci_find_class(0x04, 0xFF); // any multimedia
    if(dev) {
        terminal_writestring("  [AC97] Found audio device vendor=0x");
        { uint32_t v=dev->vendor_id; char hex[]="0123456789ABCDEF"; char out[5]; out[0]=hex[(v>>12)&0xF]; out[1]=hex[(v>>8)&0xF]; out[2]=hex[(v>>4)&0xF]; out[3]=hex[v&0xF]; out[4]=0; terminal_writestring(out); }
        terminal_writestring("\n");
        // Get IO base from BAR0
        uint32_t bar0 = pci_read(dev->bus, dev->device, dev->function, 0x10);
        ac97_io_base = bar0 & ~0x1;
        terminal_writestring("  [AC97] IO base 0x");
        { uint32_t v=ac97_io_base; char hex[]="0123456789ABCDEF"; char out[9]; for(int j=0;j<8;j++){ out[7-j]=hex[v&0xF]; v>>=4;} out[8]=0; terminal_writestring(out); }
        terminal_writestring(" (PCM stub)\n");
        ac97_found = 1;
    } else {
        terminal_writestring("  [AC97] No audio device (QEMU -soundhw ac97)\n");
    }
}

void ac97_play_beep(uint32_t freq, uint32_t duration_ms) {
    if(!ac97_found) {
        // Fallback to PC speaker
        speaker_play_tone(freq, duration_ms);
        return;
    }
    // Real AC97 would program PCM: set rate, write samples to buffer
    terminal_writestring("  [AC97] Playing tone via PCM (sim)\n");
    speaker_play_tone(freq, duration_ms);
}

int ac97_is_found(void) { return ac97_found; }
