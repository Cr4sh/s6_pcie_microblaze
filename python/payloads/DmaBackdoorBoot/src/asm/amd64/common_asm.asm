.code

public get_addr
public _ExitBootServices

; handler and return address for ExitBootServices() hook
extern ret_ExitBootServices:qword
extern new_ExitBootServices:proc


get_addr:

    call    _lb
    
_lb:

    pop     rax
    ret


_ExitBootServices:
 
    ; save return address into the global variable
    mov     rax, [rsp]
    mov     ret_ExitBootServices, rax

    ; jump to the hook handler
    jmp     new_ExitBootServices


end
