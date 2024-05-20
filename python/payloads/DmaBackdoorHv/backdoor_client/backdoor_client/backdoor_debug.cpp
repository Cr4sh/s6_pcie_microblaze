#include "stdafx.h"

// winio driver binary
#include "../wnBios64_sys.h"
//--------------------------------------------------------------------------------------
int backdoor_read_debug_messages(void)
{    
    int ret = -1;    
    wchar_t *var_name = BACKDOOR_VAR_NAME, var_guid[MAX_PATH];
    GUID guid = BACKDOOR_VAR_GUID;
    uint64_t address = 0;

    wsprintfW(
        var_guid, L"{%.8x-%.4x-%.4x-%.2x%.2x-%.2x%.2x%.2x%.2x%.2x%.2x}",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]
    );

    // obtain needed privileges
    if (load_privileges(SE_SYSTEM_ENVIRONMENT_NAME) != 0)
    {
        bd_printf("ERROR: Unable to obtain %s privilege\n", SE_SYSTEM_ENVIRONMENT_NAME);
        return -1;
    }

    bd_printf("[+] Reading firmware variable %ws %ws\n", var_name, var_guid);

    // get debug messages buffer address
    if (GetFirmwareEnvironmentVariableW(var_name, var_guid, (void *)&address, sizeof(address)) != sizeof(address))
    {
        bd_printf("GetFirmwareEnvironmentVariable() ERROR %d\n", GetLastError());
        bd_printf("ERROR: Unable to get debug messages buffer address\n");
        return -1;
    }    

    HANDLE fd = NULL;
    char driver_path[MAX_PATH];
    char *driver_name = WINIO_DRIVER_NAME, *device_path = WINIO_DEVICE_PATH, *service_name = WINIO_SERVICE_NAME;

    GetSystemDirectory(driver_path, MAX_PATH);
    strcat_s(driver_path, "\\drivers\\");
    strcat_s(driver_path, driver_name);

    // write driver into the system folder
    if ((fd = CreateFile(driver_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
    {
        DWORD written = 0;

        WriteFile(fd, driver, sizeof(driver), &written, NULL);
        CloseHandle(fd);
    }
    else
    {
        bd_printf("CreateFile() ERROR %d\n", GetLastError());
        bd_printf("ERROR: Unable to create driver file \"%s\"\n", driver_path);
        return -1;
    }

    bool already_started = FALSE, stop = FALSE;

    bd_printf("[+] Loading %s driver...\n", service_name);

    if ((fd = CreateFile(device_path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE)
    {
        // create and start service
        if (service_start(service_name, driver_path, &already_started))
        {
            stop = !already_started;

            bd_printf("[+] %s driver was loaded\n", service_name);

            // open driver device
            fd = CreateFile(device_path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
            if (fd == INVALID_HANDLE_VALUE)
            {
                bd_printf("CreateFile() ERROR %d\n", GetLastError());
                bd_printf("ERROR: Unable to open \"%s\"\n", device_path);
                goto _end;
            }
        }
        else
        {
            bd_printf("ERROR: Unable to load %s driver\n", service_name);
            goto _end;
        }
    }    
    else
    {
        bd_printf("[+] %s driver is already loaded\n", service_name);
    }

    if (fd != INVALID_HANDLE_VALUE)
    {   
        char *buff = (char *)bd_alloc(DEBUG_OUTPUT_SIZE);
        if (buff)
        {
            memset(buff, 0, DEBUG_OUTPUT_SIZE);

            bd_printf("[+] Reading DXE driver debug messages from 0x%llx\n", address);

            // read debug messages buffer 
            if (winio_phys_mem_read(fd, address, DEBUG_OUTPUT_SIZE, buff) == 0)
            {
                // print debug messages to the screen
                bd_printf("\n%s\n", buff);

                ret = 0;
            }
            else
            {
                bd_printf("ERROR: Unable to read debug messages buffer\n");
            }

            bd_free(buff);
        }

        CloseHandle(fd);
    }

_end:

    if (stop)
    {
        // stop and delete service
        service_stop(service_name);
        service_remove(service_name);
    }

    DeleteFile(driver_path);

    return ret;
}
//--------------------------------------------------------------------------------------
// EoF
