
#define _ALIGN_DOWN(x, align)(x &~ (align - 1))
#define _ALIGN_UP(x, align)((x & (align - 1)) ? _ALIGN_DOWN(x, align) + align : x)

#define RVATOVA(_base_, _offset_) ((PUCHAR)(_base_) + (ULONG)(_offset_))


#define ABSOLUTE(wait) (wait)
#define RELATIVE(wait) (-(wait))


#define NANOSECONDS(nanos)      \
    (((signed __int64)(nanos)) / 100L)

#define MICROSECONDS(micros)    \
    (((signed __int64)(micros)) * NANOSECONDS(1000L))

#define MILLISECONDS(milli)     \
    (((signed __int64)(milli)) * MICROSECONDS(1000L))

#define SECONDS(seconds)        \
    (((signed __int64)(seconds)) * MILLISECONDS(1000L))


#define IFMT32 "0x%.8x"
#define IFMT64 "0x%.16I64x"

#define IFMT32_W L"0x%.8x"
#define IFMT64_W L"0x%.16I64x"

#ifdef _X86_

#define IFMT IFMT32
#define IFMT_W IFMT32_W

#elif _AMD64_

#define IFMT IFMT64
#define IFMT_W IFMT64_W

#endif
