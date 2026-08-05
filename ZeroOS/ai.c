#include "ai.h"
#include "fs/vfs.h"

extern void terminal_writestring(const char*);

static const char* knowledge[][2] = {
    {"what is zero os", "Zero OS is a real OS from scratch, Zero Bloat, Zero Tracking, Zero Limits, built in Arena sandbox"},
    {"who are you", "I am Zero AI, local offline assistant built into Zero OS kernel v2.0, no cloud, zero tracking"},
    {"kernel size", "v1.2 59K, v2.0 now 65K+ with USB HID AC97 FAT32 TCP/IP AI"},
    {"features", "Boot GDT IDT PMM Heap VFS ZeroFS ZeroFS2 FAT32 FB Mouse Compositor PIT Tasking ELF Syscalls Paging App Store SMP PCI USB NET Speaker AC97 AI"},
    {"help", "Commands: help, ls, cat, mem, ps, apps, launch, store, ai <question>, play, ping, pci, usb, net"},
    {"who created", "Created by you and Arena AI in Hillsboro Oregon, built from zero"},
    {"zero", "Zero is circle, perfect, infinite, start from zero"},
    {0,0}
};

static int strcmp_ai(const char* a, const char* b) {
    // case insensitive contains
    while(*a && *b) {
        char ca = *a; if(ca>='A'&&ca<='Z') ca+=32;
        char cb = *b; if(cb>='A'&&cb<='Z') cb+=32;
        if(ca!=cb) return ca-cb;
        a++; b++;
    }
    return 0;
}

static int str_contains(const char* hay, const char* needle) {
    // simple case-insensitive contains
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
    terminal_writestring("  [ZeroAI] Local model loaded (rule-based 7 facts, offline)\n");
}

void ai_query(const char* question, char* answer, uint32_t max_len) {
    (void)max_len;
    for(int i=0; knowledge[i][0]; i++) {
        if(str_contains(question, knowledge[i][0])) {
            int j=0; while(knowledge[i][1][j] && j<(int)max_len-1){ answer[j]=knowledge[i][1][j]; j++; } answer[j]=0;
            return;
        }
    }
    // default
    const char* def = "I am Zero AI v2.0, I know about Zero OS features, kernel, Zero Ring design. Ask: what is zero os, features, kernel size, who created";
    int j=0; while(def[j] && j<(int)max_len-1){ answer[j]=def[j]; j++; } answer[j]=0;
}

void ai_chat(const char* input) {
    char answer[256];
    ai_query(input, answer, 256);
    terminal_writestring("\n [ZeroAI] ");
    terminal_writestring(answer);
    terminal_writestring("\n\n");
}
