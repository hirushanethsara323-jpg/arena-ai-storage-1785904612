#ifndef USB_H
#define USB_H

#include "io.h"

#define USB_CLASS 0x0C
#define USB_SUBCLASS_UHCI 0x00
#define USB_SUBCLASS_OHCI 0x10
#define USB_SUBCLASS_EHCI 0x20
#define USB_SUBCLASS_XHCI 0x30

void usb_init(void);
void usb_poll(void);
int usb_get_device_count(void);

#endif
