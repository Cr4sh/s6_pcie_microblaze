#include "stdafx.h"
//--------------------------------------------------------------------------------------
void hexdump(unsigned char *data, unsigned int data_size, unsigned long long addr)
{
    unsigned int dp = 0, p = 0;
    const char trans[] =
        "................................ !\"#$%&'()*+,-./0123456789"
        ":;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklm"
        "nopqrstuvwxyz{|}~...................................."
        "....................................................."
        "........................................";

    char buff[0x100], temp[10];
    memset(buff, 0, sizeof(buff));

    for (dp = 1; dp <= data_size; dp++)
    {
        sprintf(temp, "%02x ", data[dp - 1]);
        strcat(buff, temp);

        if ((dp % 8) == 0)
        {
            strcat(buff, " ");
        }

        if ((dp % 16) == 0)
        {
            strcat(buff, "| ");
            p = dp;

            for (dp -= 16; dp < p; dp++)
            {
                sprintf(temp, "%c", trans[data[dp]]);
                strcat(buff, temp);
            }

            printf(addr ? "%.16llx: %s\r\n" : "%.8llx: %s\r\n", addr + dp - 16, buff);
            memset(buff, 0, sizeof(buff));
        }
    }

    if ((data_size % 16) != 0)
    {
        p = dp = 16 - (data_size % 16);

        for (dp = p; dp > 0; dp--)
        {
            strcat(buff, "   ");

            if (((dp % 8) == 0) && (p != 8))
            {
                strcat(buff, " ");
            }
        }

        strcat(buff, " | ");
        
        for (dp = (data_size - (16 - p)); dp < data_size; dp++)
        {
            sprintf(temp, "%c", trans[data[dp]]);
            strcat(buff, temp);
        }

        printf(addr ? "%.16llx: %s\r\n" : "%.8llx: %s\r\n", addr + data_size - (data_size % 16), buff);
    }
}
//--------------------------------------------------------------------------------------
int load_privileges(char *name)
{
    int ret = -1;
    HANDLE token = NULL;
    TOKEN_PRIVILEGES privs;
    LUID val;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
    {
        printf("OpenProcessToken() ERROR %d\n", GetLastError());
        goto _end;
    }

    if (!LookupPrivilegeValueA(NULL, name, &val))
    {
        printf("LookupPrivilegeValue() ERROR %d\n", GetLastError());
        goto _end;
    }

    privs.PrivilegeCount = 1;
    privs.Privileges[0].Luid = val;
    privs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(token, FALSE, &privs, sizeof (privs), NULL, NULL))
    {
        printf("AdjustTokenPrivileges() ERROR %d\n", GetLastError());
        goto _end;
    }

    ret = 0;

_end:

    if (token)
    {
        CloseHandle(token);
    }

    return ret;
}
//--------------------------------------------------------------------------------------
int process_name_by_id(uint32_t process_id, char *name, size_t name_len)
{
    int ret = -1;

    // enumerate processes
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 entry = { 0 };
        entry.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(snap, &entry))
        {
            do 
            {
                if (entry.th32ProcessID == process_id)
                {
                    strncpy(name, entry.szExeFile, name_len);
                    ret = 0;
                    break;
                }
            }
            while (Process32Next(snap, &entry));
        }

        CloseHandle(snap);
    }

    return ret;
}
//--------------------------------------------------------------------------------------
// EoF
