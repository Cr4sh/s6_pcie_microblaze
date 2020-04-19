
/*
    backdoor_call() control codes
*/

#define HVBD_C_INFO             0x00    // get backdoor information
#define HVBD_C_INFO_EX          0x01    // get extended backdoor information
#define HVBD_C_INFO_EX2         0x02    // get extended backdoor information
#define HVBD_C_VIRT_READ        0x03    // read virtual memory
#define HVBD_C_VIRT_WRITE       0x04    // write virtual memory
#define HVBD_C_VMREAD           0x05    // read VMCS value
#define HVBD_C_VMWRITE          0x06    // write VMCS value
#define HVBD_C_EPT_GET_START    0x07    // start collecting EPT information
#define HVBD_C_EPT_GET_STOP     0x08    // stop collecting EPT information
#define HVBD_C_SK_GET_START     0x09    // start collecting secure kernel information
#define HVBD_C_SK_GET_STOP      0x0a    // stop collecting secure kernel information
#define HVBD_C_INVL_CACHES      0x0b    // invalidate processpr caches and flush TLB

/*
    backdoor_call() return values
*/

#define HVBD_E_NO_BACKDOOR      0xffffffffffffffff  // VM exit backdoor is not present
#define HVBD_E_INVALID_PARAM    0x8000000000000002  // invalid parameter
#define HVBD_E_SUCCESS          0x0000000000000000  // operation successfully completed

/*
    misc
*/

// magic R10 value to activate VM exit backdoor
#define HVBD_VM_EXIT_MAGIC 0x5ad0432adfc25b2b

#define EPT_NONE 0xffffffffffffffff
#define EPT_MAX_COUNT 32

#define SK_NONE 0xffffffffffffffff
#define SK_MAX_COUNT 16

#pragma pack(1)

// EPT information
typedef struct _EPT_INFO
{
    UINT64 Vpid;
    UINT64 Addr;

} EPT_INFO,
*PEPT_INFO;

// secure kernel information
typedef struct _SK_INFO
{
    UINT64 Rip;
    UINT64 Cr3;
    UINT64 IdtAddr;
    UINT64 GdtAddr;
    UINT64 EptAddr;

} SK_INFO,
*PSK_INFO;

#pragma pack()

/*
    Firmware variable to store debug messages buffer address
*/

#define BACKDOOR_VAR_NAME L"DmaBackdoorInfo"

#define BACKDOOR_VAR_GUID { 0x4c52678d, 0x4851, 0x4501, \
                          { 0x9a, 0x14, 0x29, 0xa9, 0xae, 0x18, 0xf0, 0x57 }}

// debug messages buffer size 
#define DEBUG_OUTPUT_SIZE PAGE_SIZE
