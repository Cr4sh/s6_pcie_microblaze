.code

public get_addr
public _OpenProtocol
public _ExitBootServices

; handler and return address for OpenProtocol() hook
extern ret_OpenProtocol:qword
extern new_OpenProtocol:proc

; handler and return address for ExitBootServices() hook
extern ret_ExitBootServices:qword
extern new_ExitBootServices:proc


get_addr:

    call    _lb
    
_lb:

    pop     rax
    ret


_OpenProtocol:
 
    ; save return address into the global variable
    mov     rax, [rsp]
    mov     ret_OpenProtocol, rax

    ; jump to the hook handler
    jmp     new_OpenProtocol


_ExitBootServices:
 
    ; save return address into the global variable
    mov     rax, [rsp]
    mov     ret_ExitBootServices, rax

    ; jump to the hook handler
    jmp     new_ExitBootServices


end
