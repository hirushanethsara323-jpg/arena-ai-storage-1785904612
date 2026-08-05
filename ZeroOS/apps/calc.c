#include "calc.h"
#include "../gui/compositor.h"
#include "../gui/fb.h"
#include "../gui/font.h"
#include "../heap.h"

extern void terminal_writestring(const char*);

void calc_app_entry(void) {
    terminal_writestring("\n [Calc] Calculator app launched\n");
    int win = compositor_create_window("Calculator", 300, 200, 300, 400);
    if(win<0) return;
    terminal_writestring(" [Calc] Window created, use shell 'calc' for now\n");
}

// Shell calc
int calc_eval(const char* expr) {
    // Simple eval: supports single op like 2+2, 10-5, etc.
    int a=0,b=0;
    char op=0;
    int i=0;
    while(expr[i]>='0'&&expr[i]<='9'){ a=a*10+(expr[i]-'0'); i++; }
    while(expr[i]==' ') i++;
    op=expr[i++];
    while(expr[i]==' ') i++;
    while(expr[i]>='0'&&expr[i]<='9'){ b=b*10+(expr[i]-'0'); i++; }
    switch(op){
        case '+': return a+b;
        case '-': return a-b;
        case '*': return a*b;
        case '/': return b? a/b : 0;
        default: return a;
    }
}
