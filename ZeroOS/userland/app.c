#include "app.h"
#include "../heap.h"

extern void terminal_writestring(const char*);
extern void terminal_writestring(const char*);

static app_t apps[MAX_APPS];
static int app_count = 0;

static void strcpy_a(char* d, const char* s){ while((*d++=*s++)); }
static int strcmp_a(const char* a, const char* b){ while(*a&&*b&&*a==*b){a++;b++;} return *a-*b; }

void app_init(void) {
    for(int i=0;i<MAX_APPS;i++) apps[i].used=0;
    app_count=0;
}

int app_register(const char* name, const char* desc, void (*entry)(void), uint32_t icon, uint32_t color) {
    for(int i=0;i<MAX_APPS;i++) {
        if(!apps[i].used) {
            apps[i].used=1;
            int j=0; while(name[j] && j<APP_NAME_MAX-1){ apps[i].name[j]=name[j]; j++; } apps[i].name[j]=0;
            j=0; while(desc[j] && j<63){ apps[i].desc[j]=desc[j]; j++; } apps[i].desc[j]=0;
            apps[i].entry = entry;
            apps[i].icon = icon;
            apps[i].color = color;
            app_count++;
            return i;
        }
    }
    return -1;
}

void app_list(void) {
    terminal_writestring("\n [App Store] ");
    { char b[12]; int n=app_count; int i=0; if(n==0)b[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)b[i++]=rev[--r];} b[i]=0; terminal_writestring(b); }
    terminal_writestring(" apps:\n\n");
    for(int i=0;i<MAX_APPS;i++) {
        if(apps[i].used) {
            terminal_writestring("  ");
            // icon
            terminal_writestring("[");
            { char ic[2]; ic[0]=apps[i].icon ? apps[i].icon : '*'; ic[1]=0; terminal_writestring(ic); }
            terminal_writestring("] ");
            terminal_writestring(apps[i].name);
            terminal_writestring(" - ");
            terminal_writestring(apps[i].desc);
            terminal_writestring("\n");
        }
    }
    terminal_writestring("\n");
}

int app_launch(const char* name) {
    for(int i=0;i<MAX_APPS;i++) {
        if(apps[i].used && strcmp_a(apps[i].name, name)==0) {
            terminal_writestring("\n [App] Launching ");
            terminal_writestring(name);
            terminal_writestring("...\n");
            if(apps[i].entry) apps[i].entry();
            terminal_writestring("\n [App] ");
            terminal_writestring(name);
            terminal_writestring(" exited\n\n");
            return 0;
        }
    }
    terminal_writestring("\n [App] Not found: ");
    terminal_writestring(name);
    terminal_writestring("\n\n");
    return -1;
}

void app_store(void) {
    terminal_writestring("\n");
    terminal_writestring("  ╔════════════════════════════════════════╗\n");
    terminal_writestring("  ║           ZERO APP STORE v1.0           ║\n");
    terminal_writestring("  ╠════════════════════════════════════════╣\n");
    terminal_writestring("  ║  • Terminal  - Zero Shell              ║\n");
    terminal_writestring("  ║  • Files     - ZeroFS Browser          ║\n");
    terminal_writestring("  ║  • Editor    - Zero Edit               ║\n");
    terminal_writestring("  ║  • Browser   - Void Browser            ║\n");
    terminal_writestring("  ║  • Settings  - Zero Settings           ║\n");
    terminal_writestring("  ║  • AI        - Zero AI Assistant       ║\n");
    terminal_writestring("  ║  • Store     - This store              ║\n");
    terminal_writestring("  ╚════════════════════════════════════════╝\n");
    terminal_writestring("\n  Type 'apps' to list, 'launch <name>' to run\n\n");
}
