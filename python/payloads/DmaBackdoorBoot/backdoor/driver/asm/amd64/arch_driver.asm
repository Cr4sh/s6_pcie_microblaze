.code

public clear_wp
public set_wp
public _ZwProtectVirtualMemory

extern m_SDT_NtProtectVirtualMemory:dword
extern _KiServiceInternal:qword


clear_wp:

    push    rax                 
    mov     rax, cr0             
    and     eax, not 000010000h
    mov     cr0, rax
    pop     rax
    ret


set_wp:

    push    rax
    mov     rax, cr0
    or      eax, 000010000h
    mov     cr0, rax
    pop     rax
    ret


_KiServiceLinkage:

    ret


_ZwProtectVirtualMemory:

    mov     rax, rsp
    cli
    sub     rsp, 10h
    push    rax
    pushfq
    push    10h
    lea     rax, _KiServiceLinkage
    push    rax
    mov     rax, _KiServiceInternal
    push    rax
    mov     eax, m_SDT_NtProtectVirtualMemory
    ret       


END
