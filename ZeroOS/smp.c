#include "smp.h"

extern void terminal_writestring(const char*);

static int cpu_count = 1;
static int apic_enabled = 0;

static void cpuid(uint32_t code, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx) {
    __asm__ volatile ("cpuid" : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx) : "a"(code));
}

void smp_init(void) {
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, &eax, &ebx, &ecx, &edx);
    // Check if HTT
    if(edx & (1<<28)) {
        // HTT bit
        int logical = (ebx >> 16) & 0xFF;
        if(logical>1) cpu_count = logical;
    }
    // Check APIC
    if(edx & (1<<9)) apic_enabled = 1;

    terminal_writestring("  [SMP] CPUID: ");
    { char b[12]; int n=cpu_count; int i=0; if(n==0)b[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[i++]=rev[--r];} b[i]=0; terminal_writestring(b); }
    terminal_writestring(" CPUs detected, APIC ");
    terminal_writestring(apic_enabled ? "enabled\n" : "disabled\n");

    if(apic_enabled) smp_apic_init();
}

void smp_apic_init(void) {
    // Enable APIC via MSR? For now stub
    // In real SMP, we'd parse MADT, enable LAPIC at 0xFEE00000
    terminal_writestring("  [APIC] LAPIC @0xFEE00000 (sim)\n");
}

int smp_get_cpu_count(void) { return cpu_count; }
uint32_t smp_get_lapic_id(void) { return 0; } // boot CPU
