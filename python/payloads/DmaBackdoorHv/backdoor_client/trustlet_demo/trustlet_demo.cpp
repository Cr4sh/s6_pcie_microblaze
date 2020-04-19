#include "stdafx.h"

// image policy section
#pragma section(IMAGE_POLICY_SECTION_NAME, read)

// trustlet instance GUID
__declspec(allocate(IMAGE_POLICY_SECTION_NAME))
const char m_ScenarioId[] = "{8059e825-a3e7-47a1-bf46-ac02991067fb}";

// trustlet application ID
#define APPLICATION_ID 1

#ifdef _DEBUG

// in case of debug build allow trustlet debuggning
#define ALLOW_DEBUG 1

#else

#define ALLOW_DEBUG 0

#endif

IMAGE_POLICY_START(APPLICATION_ID)
IMAGE_POLICY_UINT32(ImagePolicyIdEtw, 0)
IMAGE_POLICY_UINT32(ImagePolicyIdDebug, ALLOW_DEBUG)
IMAGE_POLICY_UINT32(ImagePolicyIdCrashDump, 0)
IMAGE_POLICY_ANSI_STRING(ImagePolicyIdScenarioId, m_ScenarioId)
IMAGE_POLICY_END()

uint8_t *m_identy_key = NULL;
unsigned long m_identy_key_size = 0;
//--------------------------------------------------------------------------------------
/*
    Memory allocation function for RPC
*/
void *__RPC_USER midl_user_allocate(size_t size)
{
    return LocalAlloc(LMEM_FIXED, size);
}

/*
    Memory deallocation function for RPC
*/
void __RPC_USER midl_user_free(void *ptr)
{
    LocalFree(ptr);
}

/*
    Terminate trustlet RPC call handler
*/
void Terminate(handle_t IDL_handle, unsigned long Code)
{
    ExitProcess(Code);
}

/*
    Get identy key RPC call handler
*/
error_status_t GetIdentyKey(handle_t IDL_handle, IDENTY_KEY *pKey)
{
    if (m_identy_key && m_identy_key_size > 0)
    {
        // allocate memory for return data
        if ((pKey->Buffer = (byte *)MIDL_user_allocate(m_identy_key_size)) == NULL)
        {
            return RPC_S_OUT_OF_MEMORY;
        }

        pKey->BufferLength = m_identy_key_size;
        memcpy(pKey->Buffer, m_identy_key, m_identy_key_size);

        return RPC_S_OK;
    }

    return RPC_S_INTERNAL_ERROR;
}
//--------------------------------------------------------------------------------------
int rpc_start(void)
{
    // initialize local RPC
    RPC_STATUS status = RpcServerUseProtseqEp(
        (RPC_CSTR)TRUSTLET_RPC_PROT, RPC_C_PROTSEQ_MAX_REQS_DEFAULT,
        (RPC_CSTR)TRUSTLET_RPC_NAME, NULL
    );
    if (status != RPC_S_OK)
    {
        return -1;
    }   

    // registers the TrustletDebug interface
    status = RpcServerRegisterIf(TrustletDebug_v1_0_s_ifspec, NULL, NULL);
    if (status != RPC_S_OK)
    {
        return -1;
    }    

    // start to listen for remote procedure calls for all registered interfaces
    RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, 0);
   
    return 0;
}

int rpc_stop(void)
{
    // stop RPC server
    RPC_STATUS status = RpcMgmtStopServerListening(NULL);
    if (status != RPC_S_OK)
    {
        return -1;
    }

    return 0;
}
//--------------------------------------------------------------------------------------
uint32_t trustlet_main(void)
{    
    // check if current process is running in VTL1
    if (!IsSecureProcess())
    {
        return -1;
    }

    // determine secure identy key size
    if (!GetSecureIdentityKey(m_identy_key, &m_identy_key_size) && m_identy_key_size > 0)
    {
        if ((m_identy_key = (uint8_t *)LocalAlloc(LMEM_FIXED, m_identy_key_size)) != NULL)
        {
            // get secure identy key data
            if (!GetSecureIdentityKey(m_identy_key, &m_identy_key_size))
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

    // start RPC server
    if (rpc_start() != 0)
    {
        return -1;
    }

    return 0;
}
//--------------------------------------------------------------------------------------
// EoF
