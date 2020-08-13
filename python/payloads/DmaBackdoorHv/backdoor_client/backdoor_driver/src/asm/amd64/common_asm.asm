.code

public _cr0_get
public _cr0_set
public _msr_get
public _msr_set


_cr0_get:

    mov     rax, cr0
    ret


_cr0_set:

    and     rcx, 0FFFFFFFFh
    mov     cr0, rcx
    ret


_msr_get:
  
    push    rdx    
    rdmsr
    shr     rdx, 32
    or      rax, rdx
    pop     rdx
    ret
   

_msr_set:

    push    rax
    push    rdx
    mov     rax, rdx
    and     rax, 0FFFFFFFFh
    shl     rdx, 32
    wrmsr
    pop     rdx
    pop     rax
    ret

end
