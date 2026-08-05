#include "io.h"
#include "keyboard.h"

#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64

// US QWERTY scancode set 1
static char scancode_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, /* ctrl */
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, /* lshift */
    '\\','z','x','c','v','b','n','m',',','.','/',
    0, /* rshift */
    '*',
    0, /* alt */
    ' ', /* space */
    0, /* caps */
    0,0,0,0,0,0,0,0,0,0, // F1-F10
    0,0, // numlock, scrolllock
    0, // home
    0, // up
    0, // pgup
    '-',
    0, // left
    0, // center
    0, // right
    '+',
    0, // end
    0, // down
    0, // pgdown
    0, // ins
    0, // del
    0,0,0,
    0,0, // F11 F12
    0,
};

static char scancode_shift_ascii[128] = {
    0,  27, '!','@','#','$','%','^','&','*','(',')','_','+', '\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,
    'A','S','D','F','G','H','J','K','L',':','"','~',
    0,
    '|','Z','X','C','V','B','N','M','<','>','?',
    0,
    '*',
    0,
    ' ',
    0,
};

static int shift_pressed = 0;
static int caps_lock = 0;

void keyboard_init(void) {
    // Clear buffer
    while(inb(KBD_STATUS_PORT) & 1) {
        inb(KBD_DATA_PORT);
    }
}

int keyboard_has_key(void) {
    return inb(KBD_STATUS_PORT) & 1;
}

char keyboard_poll(void) {
    if(!keyboard_has_key()) return 0;

    uint8_t scancode = inb(KBD_DATA_PORT);
    
    // Key release
    if(scancode & 0x80) {
        uint8_t released = scancode & 0x7F;
        if(released == 0x2A || released == 0x36) shift_pressed = 0; // shift release
        return 0;
    }

    // Special keys
    if(scancode == 0x2A || scancode == 0x36) { // lshift, rshift
        shift_pressed = 1;
        return 0;
    }
    if(scancode == 0x3A) { // capslock
        caps_lock = !caps_lock;
        return 0;
    }

    if(scancode >= 128) return 0;

    char c;
    if(shift_pressed) {
        c = scancode_shift_ascii[scancode];
        if(caps_lock && c >= 'A' && c <= 'Z') {
            // shift + caps = lowercase
            if(scancode_ascii[scancode] >= 'a' && scancode_ascii[scancode] <= 'z')
                c = scancode_ascii[scancode];
        }
    } else {
        c = scancode_ascii[scancode];
        if(caps_lock && c >= 'a' && c <= 'z') {
            c = c - 32; // to upper
        }
    }

    return c;
}

char keyboard_getchar(void) {
    char c;
    while((c = keyboard_poll()) == 0) {
        // halt to save power
        __asm__ volatile ("hlt");
    }
    return c;
}
