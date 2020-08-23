
// VM exec user export ordinals
#define VM_EXEC_USER_ORD_STRUCT 1

// name of the process to inject DLL into
#define VM_EXEC_PROCESS L"svchost.exe"

// VM_EXEC_INFO address form the beginning of the processor start block
#define VM_EXEC_INFO_ADDR (PAGE_SIZE - sizeof(VM_EXEC_INFO))

// VM_EXEC_INFO signature
#define VM_EXEC_INFO_SIGN 0x2c95491f12db7159

// maximum length of the command
#define VM_EXEC_MAX_COMMAND_LEN 0x100

// control code values
#define VM_EXEC_CTL_READY   0   // ready to execute command
#define VM_EXEC_CTL_RUNING  1   // command is running
#define VM_EXEC_CTL_DONE    2   // command was executed
#define VM_EXEC_CTL_ERROR   3   // error while executing command
#define VM_EXEC_CTL_TIMEOUT 4   // timeout while executing command

#pragma pack(1)

typedef struct _VM_EXEC_STRUCT
{
    uint32_t control;
    uint32_t output_size;
    void *output;
    char command[VM_EXEC_MAX_COMMAND_LEN];

} VM_EXEC_STRUCT,
*PVM_EXEC_STRUCT;

typedef struct _VM_EXEC_INFO
{
    uint64_t signature;
    uint64_t page_dir;
    PVM_EXEC_STRUCT struct_addr;

} VM_EXEC_INFO,
*PVM_EXEC_INFO;

#pragma pack()
