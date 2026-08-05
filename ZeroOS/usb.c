#include "usb.h"
#include "pci.h"

extern void terminal_writestring(const char*);

static int usb_device_count = 0;

void usb_init(void) {
    terminal_writestring("  [USB] Scanning PCI for USB controllers...\n");
    // Find UHCI, OHCI, EHCI, XHCI via PCI class 0x0C subclass
    for(int i=0;i<4;i++) {
        uint8_t subclass = 0xFF;
        if(i==0) subclass=0x00; // UHCI
        if(i==1) subclass=0x10; // OHCI
        if(i==2) subclass=0x20; // EHCI
        if(i==3) subclass=0x30; // XHCI
        pci_device_t* dev = pci_find_class(0x0C, subclass);
        if(dev) {
            terminal_writestring("  [USB] Found ");
            if(subclass==0x00) terminal_writestring("UHCI");
            else if(subclass==0x10) terminal_writestring("OHCI");
            else if(subclass==0x20) terminal_writestring("EHCI");
            else terminal_writestring("XHCI");
            terminal_writestring(" controller\n");
            usb_device_count++;
        }
    }
    if(usb_device_count==0) {
        terminal_writestring("  [USB] No USB controller (sim in QEMU -bochs without USB)\n");
    } else {
        terminal_writestring("  [USB] Initializing host controller (stub)...\n");
    }
}

void usb_poll(void) {
    // Poll USB devices
}

int usb_get_device_count(void) { return usb_device_count; }
