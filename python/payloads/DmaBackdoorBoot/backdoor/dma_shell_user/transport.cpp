#include "stdafx.h"
//--------------------------------------------------------------------------------------
BOOL DeviceWait(volatile PMEM_SHARED_HEADER Hdr, ULONG Command, DWORD dwTimeout, DWORD dwPoll)
{
    DWORD dwStarted = GetTickCount();    

    // wait for the device reply
    while (Hdr->Command == Command)
    {
        if (dwTimeout != 0 && GetTickCount() - dwStarted > dwTimeout)
        {
            return FALSE;
        }

        if (dwPoll == 0)
        {
            // poll timeout is not specified
            SwitchToThread();            
        }
        else
        {
            Sleep(dwPoll);
        }        
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------
DWORD DeviceSend(PDEVICE_CONTEXT Ctx, PUCHAR Data, DWORD dwDataSize)
{    
    if (Ctx->MemShared == NULL)
    {
        // not initialized
        return ERROR_NOT_READY;
    }

    DWORD dwSent = 0;
    volatile PUCHAR TxData = Ctx->MemShared->TxData;
    volatile PMEM_SHARED_HEADER TxHdr = &Ctx->MemShared->TxHdr;

    while (dwSent < dwDataSize)
    {
        DWORD dwChunkSize = min(dwDataSize - dwSent, MEM_SHARED_DATA_SIZE);        
        
        memcpy(TxData, Data + dwSent, dwChunkSize);        

        // start chunk transfer
        TxHdr->Size = dwChunkSize;
        TxHdr->Command = MEM_CMD_TXD;

        // wait for the completion
        if (!DeviceWait(TxHdr, MEM_CMD_TXD, DEVICE_IO_TIMEOUT, 0))
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Timeout occurred\n");
            return ERROR_TIMEOUT;
        }        

        // check for acknowledgement from the device
        if (TxHdr->Command != MEM_CMD_ACK)
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Device command %d\n", TxHdr->Command);
            return ERROR_BAD_COMMAND;
        }

        dwSent += dwChunkSize;
    }

    // indicate end of the transfer
    TxHdr->Command = MEM_CMD_IDLE;

    return ERROR_SUCCESS;
}
//--------------------------------------------------------------------------------------
DWORD DeviceRecv(PDEVICE_CONTEXT Ctx, PUCHAR Data, DWORD dwDataSize, DWORD dwTimeout)
{
    if (Ctx->MemShared == NULL)
    {
        // not initialized
        return ERROR_NOT_READY;
    }

    DWORD dwReceived = 0;
    volatile PUCHAR RxData = Ctx->MemShared->RxData;
    volatile PMEM_SHARED_HEADER RxHdr = &Ctx->MemShared->RxHdr;

    while (dwReceived < dwDataSize)
    {
        // wait for the device command
        if (!DeviceWait(RxHdr, MEM_CMD_IDLE, dwTimeout, DEVICE_IO_POLL))
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Timeout occurred\n");
            return ERROR_TIMEOUT;
        }

        // check for incomnig data from the device
        if (RxHdr->Command != MEM_CMD_TXD)
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Device command %d\n", RxHdr->Command);
            return ERROR_BAD_COMMAND;
        }

        // determine number of bytes to copy
        DWORD dwRecvSize = RxHdr->Size - Ctx->RecvPtr;
        DWORD dwChunkSize = min(dwDataSize - dwReceived, dwRecvSize);

        memcpy(Data + dwReceived, RxData + Ctx->RecvPtr, dwChunkSize);

        if (dwChunkSize < dwRecvSize)
        {
            // remember remaining amount of data to copy
            Ctx->RecvPtr += dwChunkSize;

            // receive buffer is full
            return ERROR_SUCCESS;
        }       
        else
        {
            // whole chunk was copied
            Ctx->RecvPtr = 0;
        }

        // set receive acknowledgement
        RxHdr->Command = MEM_CMD_ACK;

        // wait for the next chunk of data
        if (!DeviceWait(RxHdr, MEM_CMD_ACK, DEVICE_IO_TIMEOUT, 0))
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Timeout occurred\n");
            return ERROR_TIMEOUT;
        }

        dwReceived += dwChunkSize;
    }

    return ERROR_SUCCESS;
}


DWORD DeviceRecv(PDEVICE_CONTEXT Ctx, PUCHAR Data, DWORD dwDataSize)
{
    return DeviceRecv(Ctx, Data, dwDataSize, DEVICE_IO_TIMEOUT);
}
//--------------------------------------------------------------------------------------
BOOL DeviceReset(PDEVICE_CONTEXT Ctx)
{
    if (Ctx->MemShared == NULL)
    {
        // not initialized
        return FALSE;
    }

    Ctx->MemShared->RxHdr.Command = MEM_CMD_IDLE;
    Ctx->MemShared->TxHdr.Command = MEM_CMD_IDLE;

    Ctx->MemShared->RxHdr.Size = 0;
    Ctx->MemShared->TxHdr.Size = 0;

    Ctx->RecvPtr = 0;

    return TRUE;
}
//--------------------------------------------------------------------------------------
BOOL DeviceOpen(PDEVICE_CONTEXT Ctx, USHORT VendorId, USHORT DeviceId)
{
    BOOL bRet = FALSE;

    Ctx->RecvPtr = 0;
    Ctx->PhysDevice = Ctx->PhysShared = NULL;
    Ctx->MemDevice = NULL;
    Ctx->MemShared = NULL;    

    // map PCI device memory region to the user mode
    if (!HelperMapPci(&Ctx->PhysDevice, (PVOID *)&Ctx->MemDevice, PAGE_SIZE, VendorId, DeviceId))
    {
        DbgMsg(__FILE__, __LINE__, "ERROR: Unable to map PCI BAR\n");
        goto _end;
    }

    DbgMsg(
        __FILE__, __LINE__, "PCI BAR mapped: virt = "IFMT", phys = "IFMT"\n", 
        Ctx->MemDevice, Ctx->PhysDevice
    );

    // allocate and map shared memory region to the user mode
    if (!HelperMapMem(&Ctx->PhysShared, (PVOID *)&Ctx->MemShared, PAGE_SIZE))
    {
        DbgMsg(__FILE__, __LINE__, "ERROR: Unable to map shared memory region\n");
        goto _end;
    }

    DbgMsg(
        __FILE__, __LINE__, "Memory mapped: virt = "IFMT", phys = "IFMT"\n", 
        Ctx->MemShared, Ctx->PhysShared
    );

    // initialize shared memory headers
    ZeroMemory(Ctx->MemShared, MEM_SHARED_SIZE);

    Ctx->MemShared->RxHdr.Sign = MEM_SHARED_SIGN;
    Ctx->MemShared->TxHdr.Sign = MEM_SHARED_SIGN;

    DeviceReset(Ctx);

    // pass shared memory physical address to the PCI-E device
    Ctx->MemDevice->SharedAddr = Ctx->PhysShared;

    bRet = TRUE;

_end:

    return bRet;
}
//--------------------------------------------------------------------------------------
// EoF
