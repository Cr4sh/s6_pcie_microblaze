#include "stdafx.h"
//--------------------------------------------------------------------------------------
bool service_start(char *name, char *path, bool *already_started)
{
    bool ret = false;

    if (already_started)
    {
        *already_started = false;
    }

    SC_HANDLE manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (manager)
    {
        // create service for kernel-mode driver
        SC_HANDLE service = CreateService(
            manager, name, name, SERVICE_START | DELETE | SERVICE_STOP, 
            SERVICE_KERNEL_DRIVER, SERVICE_SYSTEM_START, SERVICE_ERROR_IGNORE, 
            path, NULL, NULL, NULL, NULL, NULL
        );
        if (service == NULL)
        {
            if (GetLastError() == ERROR_SERVICE_EXISTS)
            {
                // open existing service
                if ((service = OpenService(manager, name, SERVICE_START | DELETE | SERVICE_STOP)) == NULL)
                {
                    printf("OpenService() ERROR %d\n", GetLastError());
                }
            }
            else
            {
                printf("CreateService() ERROR %d\n", GetLastError());
            }
        }

        if (service)
        {                
            // start service
            if (StartService(service, 0, NULL))
            {
                ret = true;
            }
            else
            {
                if (GetLastError() == ERROR_SERVICE_ALREADY_RUNNING)
                {
                    // service is already started
                    if (already_started)
                    {
                        *already_started = true;
                    }

                    ret = true;
                }
                else
                {
                    printf("StartService() ERROR %d\n", GetLastError());
                }                    
            }            

            CloseServiceHandle(service);
        }

        CloseServiceHandle(manager);
    }
    else
    {
        printf("OpenSCManager() ERROR %d\n", GetLastError());
    }

    return ret;
}
//--------------------------------------------------------------------------------------
bool service_stop(char *name)
{
    bool ret = false;

    SC_HANDLE manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (manager)
    {
        // open existing service
        SC_HANDLE service = OpenService(manager, name, SERVICE_ALL_ACCESS);
        if (service)
        {
            SERVICE_STATUS Status;
            
            // stop service
            if (ControlService(service, SERVICE_CONTROL_STOP, &Status))
            {
                ret = true;
            }
            else
            {
                printf("ControlService() ERROR %d\n", GetLastError());
            }

            CloseServiceHandle(service);
        }
        else
        {
            printf("OpenService() ERROR %d\n", GetLastError());
        }

        CloseServiceHandle(manager);
    }
    else
    {
        printf("OpenSCManager() ERROR %d\n", GetLastError());
    }

    return ret;
}
//--------------------------------------------------------------------------------------
bool service_remove(char *name)
{
    bool ret = false;

    SC_HANDLE manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (manager)
    {
        // open existing service
        SC_HANDLE service = OpenService(manager, name, SERVICE_ALL_ACCESS);
        if (service)
        {     
            // delete service
            if (DeleteService(service))
            {
                ret = true;
            }
            else
            {
                printf("DeleteService() ERROR %d\n", GetLastError());
            }

            CloseServiceHandle(service);
        }
        else
        {
            printf("OpenService() ERROR %d\n", GetLastError());
        }

        CloseServiceHandle(manager);
    }
    else
    {
        printf("OpenSCManager() ERROR %d\n", GetLastError());
    }

    return ret;
}
//--------------------------------------------------------------------------------------
// EoF
