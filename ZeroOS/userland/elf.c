#include "elf.h"
#include "../heap.h"
#include "../pmm.h"

extern void terminal_writestring(const char*);

int elf_check(elf_header_t* header) {
    if(header->e_ident[0] != ELF_MAGIC0) return -1;
    if(header->e_ident[1] != ELF_MAGIC1) return -1;
    if(header->e_ident[2] != ELF_MAGIC2) return -1;
    if(header->e_ident[3] != ELF_MAGIC3) return -1;
    if(header->e_ident[4] != ELF_CLASS_32) return -1;
    if(header->e_type != ELF_EXEC) return -1;
    if(header->e_machine != ELF_386) return -1;
    return 0;
}

int elf_load(elf_header_t* header) {
    if(elf_check(header) != 0) {
        terminal_writestring("\n [ELF] Invalid magic\n");
        return -1;
    }

    terminal_writestring("\n [ELF] Loading ");
    {
        char buf[16]; uint32_t n=header->e_phnum; int i=0; if(n==0)buf[i++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)buf[i++]=rev[--r];} buf[i]=0;
        terminal_writestring(buf);
        terminal_writestring(" program headers\n");
    }

    elf_program_header_t* ph = (elf_program_header_t*)((uint32_t)header + header->e_phoff);
    for(uint32_t i=0;i<header->e_phnum;i++) {
        if(ph[i].p_type == 1) { // PT_LOAD
            terminal_writestring("  [ELF] LOAD vaddr=0x");
            {
                uint32_t addr = ph[i].p_vaddr;
                char hex[]="0123456789ABCDEF";
                char out[9]; for(int j=0;j<8;j++){ out[7-j]=hex[addr&0xF]; addr>>=4; } out[8]=0;
                terminal_writestring(out);
                terminal_writestring(" filesz=");
                char buf[16]; uint32_t n=ph[i].p_filesz; int ii=0; if(n==0)buf[ii++]='0'; else {char rev[12];int r=0;while(n>0){rev[r++]='0'+(n%10);n/=10;}while(r>0)buf[ii++]=rev[--r];} buf[ii]=0; terminal_writestring(buf);
                terminal_writestring("\n");
            }

            // In real OS with paging, we'd allocate frames and map to p_vaddr
            // For now, allocate blocks for memsz and copy
            uint32_t blocks_needed = (ph[i].p_memsz + 4095)/4096;
            for(uint32_t b=0;b<blocks_needed;b++) {
                uint32_t* frame = pmm_alloc_block();
                if(!frame) {
                    terminal_writestring("  [ELF] OOM!\n");
                    return -1;
                }
                // zero frame would be done
            }
            // memcpy from file offset to vaddr (in real would be mapped)
            // For sandbox, we skip actual copy to arbitrary address to avoid fault
        }
    }

    terminal_writestring("  [ELF] Entry=0x");
    {
        uint32_t e = header->e_entry;
        char hex[]="0123456789ABCDEF";
        char out[9]; for(int j=0;j<8;j++){ out[7-j]=hex[e&0xF]; e>>=4; } out[8]=0;
        terminal_writestring(out);
        terminal_writestring("\n");
    }

    return 0;
}
