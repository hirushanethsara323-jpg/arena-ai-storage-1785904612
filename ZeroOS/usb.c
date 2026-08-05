#include "usb.h"
#include "pci.h"
#include "heap.h"

extern void terminal_writestring(const char*);

#define UHCI_CMD 0
#define UHCI_STS 2
#define UHCI_INTR 4
#define UHCI_FRNUM 6
#define UHCI_FLBASEADD 8

typedef struct {
    uint32_t link;
    uint32_t element;
    uint8_t data[32];
} uhci_qh_t;

typedef struct {
    uint32_t link;
    uint32_t ctrl;
    uint32_t token;
    uint32_t buffer;
    uint8_t reserved[16];
} uhci_td_t;

static int usb_device_count = 0;
static uhci_qh_t* qh_pool = 0;
static uhci_td_t* td_pool = 0;

void usb_init(void) {
    terminal_writestring("  [USB] Scanning PCI for USB...\n");
    int found=0;
    for(int i=0;i<4;i++) {
        uint8_t subclass=0xFF;
        if(i==0) subclass=0x00;
        if(i==1) subclass=0x10;
        if(i==2) subclass=0x20;
        if(i==3) subclass=0x30;
        pci_device_t* dev = pci_find_class(0x0C, subclass);
        if(dev) {
            terminal_writestring("  [USB] Found ");
            if(subclass==0x00) terminal_writestring("UHCI");
            else if(subclass==0x10) terminal_writestring("OHCI");
            else if(subclass==0x20) terminal_writestring("EHCI");
            else terminal_writestring("XHCI");
            terminal_writestring(" @ ");
            { char b[4]; int n=dev->device; char hex[]="0123456789ABCDEF"; b[0]=hex[n/16]; b[1]=hex[n%16]; b[2]=0; terminal_writestring(b); }
            terminal_writestring("\n");
            found++;
            // UHCI init would:
            // - Get IO base from BAR4
            // - Reset HC: outw(IO+0, 2) then wait
            // - Allocate frame list (1024 entries) via pmm
            // - Set FLBASEADD
            // - Set FRNUM 0
            // - Start HC: outw(IO+0, 1)
        }
    }
    if(found==0) {
        terminal_writestring("  [USB] No controller (QEMU -device usb-ehci)\n");
    } else {
        qh_pool = (uhci_qh_t*)kmalloc(4096);
        td_pool = (uhci_td_t*)kmalloc(4096);
        terminal_writestring("  [USB] QH/TD pool 4K+4K alloc, HC reset stub\n");
    }
    usb_device_count = found;
}

void usb_poll(void) {
    // Poll would check TDs for completion
}

int usb_get_device_count(void) { return usb_device_count; }
