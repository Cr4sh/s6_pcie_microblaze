#include "stdafx.h"
//--------------------------------------------------------------------------------------
PVOID GetSysInf(SYSTEM_INFORMATION_CLASS InfoClass)
{
    NTSTATUS ns = STATUS_SUCCESS;
    PVOID pInfo = NULL;
    ULONG Size = 0x100;    

    while (true)
    {
        ULONG RetSize = 0;

        // allocate memory for data
        if ((pInfo = M_ALLOC(Size)) == NULL)
        {
            DbgMsg(__FILE__, __LINE__, "M_ALLOC() fails\n");
            return NULL;
        }

        // query system information
        ns = ZwQuerySystemInformation(InfoClass, pInfo, Size, &RetSize);
        if (ns == STATUS_INFO_LENGTH_MISMATCH)
        {
            // buffer is too small
            M_FREE(pInfo);
            pInfo = NULL;

            if (RetSize > 0)
            {
                // allocate more memory and try again
                Size = RetSize + 0x100;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    if (!NT_SUCCESS(ns))
    {
        // error occurred
        DbgMsg(__FILE__, __LINE__, "ZwQuerySystemInformation() ERROR 0x%.8x\n", ns);

        if (pInfo)
        {
            M_FREE(pInfo);
        }

        return NULL;
    }

    return pInfo;
}
//--------------------------------------------------------------------------------------
// EoF
