code

public GetPeb


GetPeb:

    mov     rax, gs:[30h]
    mov     rax, [rax + 60h]    	
    ret


END
