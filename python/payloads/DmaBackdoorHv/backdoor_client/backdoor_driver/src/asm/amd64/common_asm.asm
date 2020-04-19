.code

public _cr4_get
public _cr4_set
public _msr_get
public _msr_set


_cr4_get:

    mov     rax, cr4    
    ret


_cr4_set:

    and     rcx, 0FFFFFFFFh
    mov     cr4, rcx
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
