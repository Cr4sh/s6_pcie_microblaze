
// control protocol commands
#define PROT_CMD_ERROR      0
#define PROT_CMD_SUCCESS    1
#define PROT_CMD_PING       2 
#define PROT_CMD_EXEC       3
#define PROT_CMD_DOWNLOAD   4
#define PROT_CMD_UPLOAD     5
#define PROT_CMD_LOAD_IMAGE 6

#pragma pack(1)

typedef struct _PROT_CTL
{
    DWORD Code;
    DWORD Size;
    UCHAR Data[];

} PROT_CTL,
*PPROT_CTL;

// PROT_CMD_EXEC request
typedef struct _PROT_CTL_EXEC_REQUEST
{
    DWORD Timeout;
    WCHAR Command[];

} PROT_CTL_EXEC_REQUEST,
*PPROT_CTL_EXEC_REQUEST;

// PROT_CMD_EXEC reply
typedef struct _PROT_CTL_EXEC_REPLY
{
    DWORD ExitCode;
    WCHAR Output[];

} PROT_CTL_EXEC_REPLY,
*PPROT_CTL_EXEC_REPLY;

// PROT_CMD_DOWNLOAD request
typedef struct _PROT_CTL_DOWNLOAD_REQUEST
{
    WCHAR Path[];

} PROT_CTL_DOWNLOAD_REQUEST,
*PPROT_CTL_DOWNLOAD_REQUEST;

// PROT_CMD_UPLOAD request
typedef struct _PROT_CTL_UPLOAD_REQUEST
{
    DWORD FileSize;
    WCHAR Path[];

} PROT_CTL_UPLOAD_REQUEST,
*PPROT_CTL_UPLOAD_REQUEST;

#pragma pack()
