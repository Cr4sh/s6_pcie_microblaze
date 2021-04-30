#include "stdafx.h"

PDRIVER_OBJECT m_DriverObject = NULL;
PDEVICE_OBJECT m_DeviceObject = NULL;

UNICODE_STRING m_usDeviceName;
UNICODE_STRING m_usDeviceLink;
//--------------------------------------------------------------------------------------
#define PCI_ENABLE 0x80000000

// make PCI device id from bus, device and function
#define PCI_DEVICE(_bus_, _dev_, _func_) \
                                         \
    (ULONG)(((_bus_) << 16) | ((_dev_) << 11) | ((_func_) << 8))

// make PCI config space address from device id and offset
#define PCI_ADDR(_dev_id_, _off_) \
                                  \
    (ULONG)((_dev_id_) | ((_off_) & 0xfc) | PCI_ENABLE)

// PCI config space registers
#define PCI_CFG_ID      0x00
#define PCI_CFG_COMMAND 0x04
#define PCI_CFG_STATUS  0x06
#define PCI_CFG_BAR0    0x10

// PCI command register flags
#define PCI_CMD_IO      0x0001
#define PCI_CMD_MEM     0x0002

// I/O ports for legacy PCI config space access
#define PCI_PORT_ADDR   0x0cf8
#define PCI_PORT_DATA   0x0cfc

#define PCI_DEVICE_NOT_FOUND (ULONG)(-1)

ULONG PciRead(ULONG Device, ULONG Offset)
{
    KIRQL OldIrql;
    KeRaiseIrql(DISPATCH_LEVEL, &OldIrql);

    // write address
    __outdword(PCI_PORT_ADDR, PCI_ADDR(Device, Offset));

    // read data
    ULONG Data = __indword(PCI_PORT_DATA);

    KeLowerIrql(OldIrql);

    return Data;
}

void PciWrite(ULONG Device, ULONG Offset, USHORT Val)
{
    KIRQL OldIrql;
    KeRaiseIrql(DISPATCH_LEVEL, &OldIrql);

    // write address
    __outdword(PCI_PORT_ADDR, PCI_ADDR(Device, Offset));

    // write data
    __outword(PCI_PORT_DATA, Val);

    KeLowerIrql(OldIrql);
}

ULONG PciDeviceFind(USHORT VendorId, USHORT DeviceId)
{
    // enumerate bus
    for (UCHAR Bus = 0; Bus < 0xff; Bus += 1)
    {
        // enumerate device
        for (UCHAR Dev = 0; Dev < 32; Dev += 1)
        {
            // enumerate function
            for (UCHAR Func = 0; Func < 8; Func += 1)
            {               
                ULONG Device = PCI_DEVICE(Bus, Dev, Func);

                // read IDs register
                ULONG Data = PciRead(Device, PCI_CFG_ID);
                if (Data != PCI_DEVICE_NOT_FOUND)
                {
                    // match vendor ID and device ID
                    if ((USHORT)((Data >> 0) & 0xffff) == VendorId &&
                        (USHORT)((Data >> 16) & 0xffff) == DeviceId)
                    {
                        DbgMsg(
                            __FILE__, __LINE__, __FUNCTION__"(): %.4x:%.4x PCI device is at %.2x:%.2x.%x\n",
                            VendorId, DeviceId, Bus, Dev, Func
                        );

                        return Device;
                    }                    
                }
            }
        }
    }

    return PCI_DEVICE_NOT_FOUND;
}
//--------------------------------------------------------------------------------------
PVOID AllocateLowMemory(ULONG Size)
{
    ULONG Shift = 16;
    PVOID Addr = NULL;

    while (Shift < 32)
    {        
        PHYSICAL_ADDRESS AddrHi;
        AddrHi.HighPart = 0;
        AddrHi.LowPart = (1 << Shift);

        PHYSICAL_ADDRESS AddrLo;
        AddrLo.HighPart = AddrLo.LowPart = 0;

        PHYSICAL_ADDRESS AddrMul;
        AddrMul.HighPart = AddrMul.LowPart = 0;

        // try to allocate requested amount of memory
        if ((Addr = MmAllocateContiguousMemorySpecifyCache(Size, AddrLo, AddrHi, AddrMul, MmNonCached)) != NULL)
        {
            return Addr;
        }

        Shift += 1;
    }

    return NULL;
}
//--------------------------------------------------------------------------------------
void DeviceUnregisterThread(PVOID Context)
{
    if (m_DeviceObject)
    {
        // unregister device and its symbolic link
        IoDeleteSymbolicLink(&m_usDeviceLink);
        IoDeleteDevice(m_DeviceObject);

        m_DeviceObject = NULL;

        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Device successfully deleted\n");
    }
}
//--------------------------------------------------------------------------------------
NTSTATUS DriverDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    NTSTATUS ns = STATUS_INVALID_DEVICE_REQUEST;
    PIO_STACK_LOCATION Stack = IoGetCurrentIrpStackLocation(Irp);
    ULONG InSize = 0, OutSize = 0;

    Irp->IoStatus.Status = ns;
    Irp->IoStatus.Information = 0;

    if (Stack->MajorFunction == IRP_MJ_DEVICE_CONTROL)
    {
        // get IOCTL parameters
        ULONG Code = Stack->Parameters.DeviceIoControl.IoControlCode;
        PHELPER_DEVICE_REQUEST Request = (PHELPER_DEVICE_REQUEST)Irp->AssociatedIrp.SystemBuffer;

        InSize = Stack->Parameters.DeviceIoControl.InputBufferLength;
        OutSize = Stack->Parameters.DeviceIoControl.OutputBufferLength;

        // check buffer length
        if (Code == HELPER_IOCTL_REQUEST &&
            InSize >= sizeof(HELPER_DEVICE_REQUEST) && OutSize >= sizeof(HELPER_DEVICE_REQUEST))
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): IRP_MJ_DEVICE_CONTROL: Code = 0x%.8x\n", Request->Code);

            if (Request->Code == HELPER_CTL_NONE)
            {
                // do nothing, just return successful status
                ns = STATUS_SUCCESS;
            }                       
            else if (Request->Code == HELPER_CTL_MAP_PCI_BAR)
            {
                // find PCI device by vendor ID and device ID
                ULONG Device = PciDeviceFind(Request->MapPciBar.VendorId, Request->MapPciBar.DeviceId);
                if (Device != PCI_DEVICE_NOT_FOUND)
                {
                    PMDL pMdl = NULL;
                    PVOID AddrKernel = NULL, AddrUser = NULL;
                    PHYSICAL_ADDRESS BarAddr;

                    // read base address register
                    BarAddr.HighPart = 0;
                    BarAddr.LowPart = PciRead(Device, PCI_CFG_BAR0);

                    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): PCI BAR0 = 0x%.8x\n", BarAddr.LowPart);

                    // set memory space bit in command register to enable BAR0 region
                    PciWrite(Device, PCI_CFG_COMMAND, PCI_CMD_MEM);

                    // map BAR0 range into the virtual address space
                    if ((AddrKernel = MmMapIoSpace(BarAddr, Request->MapPciBar.Size, MmNonCached)) != NULL)
                    {
                        // allocate MDL for address
                        if ((pMdl = IoAllocateMdl(AddrKernel, Request->MapPciBar.Size, FALSE, FALSE, NULL)) != NULL)
                        {
                            MmBuildMdlForNonPagedPool(pMdl);

                            // map allocated pages into the user space
                            if ((AddrUser = MmMapLockedPagesSpecifyCache(
                                pMdl, UserMode, MmNonCached, NULL, FALSE, NormalPagePriority)) != NULL)
                            {                                
                                Request->MapPciBar.AddrPhys = (PVOID)BarAddr.QuadPart;
                                Request->MapPciBar.AddrVirt = AddrUser;
                                ns = STATUS_SUCCESS;
                            }                            
                            else
                            {
                                DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: MmMapLockedPagesSpecifyCache() fails\n");
                            }
                        }
                        else
                        {
                            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: IoAllocateMdl() fails\n");
                        }
                    }
                    else
                    {
                        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: MmMapIoSpace() fails\n");
                    }

                    // perform cleanup on failure
                    if (!NT_SUCCESS(ns))
                    {
                        if (pMdl)
                        {
                            IoFreeMdl(pMdl);
                        }

                        if (AddrKernel)
                        {
                            MmUnmapIoSpace(AddrKernel, Request->MapPciBar.Size);
                        }
                    }
                }
                else
                {
                    DbgMsg(
                        __FILE__, __LINE__, __FUNCTION__"() ERROR: Unable to find PCI device %.4x:%.4x\n",
                        Request->MapPciBar.VendorId, Request->MapPciBar.DeviceId
                    );
                }
            }
            else if (Request->Code == HELPER_CTL_MAP_MEM)
            {
                PMDL pMdl = NULL;
                PVOID AddrKernel = NULL, AddrUser = NULL;

                // allocate needed amount of memory
                if ((AddrKernel = AllocateLowMemory(Request->MapMem.Size)) != NULL)
                {
                    // allocate MDL for address
                    if ((pMdl = IoAllocateMdl(AddrKernel, Request->MapMem.Size, FALSE, FALSE, NULL)) != NULL)
                    {
                        MmBuildMdlForNonPagedPool(pMdl);

                        // get physical address of allocated memory
                        PHYSICAL_ADDRESS AddrPhys = MmGetPhysicalAddress(AddrKernel);

                        // map allocated pages into the user space
                        if ((AddrUser = MmMapLockedPagesSpecifyCache(
                            pMdl, UserMode, MmNonCached, NULL, FALSE, NormalPagePriority)) != NULL)
                        {                            
                            Request->MapMem.AddrPhys = (PVOID)AddrPhys.QuadPart;
                            Request->MapMem.AddrVirt = AddrUser;
                            ns = STATUS_SUCCESS;
                        }
                        else
                        {
                            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: MmMapLockedPagesSpecifyCache() fails\n");
                        }
                    }
                    else
                    {
                        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: IoAllocateMdl() fails\n");
                    }
                }
                else
                {
                    DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: AllocateLowMemory() fails\n");
                }

                // perform cleanup on failure
                if (!NT_SUCCESS(ns))
                {
                    if (pMdl)
                    {
                        IoFreeMdl(pMdl);
                    }

                    if (AddrKernel)
                    {
                        MmFreeContiguousMemory(AddrKernel);
                    }
                }
            }
            else if (Request->Code == HELPER_CTL_UNREGISTER)
            {
                HANDLE hThread = NULL;

                // create separate thread to unregister driver device
                ns = PsCreateSystemThread(
                    &hThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, DeviceUnregisterThread, NULL
                );
                if (NT_SUCCESS(ns))
                {
                    ZwClose(hThread);
                }
                else
                {
                    DbgMsg(__FILE__, __LINE__, "PsCreateSystemThread() ERROR 0x%.8x\n", ns);
                }
            }
            else
            {
                DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unknown control code\n");
            }
        }

        if (ns == STATUS_SUCCESS)
        {
            Irp->IoStatus.Information = InSize;
        }
    }
    else if (Stack->MajorFunction == IRP_MJ_CREATE)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): IRP_MJ_CREATE\n");

        ns = STATUS_SUCCESS;
    }
    else if (Stack->MajorFunction == IRP_MJ_CLOSE)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): IRP_MJ_CLOSE\n");

        ns = STATUS_SUCCESS;
    }

    if (ns != STATUS_PENDING)
    {
        Irp->IoStatus.Status = ns;

        IoCompleteRequest(Irp, IO_NO_INCREMENT);
    }

    return ns;
}
//--------------------------------------------------------------------------------------
NTSTATUS NTAPI DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{        
    m_DriverObject = DriverObject;

    DbgMsg(__FILE__, __LINE__, "DRIVER_OBJECT address is "IFMT"\n", DriverObject);

    for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i += 1)
    {
        // set IRP dispatch handler
        DriverObject->MajorFunction[i] = DriverDispatch;
    }

    RtlInitUnicodeString(&m_usDeviceName, L"\\Device\\" HELPER_DEVICE_NAME);
    RtlInitUnicodeString(&m_usDeviceLink, L"\\DosDevices\\" HELPER_DEVICE_NAME);

    // create communication device
    NTSTATUS ns = IoCreateDevice(
        DriverObject, 0, &m_usDeviceName, 
        FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &m_DeviceObject
    );
    if (NT_SUCCESS(ns))
    {
        m_DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

        // create win32 symbolic link
        ns = IoCreateSymbolicLink(&m_usDeviceLink, &m_usDeviceName);
        if (NT_SUCCESS(ns))
        {
            return STATUS_SUCCESS;
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, "IoCreateSymbolicLink() ERROR 0x%.8x\n", ns);
        }

        IoDeleteDevice(m_DeviceObject);
    } 
    else 
    {
        DbgMsg(__FILE__, __LINE__, "IoCreateDevice() ERROR 0x%.8x\n", ns);
    }

    return STATUS_UNSUCCESSFUL;
}
//--------------------------------------------------------------------------------------
// EoF
