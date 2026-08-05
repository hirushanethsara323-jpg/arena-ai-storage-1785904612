/* ZeroOS Boot - Minimal 64-bit compatible for sandbox build */
/* For real 32-bit boot with GDT/IDT flush, see boot_real.S */

.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.code64
.global _start
.type _start, @function
_start:
    /* Setup stack - use rsp for 64-bit */
    movabs $stack_top, %rsp
    /* Pass multiboot info: rdi = magic, rsi = info (System V ABI) */
    /* But also push for 32-bit compat calling convention used earlier */
    push %rbx
    push %rax
    call kernel_main
    cli
1:  hlt
    jmp 1b

/* Minimal stubs for sandbox - real implementations in boot_real.S for cross-compiler */
.global gdt_flush
.type gdt_flush, @function
gdt_flush:
    ret

.global idt_flush
.type idt_flush, @function
idt_flush:
    ret

/* Dummy ISRs - just halt, real handlers need 32-bit build */
.macro ISR_DUMMY num
.global isr\num
.type isr\num, @function
isr\num:
    cli
    hlt
    jmp isr\num
.endm

.macro IRQ_DUMMY num
.global irq\num
.type irq\num, @function
irq\num:
    cli
    hlt
    jmp irq\num
.endm

ISR_DUMMY 0
ISR_DUMMY 1
ISR_DUMMY 2
ISR_DUMMY 3
ISR_DUMMY 4
ISR_DUMMY 5
ISR_DUMMY 6
ISR_DUMMY 7
ISR_DUMMY 8
ISR_DUMMY 9
ISR_DUMMY 10
ISR_DUMMY 11
ISR_DUMMY 12
ISR_DUMMY 13
ISR_DUMMY 14
ISR_DUMMY 15
ISR_DUMMY 16
ISR_DUMMY 17
ISR_DUMMY 18
ISR_DUMMY 19
ISR_DUMMY 20
ISR_DUMMY 21
ISR_DUMMY 22
ISR_DUMMY 23
ISR_DUMMY 24
ISR_DUMMY 25
ISR_DUMMY 26
ISR_DUMMY 27
ISR_DUMMY 28
ISR_DUMMY 29
ISR_DUMMY 30
ISR_DUMMY 31

IRQ_DUMMY 0
IRQ_DUMMY 1
IRQ_DUMMY 2
IRQ_DUMMY 3
IRQ_DUMMY 4
IRQ_DUMMY 5
IRQ_DUMMY 6
IRQ_DUMMY 7
IRQ_DUMMY 8
IRQ_DUMMY 9
IRQ_DUMMY 10
IRQ_DUMMY 11
IRQ_DUMMY 12
IRQ_DUMMY 13
IRQ_DUMMY 14
IRQ_DUMMY 15
