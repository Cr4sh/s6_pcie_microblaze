
#define IMPORT_MAX_STRING_SIZE 255

extern "C"
{

ULONG NTAPI ImportHash(char *Str);
PVOID NTAPI ImportGetProcAddressEx(PVOID Image, ULONG Hash);

}

template <ULONG Module, ULONG Hash>
inline PVOID ImportFunc()
{
    typedef PVOID (NTAPI * IMPORT_FUNC)();

    IMPORT_FUNC Func = (IMPORT_FUNC)ImportGetProcAddress(Module, Hash);

    return Func();
}

template <ULONG Module, ULONG Hash, class A_1>
inline PVOID ImportFunc(A_1 a_1)
{   
    typedef PVOID (NTAPI * IMPORT_FUNC)(A_1);
    
    IMPORT_FUNC Func = (IMPORT_FUNC)ImportGetProcAddress(Module, Hash);

    return Func(a_1);
}

template <ULONG Module, ULONG Hash, class A_1, class A_2>
inline PVOID ImportFunc(A_1 a_1, A_2 a_2)
{   
    typedef PVOID (NTAPI * IMPORT_FUNC)(A_1, A_2);
    
    IMPORT_FUNC Func = (IMPORT_FUNC)ImportGetProcAddress(Module, Hash);

    return Func(a_1, a_2);
}

template <ULONG Module, ULONG Hash, class A_1, class A_2, class A_3>
inline PVOID ImportFunc(A_1 a_1, A_2 a_2, A_3 a_3)
{   
    typedef PVOID (NTAPI * IMPORT_FUNC)(A_1, A_2, A_3);
    
    IMPORT_FUNC Func = (IMPORT_FUNC)ImportGetProcAddress(Module, Hash);

    return Func(a_1, a_2, a_3);
}

template <ULONG Module, ULONG Hash, class A_1, class A_2, class A_3, class A_4>
inline PVOID ImportFunc(A_1 a_1, A_2 a_2, A_3 a_3, A_4 a_4)
{   
    typedef PVOID (NTAPI * IMPORT_FUNC)(A_1, A_2, A_3, A_4);
    
    IMPORT_FUNC Func = (IMPORT_FUNC)ImportGetProcAddress(Module, Hash);

    return Func(a_1, a_2, a_3, a_4);
}

template <ULONG Module, ULONG Hash, class A_1, class A_2, class A_3, class A_4, class A_5>
inline PVOID ImportFunc(A_1 a_1, A_2 a_2, A_3 a_3, A_4 a_4, A_5 a_5)
{   
    typedef PVOID (NTAPI * IMPORT_FUNC)(A_1, A_2, A_3, A_4, A_5);
    
    IMPORT_FUNC Func = (IMPORT_FUNC)ImportGetProcAddress(Module, Hash);

    return Func(a_1, a_2, a_3, a_4, a_5);
}

template <ULONG Module, ULONG Hash, class A_1, class A_2, class A_3, class A_4, class A_5, class A_6>
inline PVOID ImportFunc(A_1 a_1, A_2 a_2, A_3 a_3, A_4 a_4, A_5 a_5, A_6 a_6)
{   
    typedef PVOID (NTAPI * IMPORT_FUNC)(A_1, A_2, A_3, A_4, A_5, A_6);
    
    IMPORT_FUNC Func = (IMPORT_FUNC)ImportGetProcAddress(Module, Hash);

    return Func(a_1, a_2, a_3, a_4, a_5, a_6);
}

template <ULONG Module, ULONG Hash, class A_1, class A_2, class A_3, class A_4, class A_5, class A_6, class A_7>
inline PVOID ImportFunc(A_1 a_1, A_2 a_2, A_3 a_3, A_4 a_4, A_5 a_5, A_6 a_6, A_7 a_7)
{   
    typedef PVOID (NTAPI * IMPORT_FUNC)(A_1, A_2, A_3, A_4, A_5, A_6, A_7);
    
    IMPORT_FUNC Func = (IMPORT_FUNC)ImportGetProcAddress(Module, Hash);

    return Func(a_1, a_2, a_3, a_4, a_5, a_6, a_7);
}

template <ULONG Module, ULONG Hash, class A_1, class A_2, class A_3, class A_4, class A_5, class A_6, class A_7, class A_8>
inline PVOID ImportFunc(A_1 a_1, A_2 a_2, A_3 a_3, A_4 a_4, A_5 a_5, A_6 a_6, A_7 a_7, A_8 a_8)
{   
    typedef PVOID (NTAPI * IMPORT_FUNC)(A_1, A_2, A_3, A_4, A_5, A_6, A_7, A_8);
    
    IMPORT_FUNC Func = (IMPORT_FUNC)ImportGetProcAddress(Module, Hash);

    return Func(a_1, a_2, a_3, a_4, a_5, a_6, a_7, a_8);
}

template <ULONG Module, ULONG Hash, class A_1, class A_2, class A_3, class A_4, class A_5, class A_6, class A_7, class A_8, class A_9>
inline PVOID ImportFunc(A_1 a_1, A_2 a_2, A_3 a_3, A_4 a_4, A_5 a_5, A_6 a_6, A_7 a_7, A_8 a_8, A_9 a_9)
{
    typedef PVOID (NTAPI * IMPORT_FUNC)(A_1, A_2, A_3, A_4, A_5, A_6, A_7, A_8, A_9);
    
    IMPORT_FUNC Func = (IMPORT_FUNC)ImportGetProcAddress(Module, Hash);

    return Func(a_1, a_2, a_3, a_4, a_5, a_6, a_7, a_8, a_9);
}
