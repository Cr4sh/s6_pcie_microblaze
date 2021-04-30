#include "stdafx.h"

#ifdef _X86_

#pragma pack(1)

typedef struct _SIDT
{
    USHORT limit;
    ULONG base;

} SIDT,
*PSIDT;

#pragma pack()

#pragma pack(1)

typedef struct _IDT_ENTRY
{
    USHORT low_offset;
    USHORT segment_selector;
    USHORT access;
    USHORT high_offset;

} IDT_ENTRY,
*PIDT_ENTRY;

#pragma pack()

#elif _AMD64_

#pragma pack(1)

typedef struct _SIDT
{
    USHORT limit;
    ULONGLONG base;

} SIDT,
*PSIDT;

#pragma pack()

#pragma pack(1)

typedef struct _IDT_ENTRY
{
    USHORT low_offset;
    USHORT segment_selector;
    USHORT access;
    USHORT high_offset;
    ULONG  offset32;
    ULONG  reserved;

} IDT_ENTRY,
*PIDT_ENTRY;

#pragma pack()

#endif

SIDT Idt;
extern "C" PSIDT pIdt = &Idt;

extern PVOID m_KernelBase;
//--------------------------------------------------------------------------------------
#ifdef _X86_
//--------------------------------------------------------------------------------------
__declspec(naked) PEXCEPTION_REGISTRATION NTAPI _get_exception_pointer(void)
{
    __asm
    {
        mov     eax, fs:[0]
        ret
    }
}
//--------------------------------------------------------------------------------------
#endif // _X86_
//--------------------------------------------------------------------------------------
PVOID NTAPI ImportGetKernelBase(void)
{
    PVOID Base = NULL;

    if (m_KernelBase)
    {
        return m_KernelBase;
    }

#ifdef _X86_

    // get IDT limit and base
    __asm sidt Idt;

#elif _AMD64_

    // get IDT limit and base
    _sidt();

#endif

    // get pointer to debug interrupt descriptor
    PIDT_ENTRY IdtEntry = (PIDT_ENTRY)(Idt.base + 1 * sizeof(IDT_ENTRY));        

    LARGE_INTEGER DebugTrapVector;
    DebugTrapVector.QuadPart = 0;

    // get debug interrupt vector address

#ifdef _X86_

    DebugTrapVector.HighPart = 0;

#elif _AMD64_

    DebugTrapVector.HighPart = IdtEntry->offset32;

#endif

    DebugTrapVector.LowPart  = IdtEntry->low_offset | (IdtEntry->high_offset << 16);    

    Base = (PVOID)_ALIGN_DOWN(DebugTrapVector.QuadPart, PAGE_SIZE);

    // get module base by address inside of it
    while (*(PUSHORT)Base != IMAGE_DOS_SIGNATURE)
    {
        Base = (PVOID)((PUCHAR)Base - PAGE_SIZE);
    }

    m_KernelBase = Base;

    return Base;
}
//--------------------------------------------------------------------------------------
PVOID NTAPI ImportGetProcAddress(ULONG Module, ULONG Hash)
{
    PVOID ModuleBase = NULL;

    // get module address by id
    switch (Module)
    {
    case I_MODULE_NT: 

        // get address of function from ntoskrnl.exe
        ModuleBase = ImportGetKernelBase();
        break;

    case I_MODULE_HAL: 

        // get address of function from hal.dll
        ModuleBase = RuntimeGetModuleBase("hal.dll");
        break;
    }

    if (ModuleBase)
    {
        return ImportGetProcAddressEx(ModuleBase, Hash);
    }

    return NULL;
}
//--------------------------------------------------------------------------------------
// EoF
