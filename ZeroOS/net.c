#include "net.h"
#include "pci.h"

extern void terminal_writestring(const char*);

static int net_found = 0;

void net_init(void) {
    terminal_writestring("  [NET] Scanning for network cards...\n");
    pci_device_t* dev = pci_find_class(0x02, 0xFF); // network class
    if(dev) {
        terminal_writestring("  [NET] Found NIC vendor=0x");
        { uint32_t v=dev->vendor_id; char hex[]="0123456789ABCDEF"; char out[5]; out[0]=hex[(v>>12)&0xF]; out[1]=hex[(v>>8)&0xF]; out[2]=hex[(v>>4)&0xF]; out[3]=hex[v&0xF]; out[4]=0; terminal_writestring(out); }
        terminal_writestring(" (NE2000/E1000 compatible stub)\n");
        net_found = 1;
    } else {
        terminal_writestring("  [NET] No NIC found (use QEMU -net nic model=e1000)\n");
    }
}

int net_send(const char* data, uint32_t len) {
    (void)data; (void)len;
    if(!net_found) return -1;
    terminal_writestring("  [NET] Send stub\n");
    return len;
}

int net_receive(char* buffer, uint32_t max) {
    (void)buffer; (void)max;
    if(!net_found) return -1;
    return 0;
}

void net_list(void) {
    terminal_writestring("\n [NET] Status: ");
    terminal_writestring(net_found ? "NIC found (stub)\n" : "No NIC\n");
    terminal_writestring("  Stack: TCP/IP minimal (ARP, IP, ICMP, UDP stub)\n\n");
}
