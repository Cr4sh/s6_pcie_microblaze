
#define IMPORT_MAX_STRING_SIZE 255

#define ImportGetModuleBase RuntimeGetModuleBase

extern "C"
{
PVOID NTAPI GetKernelBase(void);
PVOID NTAPI ImportGetProcAddress(ULONG Module, ULONG Hash);
};

template <ULONG h, ULONG hash>
inline PVOID pushargEx()
{	
    typedef PVOID (NTAPI * newfunc)();
    newfunc func = (newfunc)ImportGetProcAddress(h, hash);
    return func();
}

template <ULONG h, ULONG hash, class A>
inline PVOID pushargEx(A a1)
{	
    typedef PVOID (NTAPI * newfunc)(A);
    newfunc func = (newfunc)ImportGetProcAddress(h, hash);
    return func(a1);
}

template <ULONG h, ULONG hash, class A, class B>
inline PVOID pushargEx(A a1, B a2)
{	
    typedef PVOID (NTAPI * newfunc)(A, B);
    newfunc func = (newfunc)ImportGetProcAddress(h, hash);
    return func(a1, a2);
}

template <ULONG h, ULONG hash, class A, class B, class C>
inline PVOID pushargEx(A a1, B a2, C a3)
{
    typedef PVOID (NTAPI * newfunc)(A, B, C);
    newfunc func = (newfunc)ImportGetProcAddress(h, hash);
    return func(a1, a2, a3);
}

template <ULONG h, ULONG hash, class A, class B, class C, class D>
inline PVOID pushargEx(A a1, B a2, C a3, D a4)
{	
    typedef PVOID (NTAPI * newfunc)(A, B, C, D);
    newfunc func = (newfunc)ImportGetProcAddress(h, hash);
    return func(a1, a2, a3, a4);
}

template <ULONG h, ULONG hash, class A, class B, class C, class D, class E>
inline PVOID pushargEx(A a1, B a2, C a3, D a4, E a5)
{	
    typedef PVOID (NTAPI * newfunc)(A, B, C, D, E);
    newfunc func = (newfunc)ImportGetProcAddress(h, hash);
    return func(a1, a2, a3, a4, a5);
}

template <ULONG h, ULONG hash, class A, class B, class C, class D, class E, class F>
inline PVOID pushargEx(A a1, B a2, C a3, D a4, E a5, F a6)
{	
    typedef PVOID (NTAPI * newfunc)(A, B, C, D, E, F);
    newfunc func = (newfunc)ImportGetProcAddress(h, hash);
    return func(a1, a2, a3, a4, a5, a6);
}

template <ULONG h, ULONG hash, class A, class B, class C, class D, class E, class F, class G>
PVOID pushargEx(A a1, B a2, C a3, D a4, E a5, F a6, G a7)
{	
    typedef PVOID (NTAPI * newfunc)(A, B, C, D, E, F, G);
    newfunc func = (newfunc)ImportGetProcAddress(h, hash);
    return func(a1, a2, a3, a4, a5, a6, a7);
}

template <ULONG h, ULONG hash, class A, class B, class C, class D, class E, class F, class G, class H>
inline PVOID pushargEx(A a1, B a2, C a3, D a4, E a5, F a6, G a7, H a8)
{	
    typedef PVOID (NTAPI * newfunc)(A, B, C, D, E, F, G, H);
    newfunc func = (newfunc)ImportGetProcAddress(h, hash);
    return func(a1, a2, a3, a4, a5, a6, a7, a8);
}

template <ULONG h, ULONG hash, class A, class B, class C, class D, class E, class F, class G, class H, class I>
inline PVOID pushargEx(A a1, B a2, C a3, D a4, E a5, F a6, G a7, H a8, I a9)
{	
    typedef PVOID (NTAPI * newfunc)(A, B, C, D, E, F, G, H, I);
    newfunc func = (newfunc)ImportGetProcAddress(h, hash);
    return func(a1, a2, a3, a4, a5, a6, a7, a8, a9);
}

template <ULONG h, ULONG hash, class A, class B, class C, class D, class E, class F, class G, class H, class I, class X>
inline PVOID pushargEx(A a1, B a2, C a3, D a4, E a5, F a6, G a7, H a8, I a9, X a10)
{	
    typedef PVOID (NTAPI * newfunc)(A, B, C, D, E, F, G, H, I, X);
    newfunc func = (newfunc)ImportGetProcAddress(h, hash);
    return func(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

template <ULONG h, ULONG hash, class A, class B, class C, class D, class E, class F, class G, class H, class I, class X, class Y>
inline PVOID pushargEx(A a1, B a2, C a3, D a4, E a5, F a6, G a7, H a8, I a9, X a10, Y a11)
{	
    typedef PVOID (NTAPI * newfunc)(A, B, C, D, E, F, G, H, I, X, Y);
    newfunc func = (newfunc)ImportGetProcAddress(h, hash);
    return func(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
}

template <ULONG h, ULONG hash, class A, class B, class C, class D, class E, class F, class G, class H, class I, class X, class Y, class Z, class K>
inline PVOID pushargEx(A a1, B a2, C a3, D a4, E a5, F a6, G a7, H a8, I a9, X a10, Y a11, Z a12, K a13)
{	
    typedef PVOID (NTAPI * newfunc)(A, B, C, D, E, F, G, H, I, X, Y, Z, K);
    newfunc func = (newfunc)ImportGetProcAddress(h, hash);
    return func(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
}

