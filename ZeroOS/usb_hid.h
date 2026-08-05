#ifndef USB_HID_H
#define USB_HID_H

#include "io.h"

void usb_hid_init(void);
int usb_hid_poll_keyboard(char* out_char);
int usb_hid_poll_mouse(int* dx, int* dy, int* buttons);
void usb_hid_inject(const char* str);

#endif
