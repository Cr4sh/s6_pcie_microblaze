
/*
    backdoor_call() control codes
*/

// get backdoor information
#define HVBD_C_INFO             0

// get extended backdoor information
#define HVBD_C_INFO_EX          1

// get extended backdoor information
#define HVBD_C_INFO_EX2         2

// read virtual memory
#define HVBD_C_VIRT_READ        3

// write virtual memory
#define HVBD_C_VIRT_WRITE       4

// read VMCS value
#define HVBD_C_VMREAD           5

// write VMCS value
#define HVBD_C_VMWRITE          6

// start collecting EPT pointers
#define HVBD_C_EPT_GET_START    7

// stop collecting EPT pointers
#define HVBD_C_EPT_GET_STOP     8

// invalidate processpr caches and flush TLB
#define HVBD_C_INVL_CACHES      9

/*
    backdoor_call() return values
*/

// VM exit backdoor is not present
#define HVBD_E_NO_BACKDOOR      0xffffffffffffffff

// operation successfully completed
#define HVBD_E_SUCCESS          0x0000000000000000

// invalid parameter
#define HVBD_E_INVALID_PARAM    0x8000000000000002

/*
    misc
*/

// magic R10 value to activate VM exit backdoor
#define HVBD_VM_EXIT_MAGIC 0x5ad0432adfc25b2b

typedef struct _EPT_INFO
{
    UINT64 Vpid;
    UINT64 Addr;

} EPT_INFO,
*PEPT_INFO;

#define EPT_MAX_COUNT 32

#define EPT_NONE 0xffffffffffffffff
