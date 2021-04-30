.code

public _sidt
public _int_3

extern pIdt:qword


_sidt:

    mov     rax, [pIdt]
    sidt    fword ptr [rax]
    ret


_int_3:

    int     3
    ret
    

END
