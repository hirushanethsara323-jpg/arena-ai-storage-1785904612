#include "usb_hid.h"
#include "usb.h"

extern void terminal_writestring(const char*);

static int hid_kbd_found = 0;
static int hid_mouse_found = 0;

void usb_hid_init(void) {
    terminal_writestring("  [USB-HID] Enumerating HID devices...\n");
    // In real UHCI driver:
    // - Reset port
    // - Get device descriptor
    // - Set address
    // - Get config descriptor
    // - Parse HID report descriptor
    // - Set protocol
    // For now, simulate detection
    if(usb_get_device_count() > 0) {
        hid_kbd_found = 1;
        hid_mouse_found = 1;
        terminal_writestring("  [USB-HID] Keyboard + Mouse found (sim)\n");
    } else {
        terminal_writestring("  [USB-HID] No HID (plug USB KBD in QEMU -usbdevice keyboard)\n");
    }
}

int usb_hid_poll_keyboard(char* out_char) {
    (void)out_char;
    if(!hid_kbd_found) return 0;
    // Would read interrupt transfer from endpoint
    return 0;
}

int usb_hid_poll_mouse(int* dx, int* dy, int* buttons) {
    (void)dx; (void)dy; (void)buttons;
    if(!hid_mouse_found) return 0;
    return 0;
}
