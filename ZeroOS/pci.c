#include "pci.h"

extern void terminal_writestring(const char*);

#define MAX_PCI_DEVICES 32
static pci_device_t pci_devices[MAX_PCI_DEVICES];
static int pci_device_count = 0;

uint32_t pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t address = (1<<31) | (bus<<16) | (device<<11) | (function<<8) | (offset & 0xFC);
    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}

void pci_scan(void) {
    pci_device_count = 0;
    terminal_writestring("  [PCI] Scanning bus 0...\n");
    for(uint8_t bus=0; bus<1; bus++) {
        for(uint8_t dev=0; dev<32; dev++) {
            for(uint8_t func=0; func<8; func++) {
                uint32_t vendor_device = pci_read(bus, dev, func, 0);
                uint16_t vendor = vendor_device & 0xFFFF;
                if(vendor == 0xFFFF) continue;
                uint16_t device = (vendor_device >> 16) & 0xFFFF;
                uint32_t class_reg = pci_read(bus, dev, func, 8);
                uint8_t class_code = (class_reg >> 24) & 0xFF;
                uint8_t subclass = (class_reg >> 16) & 0xFF;
                
                if(pci_device_count < MAX_PCI_DEVICES) {
                    pci_devices[pci_device_count].bus = bus;
                    pci_devices[pci_device_count].device = dev;
                    pci_devices[pci_device_count].function = func;
                    pci_devices[pci_device_count].vendor_id = vendor;
                    pci_devices[pci_device_count].device_id = device;
                    pci_devices[pci_device_count].class_code = class_code;
                    pci_devices[pci_device_count].subclass = subclass;
                    pci_device_count++;
                }
            }
        }
    }
    terminal_writestring("  [PCI] Found ");
    { char b[12]; int n=pci_device_count; int i=0; if(n==0)b[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[i++]=rev[--r];} b[i]=0; terminal_writestring(b); }
    terminal_writestring(" devices\n");
}

void pci_list(void) {
    terminal_writestring("\n [PCI] Devices:\n");
    for(int i=0;i<pci_device_count;i++) {
        terminal_writestring("  ");
        { char b[4]; int n=pci_devices[i].bus; b[0]='0'+n; b[1]=0; terminal_writestring(b); }
        terminal_writestring(":");
        { char b[4]; int n=pci_devices[i].device; char hex[]="0123456789ABCDEF"; b[0]=hex[n/16]; b[1]=hex[n%16]; b[2]=0; terminal_writestring(b); }
        terminal_writestring(".");
        { char b[4]; int n=pci_devices[i].function; b[0]='0'+n; b[1]=0; terminal_writestring(b); }
        terminal_writestring(" vendor=0x");
        { uint32_t v=pci_devices[i].vendor_id; char hex[]="0123456789ABCDEF"; char out[5]; out[0]=hex[(v>>12)&0xF]; out[1]=hex[(v>>8)&0xF]; out[2]=hex[(v>>4)&0xF]; out[3]=hex[v&0xF]; out[4]=0; terminal_writestring(out); }
        terminal_writestring(" device=0x");
        { uint32_t v=pci_devices[i].device_id; char hex[]="0123456789ABCDEF"; char out[5]; out[0]=hex[(v>>12)&0xF]; out[1]=hex[(v>>8)&0xF]; out[2]=hex[(v>>4)&0xF]; out[3]=hex[v&0xF]; out[4]=0; terminal_writestring(out); }
        terminal_writestring(" class=");
        { char b[4]; int n=pci_devices[i].class_code; char hex[]="0123456789ABCDEF"; b[0]=hex[n/16]; b[1]=hex[n%16]; b[2]=0; terminal_writestring(b); }
        terminal_writestring("\n");
    }
    terminal_writestring("\n");
}

pci_device_t* pci_find_class(uint8_t class_code, uint8_t subclass) {
    for(int i=0;i<pci_device_count;i++) {
        if(pci_devices[i].class_code==class_code && (subclass==0xFF || pci_devices[i].subclass==subclass)) return &pci_devices[i];
    }
    return 0;
}
