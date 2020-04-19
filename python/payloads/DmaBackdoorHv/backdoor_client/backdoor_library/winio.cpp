#include "stdafx.h"
//--------------------------------------------------------------------------------------
int winio_phys_mem_map(
    HANDLE fd, uint64_t address, uint64_t size,
    HANDLE *section_handle, void **section_address, void **object)
{
    WINIO_PHYS_MEM request;
    unsigned long bytes_returned = 0;    

    memset(&request, 0, sizeof(request));

    request.size = size;
    request.address = address;

    // send memory map request
    if (DeviceIoControl(
        fd, IOCTL_WINIO_PHYS_MEM_MAP,
        &request, sizeof(request), &request, sizeof(request), &bytes_returned, NULL))
    {
        *object = request.object;
        *section_handle = request.section_handle;
        *section_address = request.section_address;
        
        return 0;
    }
    else
    {
        printf("DeviceIoControl() ERROR %d\n", GetLastError());
        printf("ERROR: Can't map physical memory\n");
    }

    return -1;
}
//--------------------------------------------------------------------------------------
int winio_phys_mem_unmap(HANDLE fd, HANDLE section_handle, void *section_address, void *object)
{
    WINIO_PHYS_MEM request;
    unsigned long bytes_returned = 0;    

    memset(&request, 0, sizeof(request));

    request.section_handle = section_handle;
    request.section_address = section_address;
    request.object = object;    

    // send memory unmap request
    if (DeviceIoControl(
        fd, IOCTL_WINIO_PHYS_MEM_UNMAP,
        &request, sizeof(request), &request, sizeof(request), &bytes_returned, NULL))
    {
        return 0;
    }
    else
    {
        printf("DeviceIoControl() ERROR %d\n", GetLastError());
        printf("ERROR: Can't unmap physical memory\n");
    }

    return -1;
}
//--------------------------------------------------------------------------------------
int winio_phys_mem_read(HANDLE fd, uint64_t address, uint64_t size, void *buff)
{
    int ret = -1;

    HANDLE section_handle = NULL;
    void *section_address = NULL;
    void *object = NULL;

    // map physical memory region
    if (winio_phys_mem_map(
        fd, address, size,
        &section_handle, &section_address, &object) != 0)
    {
        return -1;
    }

    __try
    {
        memcpy(buff, section_address, size);

        ret = 0;
    }
    __finally
    {
        // unmap physical memory region
        winio_phys_mem_unmap(fd, section_handle, section_address, object);
    }       

    return ret;
}
//--------------------------------------------------------------------------------------
int winio_phys_mem_write(HANDLE fd, uint64_t address, uint64_t size, void *buff)
{
    int ret = -1;

    HANDLE section_handle = NULL;
    void *section_address = NULL;
    void *object = NULL;

    // map physical memory region
    if (winio_phys_mem_map(
        fd, address, size,
        &section_handle, &section_address, &object) != 0)
    {
        return -1;
    }

    __try
    {
        memcpy(section_address, buff, size);

        ret = 0;
    }
    __finally
    {
        // unmap physical memory region
        winio_phys_mem_unmap(fd, section_handle, section_address, object);
    }

    return ret;
}
//--------------------------------------------------------------------------------------
// EoF
