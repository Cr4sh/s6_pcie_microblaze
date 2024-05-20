
// driver device path
#define WINIO_DEVICE_PATH "\\\\.\\Global\\WNBIOS" 

// driver file name and serive name
#define WINIO_DRIVER_NAME "wnBios64.sys"
#define WINIO_SERVICE_NAME "wnBios64"


#define FILE_DEVICE_WINIO 0x00008010

// map physical memory region
#define IOCTL_WINIO_PHYS_MEM_MAP CTL_CODE(FILE_DEVICE_WINIO, 0x00000810, METHOD_BUFFERED, FILE_ANY_ACCESS)

// unmap physical memory region
#define IOCTL_WINIO_PHYS_MEM_UNMAP CTL_CODE(FILE_DEVICE_WINIO, 0x00000811, METHOD_BUFFERED, FILE_ANY_ACCESS)


/*
    IOCTL_WINIO_PHYS_MEM_MAP and IOCTL_WINIO_PHYS_MEM_UNMAP input params
*/
typedef struct _WINIO_PHYS_MEM
{
    uint64_t size;
    uint64_t address;
    HANDLE section_handle;
    void *section_address;
    void *object;

} WINIO_PHYS_MEM,
*PWINIO_PHYS_MEM;


int winio_phys_mem_read(HANDLE fd, uint64_t address, uint64_t size, void *buff);

int winio_phys_mem_write(HANDLE fd, uint64_t address, uint64_t size, void *buff);
