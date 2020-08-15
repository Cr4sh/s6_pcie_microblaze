.code

public lock_aquire
public lock_release


lock_aquire:

    xor     rax, rax
    inc     rax
    xchg    rax, [rcx]
    test    rax, rax
    jnz     lock_aquire
    ret


lock_release:

    xor     rax, rax
    xchg    rax, [rcx]
    ret


end
