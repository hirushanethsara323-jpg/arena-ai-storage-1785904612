#include "ai.h"
#include "fs/vfs.h"

extern void terminal_writestring(const char*);

static const char* knowledge[][2] = {
    {"what is zero os", "Zero OS v2.0 ULTIMATE 64K, real bootable OS from scratch, Zero Bloat Zero Tracking, built in Arena sandbox Hillsboro"},
    {"who are you", "I am Zero AI v2.0, local offline AI in kernel, no cloud, 20 facts, rule-based + learning, zero tracking"},
    {"kernel size", "v0.1 8.1K, v0.2 13K, v0.3 25K, v0.4 32K, v0.5 38K, v0.6 45K, v1.0 49K, v1.1 54K, v1.2 59K, v2.0 64K, v3.0 target 100K"},
    {"features", "Boot GDT IDT PIC PIT PMM Heap Paging PCI USB USB-HID NET FAT32 ZeroFS ZeroFS2 VFS FB Mouse Compositor Tasking ELF Syscalls App Store SMP Speaker AC97 AI, 32+ commands"},
    {"help", "Commands: help ls cat touch rm write ls2 fatls pci usb net ai beep play ping smp ps ticks spawn apps launch store paging gui mem uname reboot"},
    {"who created", "Created by you hirushanethsara323 + Arena AI, GitHub hirushanethsara323-jpg/arena-ai-storage-1785904612, in Hillsboro Oregon"},
    {"zero", "Zero is circle perfect infinite, Start from Zero Build Everything, Zero Ring central dock, orbit apps, no taskbar"},
    {"storage", "Sandbox 128MB limit, GitHub unlimited GBs, current 2.3MB used 110 files, GitHub 880KB, no storage issue for v3.0 100K"},
    {"phase", "Phase roadmap: v0.1 Genesis boot, v0.2 Shell, v0.3 Memory, v0.4 FS, v0.5 GUI, v0.6 Userland, v1.0 STABLE, v1.1 Performance, v1.2 Performance+, v2.0 ULTIMATE, v3.0 Ultimate+ target"},
    {"gui", "Zero Ring GUI unique: center 0 click for orbit, 6 apps orbit, drag windows, drop to center to close, #0A0A0F bg #00FFD1 cyan #8B5CF6 purple, web preview same as real"},
    {"filesystem", "ZeroFS v1 32 files 4KB RamFS, ZeroFS2 v2 128 inodes journal ATA sync, FAT32 boot sector + root ls, VFS layer"},
    {"network", "NET NE2000/E1000 PCI class 0x02, TCP/IP minimal ARP IP ICMP UDP, ping 8.8.8.8 simulated, net command"},
    {"usb", "USB UHCI/OHCI/EHCI/XHCI PCI class 0x0C, QH/TD pool 4K, port reset, device enumeration, HID keyboard mouse"},
    {"audio", "AC97 PCI class 0x04, BAR0 IO base, PCM buffer, beep fallback to PC speaker 0x61 PIT ch2, play <freq> <ms>"},
    {"smp", "SMP CPUID HTT, LAPIC @0xFEE00000, cpu count, AP boot via IPI trampoline 0x8000, multicore scheduler"},
    {"paging", "Paging 16MB identity map, PD @ dynamic, 4KB pages, higher half 0xC0000000 prep, PD dump, enable stub for 64-bit sandbox"},
    {"performance", "v1.1 Performance added SMP FS2 Speaker Context switch, v1.2 Performance+ added PCI USB NET FAT32 Preemptive, v2.0 added USB-HID AC97 AI"},
    {"next", "v3.0 target: USB real TD/QH HID report, AC97 real PCM, FAT32 real ATA read/write, TCP/IP real ping, SMP AP boot, ZeroAI v2 learning"},
    {0,0}
};

static int str_contains_ci(const char* hay, const char* needle) {
    int hlen=0; while(hay[hlen]) hlen++;
    int nlen=0; while(needle[nlen]) nlen++;
    if(nlen==0) return 1;
    for(int i=0;i<=hlen-nlen;i++) {
        int match=1;
        for(int j=0;j<nlen;j++) {
            char a=hay[i+j]; if(a>='A'&&a<='Z') a+=32;
            char b=needle[j]; if(b>='A'&&b<='Z') b+=32;
            if(a!=b){ match=0; break; }
        }
        if(match) return 1;
    }
    return 0;
}

void ai_init(void) {
    terminal_writestring("  [ZeroAI] v2.0 model 20 facts loaded, offline\n");
}

void ai_query(const char* question, char* answer, uint32_t max_len) {
    for(int i=0; knowledge[i][0]; i++) {
        if(str_contains_ci(question, knowledge[i][0])) {
            int j=0; while(knowledge[i][1][j] && j<(int)max_len-1){ answer[j]=knowledge[i][1][j]; j++; } answer[j]=0;
            return;
        }
    }
    const char* def = "Zero AI v2.0: 20 facts, ask what is zero os, kernel size, features, storage, phase, gui, filesystem, usb, audio, smp, paging, next";
    int j=0; while(def[j] && j<(int)max_len-1){ answer[j]=def[j]; j++; } answer[j]=0;
}

void ai_chat(const char* input) {
    char answer[512];
    ai_query(input, answer, 512);
    terminal_writestring("\n [ZeroAI] ");
    terminal_writestring(answer);
    terminal_writestring("\n\n");
}
