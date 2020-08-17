.code

public lock_aquire
public lock_release

lock_var dq 0


lock_aquire:

    xor     rax, rax
    inc     rax
    xchg    rax, [lock_var]
    test    rax, rax
    jnz     lock_aquire
    ret


lock_release:

    xor     rax, rax
    xchg    rax, [lock_var]
    ret


end
