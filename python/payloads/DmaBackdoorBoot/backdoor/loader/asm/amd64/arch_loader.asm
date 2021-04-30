code

public ep_entry

extern MainProc:proc


ep_entry:

    sub     rsp, 28h
    call    MainProc   
    add     rsp, 28h
    test    rax, rax
    jz      _ret
    push    rax

_ret:

    ret


end
