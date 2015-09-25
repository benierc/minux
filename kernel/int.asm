extern isr_default_int, isr_clock_int, isr_kbd_int, isr_GP_exc
global _asm_default_int, _asm_irq_0, _asm_irq_1, _asm_exc_GP

%macro  PUSH_REGS 0
    pushad
    push ds
    push es
    push fs
    push gs
    push ebx
    mov bx, 0x10
    mov ds, bx
    pop ebx
%endmacro

%macro  POP_REGS 0
    pop gs
    pop fs
    pop es
    pop ds
    popad
%endmacro

_asm_default_int:
    PUSH_REGS
    call isr_default_int
    mov al, 0x20
    out 0x20,al
    POP_REGS
    iret

_asm_exc_GP:
    PUSH_REGS
    call isr_GP_exc
    POP_REGS
    add esp,4
    iret

_asm_irq_0:
    PUSH_REGS
    call isr_clock_int
    mov al, 0x20
    out 0x20, al
    POP_REGS
    iret

_asm_irq_1:
    PUSH_REGS
    call isr_kbd_int
    mov al, 0x20
    out 0x20, al
    POP_REGS
    iret
