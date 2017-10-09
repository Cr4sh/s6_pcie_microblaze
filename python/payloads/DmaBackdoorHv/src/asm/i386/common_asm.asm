.386p

_TEXT$00 SEGMENT DWORD PUBLIC 'CODE'

public _get_addr

_get_addr:

    call    _lb
    
_lb:

    pop     eax
    ret

_TEXT$00 ends

end
