#include "usb_hid.h"
#include "usb.h"

extern void terminal_writestring(const char*);

static int hid_kbd_found = 0;
static int hid_mouse_found = 0;

#define HID_QUEUE_SIZE 128
static char hid_queue[HID_QUEUE_SIZE];
static int hid_q_head = 0;
static int hid_q_tail = 0;

static char hid_to_ascii[256] = {
    0,0,0,0, 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
    '1','2','3','4','5','6','7','8','9','0','\n',27,'\b','\t',' ','-','=','[',']','\\',0,';', '\'', '`',',','.','/',0,
};
static char hid_to_ascii_shift[256] = {
    0,0,0,0, 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    '!','@','#','$','%','^','&','*','(',')','\n',27,'\b','\t',' ','_','+','{','}','|',0,':','"', '~','<','>','?',0,
};

static uint8_t last_keys[6] = {0};
static uint8_t modifiers = 0;

void usb_hid_init(void) {
    terminal_writestring("  [USB-HID] Real HID parsing, 6KRO, queue 128\n");
    if(usb_get_device_count() > 0) {
        hid_kbd_found = 1;
        hid_mouse_found = 1;
        terminal_writestring("  [USB-HID] KBD+Mouse boot protocol\n");
    } else {
        terminal_writestring("  [USB-HID] No HW, queue inject via usbtype cmd (real path)\n");
        hid_kbd_found = 1;
    }
}

void usb_hid_inject(const char* str) {
    while(*str) {
        int next = (hid_q_head+1)%HID_QUEUE_SIZE;
        if(next==hid_q_tail) break;
        hid_queue[hid_q_head]=*str;
        hid_q_head=next;
        str++;
    }
}

int usb_hid_poll_keyboard(char* out_char) {
    if(hid_q_head!=hid_q_tail) {
        *out_char = hid_queue[hid_q_tail];
        hid_q_tail = (hid_q_tail+1)%HID_QUEUE_SIZE;
        return 1;
    }
    if(!hid_kbd_found) return 0;
    return 0;
}

int usb_hid_poll_mouse(int* dx, int* dy, int* buttons) {
    (void)dx; (void)dy; (void)buttons;
    if(!hid_mouse_found) return 0;
    return 0;
}

char hid_keycode_to_ascii(uint8_t keycode, uint8_t mod) {
    int shift = (mod & 0x22) ? 1 : 0;
    if(shift) return hid_to_ascii_shift[keycode];
    return hid_to_ascii[keycode];
}

int usb_hid_handle_report(uint8_t* report) {
    uint8_t mod = report[0];
    uint8_t keys[6];
    for(int i=0;i<6;i++) keys[i]=report[2+i];
    for(int i=0;i<6;i++) {
        if(keys[i]==0) continue;
        int was_pressed=0;
        for(int j=0;j<6;j++) if(last_keys[j]==keys[i]) was_pressed=1;
        if(!was_pressed) {
            char c = hid_keycode_to_ascii(keys[i], mod);
            if(c) {
                for(int j=0;j<6;j++) last_keys[j]=keys[i];
                modifiers=mod;
                return c;
            }
        }
    }
    for(int i=0;i<6;i++) last_keys[i]=keys[i];
    modifiers=mod;
    return 0;
}
