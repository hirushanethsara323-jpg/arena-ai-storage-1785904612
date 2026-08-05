/* Zero OS Context Switch - Real Task Switching */
.section .text
.code64

.global context_switch
.type context_switch, @function
context_switch:
    # rdi = old_esp pointer, rsi = new_esp
    # Save old context
    push %rbp
    push %rbx
    push %r12
    push %r13
    push %r14
    push %r15
    pushfq
    mov %rsp, (%rdi)  # save old esp
    mov %rsi, %rsp    # load new esp
    popfq
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbx
    pop %rbp
    ret

.global context_switch_first
.type context_switch_first, @function
context_switch_first:
    # rdi = new_esp
    mov %rdi, %rsp
    popfq
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbx
    pop %rbp
    ret

/* 32-bit version for boot_real.S build */
.code32
.global context_switch_32
.type context_switch_32, @function
context_switch_32:
    mov 4(%esp), %eax  # old_esp pointer
    mov 8(%esp), %edx  # new_esp
    push %ebp
    push %ebx
    push %esi
    push %edi
    pushf
    mov %esp, (%eax)
    mov %edx, %esp
    popf
    pop %edi
    pop %esi
    pop %ebx
    pop %ebp
    ret
