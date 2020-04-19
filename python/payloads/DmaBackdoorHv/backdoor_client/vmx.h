
/*
    VMCS fields
*/
#define GUEST_ES_SELECTOR               0x00000800
#define GUEST_CS_SELECTOR               0x00000802
#define GUEST_SS_SELECTOR               0x00000804
#define GUEST_DS_SELECTOR               0x00000806
#define GUEST_FS_SELECTOR               0x00000808
#define GUEST_GS_SELECTOR               0x0000080a
#define GUEST_LDTR_SELECTOR             0x0000080c
#define GUEST_TR_SELECTOR               0x0000080e
#define HOST_ES_SELECTOR                0x00000c00
#define HOST_CS_SELECTOR                0x00000c02
#define HOST_SS_SELECTOR                0x00000c04
#define HOST_DS_SELECTOR                0x00000c06
#define HOST_FS_SELECTOR                0x00000c08
#define HOST_GS_SELECTOR                0x00000c0a
#define HOST_TR_SELECTOR                0x00000c0c
#define IO_BITMAP_A                     0x00002000
#define IO_BITMAP_A_HIGH                0x00002001
#define IO_BITMAP_B                     0x00002002
#define IO_BITMAP_B_HIGH                0x00002003
#define GUEST_PDPTE0                    0x0000280A
#define GUEST_PDPTE1                    0x0000280C
#define GUEST_PDPTE2                    0x0000280E
#define GUEST_PDPTE3                    0x00002810
#define EPT_POINTER                     0x0000201a
#define EPT_POINTER_HIGH                0x0000201b
#define MSR_BITMAP                      0x00002004
#define MSR_BITMAP_HIGH                 0x00002005
#define VM_EXIT_MSR_STORE_ADDR          0x00002006
#define VM_EXIT_MSR_STORE_ADDR_HIGH     0x00002007
#define VM_EXIT_MSR_LOAD_ADDR           0x00002008
#define VM_EXIT_MSR_LOAD_ADDR_HIGH      0x00002009
#define VM_ENTRY_MSR_LOAD_ADDR          0x0000200a
#define VM_ENTRY_MSR_LOAD_ADDR_HIGH     0x0000200b
#define TSC_OFFSET                      0x00002010
#define TSC_OFFSET_HIGH                 0x00002011
#define VIRTUAL_APIC_PAGE_ADDR          0x00002012
#define VIRTUAL_APIC_PAGE_ADDR_HIGH     0x00002013
#define VMCS_LINK_POINTER               0x00002800
#define VMCS_LINK_POINTER_HIGH          0x00002801
#define GUEST_IA32_DEBUGCTL             0x00002802
#define GUEST_IA32_DEBUGCTL_HIGH        0x00002803
#define PIN_BASED_VM_EXEC_CONTROL       0x00004000
#define CPU_BASED_VM_EXEC_CONTROL       0x00004002
#define EXCEPTION_BITMAP                0x00004004
#define PAGE_FAULT_ERROR_CODE_MASK      0x00004006
#define PAGE_FAULT_ERROR_CODE_MATCH     0x00004008
#define CR3_TARGET_COUNT                0x0000400a
#define VM_EXIT_CONTROLS                0x0000400c
#define VM_EXIT_MSR_STORE_COUNT         0x0000400e
#define VM_EXIT_MSR_LOAD_COUNT          0x00004010
#define VM_ENTRY_CONTROLS               0x00004012
#define VM_ENTRY_MSR_LOAD_COUNT         0x00004014
#define VM_ENTRY_INTR_INFO_FIELD        0x00004016
#define VM_ENTRY_EXCEPTION_ERROR_CODE   0x00004018
#define VM_ENTRY_INSTRUCTION_LEN        0x0000401a
#define TPR_THRESHOLD                   0x0000401c
#define SECONDARY_VM_EXEC_CONTROL       0x0000401e
#define VM_INSTRUCTION_ERROR            0x00004400
#define VM_EXIT_REASON                  0x00004402
#define VM_EXIT_INTR_INFO               0x00004404
#define VM_EXIT_INTR_ERROR_CODE         0x00004406
#define IDT_VECTORING_INFO_FIELD        0x00004408
#define IDT_VECTORING_ERROR_CODE        0x0000440a
#define VM_EXIT_INSTRUCTION_LEN         0x0000440c
#define VMX_INSTRUCTION_INFO            0x0000440e
#define GUEST_ES_LIMIT                  0x00004800
#define GUEST_CS_LIMIT                  0x00004802
#define GUEST_SS_LIMIT                  0x00004804
#define GUEST_DS_LIMIT                  0x00004806
#define GUEST_FS_LIMIT                  0x00004808
#define GUEST_GS_LIMIT                  0x0000480a
#define GUEST_LDTR_LIMIT                0x0000480c
#define GUEST_TR_LIMIT                  0x0000480e
#define GUEST_GDTR_LIMIT                0x00004810
#define GUEST_IDTR_LIMIT                0x00004812
#define GUEST_ES_AR_BYTES               0x00004814
#define GUEST_CS_AR_BYTES               0x00004816
#define GUEST_SS_AR_BYTES               0x00004818
#define GUEST_DS_AR_BYTES               0x0000481a
#define GUEST_FS_AR_BYTES               0x0000481c
#define GUEST_GS_AR_BYTES               0x0000481e
#define GUEST_LDTR_AR_BYTES             0x00004820
#define GUEST_TR_AR_BYTES               0x00004822
#define GUEST_INTERRUPTIBILITY_INFO     0x00004824
#define GUEST_ACTIVITY_STATE            0x00004826
#define GUEST_SM_BASE                   0x00004828
#define GUEST_SYSENTER_CS               0x0000482A
#define HOST_IA32_SYSENTER_CS           0x00004c00
#define CR0_GUEST_HOST_MASK             0x00006000
#define CR4_GUEST_HOST_MASK             0x00006002
#define CR0_READ_SHADOW                 0x00006004
#define CR4_READ_SHADOW                 0x00006006
#define CR3_TARGET_VALUE0               0x00006008
#define CR3_TARGET_VALUE1               0x0000600a
#define CR3_TARGET_VALUE2               0x0000600c
#define CR3_TARGET_VALUE3               0x0000600e
#define EXIT_QUALIFICATION              0x00006400
#define GUEST_LINEAR_ADDRESS            0x0000640a
#define GUEST_CR0                       0x00006800
#define GUEST_CR3                       0x00006802
#define GUEST_CR4                       0x00006804
#define GUEST_ES_BASE                   0x00006806
#define GUEST_CS_BASE                   0x00006808
#define GUEST_SS_BASE                   0x0000680a
#define GUEST_DS_BASE                   0x0000680c
#define GUEST_FS_BASE                   0x0000680e
#define GUEST_GS_BASE                   0x00006810
#define GUEST_LDTR_BASE                 0x00006812
#define GUEST_TR_BASE                   0x00006814
#define GUEST_GDTR_BASE                 0x00006816
#define GUEST_IDTR_BASE                 0x00006818
#define GUEST_DR7                       0x0000681a
#define GUEST_RSP                       0x0000681c
#define GUEST_RIP                       0x0000681e
#define GUEST_RFLAGS                    0x00006820
#define GUEST_PENDING_DBG_EXCEPTIONS    0x00006822
#define GUEST_SYSENTER_ESP              0x00006824
#define GUEST_SYSENTER_EIP              0x00006826
#define HOST_CR0                        0x00006c00
#define HOST_CR3                        0x00006c02
#define HOST_CR4                        0x00006c04
#define HOST_FS_BASE                    0x00006c06
#define HOST_GS_BASE                    0x00006c08
#define HOST_TR_BASE                    0x00006c0a
#define HOST_GDTR_BASE                  0x00006c0c
#define HOST_IDTR_BASE                  0x00006c0e
#define HOST_IA32_SYSENTER_ESP          0x00006c10
#define HOST_IA32_SYSENTER_EIP          0x00006c12
#define HOST_RSP                        0x00006c14
#define HOST_RIP                        0x00006c16

/*
    VM exit reasons
*/
#define VM_EXIT_NMI             0       // Exception or non-maskable interrupt (NMI)
#define VM_EXIT_INT             1       // External interrupt
#define VM_EXIT_TF              2       // Triple fault
#define VM_EXIT_INIT            3       // An INIT signal arrived
#define VM_EXIT_SIPI            4       // Start-up IPI (SIPI)
#define VM_EXIT_SMI             5       // I/O system-management interrupt (SMI)
#define VM_EXIT_OTHER_SMI       6       // Other SMI
#define VM_EXIT_INT_WND         7       // Interrupt window
#define VM_EXIT_NMI_WND         8       // NMI window
#define VM_EXIT_TASK            9       // Guest software attempted a task switch
#define VM_EXIT_CPUID           10      // Guest software attempted to execute CPUID
#define VM_EXIT_GETSEC          11      // Guest software attempted to execute GETSEC
#define VM_EXIT_HLT             12      // Guest software attempted to execute HLT
#define VM_EXIT_INVD            13      // Guest software attempted to execute INVD
#define VM_EXIT_INVLPG          14      // Guest software attempted to execute INVLPG
#define VM_EXIT_RDPMC           15      // Guest software attempted to execute RDPMC
#define VM_EXIT_RDTSC           16      // Guest software attempted to execute RDTSC
#define VM_EXIT_RSM             17      // Guest software attempted to execute RSM in SMM
#define VM_EXIT_VMCALL          18      // VMCALL was executed
#define VM_EXIT_VMCLEAR         19      // Guest software attempted to execute VMCLEAR
#define VM_EXIT_VMLAUNCH        20      // Guest software attempted to execute VMLAUNCH
#define VM_EXIT_VMPTRLD         21      // Guest software attempted to execute VMPTRLD
#define VM_EXIT_VMPTRST         22      // Guest software attempted to execute VMPTRST
#define VM_EXIT_VMREAD          23      // Guest software attempted to execute VMREAD
#define VM_EXIT_VMRESUME        24      // Guest software attempted to execute VMRESUME
#define VM_EXIT_VMWRITE         25      // Guest software attempted to execute VMWRITE
#define VM_EXIT_VMXOFF          26      // Guest software attempted to execute VMXOFF
#define VM_EXIT_VMXON           27      // Guest software attempted to execute VMXON
#define VM_EXIT_CR              28      // Guest software attempted to access CR0, CR3, CR4, or CR8
#define VM_EXIT_DR              29      // Guest software attempted a MOV to or from a debug register
#define VM_EXIT_IO              30      // Guest software attempted to execute an I/O instruction
#define VM_EXIT_RDMSR           31      // Guest software attempted to execute RDMSR
#define VM_EXIT_WRMSR           32      // Guest software attempted to execute WRMSR
#define VM_EXIT_VM_FAIL_STATE   33      // VM-entry failure due to invalid guest state
#define VM_EXIT_VM_FAIL_MSR     34      // VM-entry failure due to MSR loading
#define VM_EXIT_MWAIT           36      // Guest software attempted to execute MWAIT
#define VM_EXIT_TRAP            37      // A VM entry occurred due to the 1-setting of the “monitor trap flag”
#define VM_EXIT_MONITOR         39      // Guest software attempted to execute MONITOR
#define VM_EXIT_PAUSE           40      // Guest software attempted to execute PAUSE
#define VM_EXIT_VM_FAIL_MC      41      // VM-entry failure due to machine-check event
#define VM_EXIT_TPR             43      // TPR below threshold
#define VM_EXIT_APIC            44      // APIC access
#define VM_EXIT_EOI             45      // Virtualized EOI
#define VM_EXIT_GDTR            46      // Guest software attempted to execute LGDT, LIDT, SGDT, or SIDT
#define VM_EXIT_LDTR            47      // Guest software attempted to execute LLDT, LTR, SLDT, or STR
#define VM_EXIT_EPT_VIOLATION   48      // EPT violation
#define VM_EXIT_EPT_MISCONFIG   49      // EPT misconfiguration
#define VM_EXIT_INVEPT          50      // Guest software attempted to execute INVEPT
#define VM_EXIT_RDTSCP          51      // Guest software attempted to execute RDTSCP
#define VM_EXIT_VMX_TIMER       52      // VMX-preemption timer expired
#define VM_EXIT_INVVPID         53      // Guest software attempted to execute INVVPID
#define VM_EXIT_WBINVD          54      // Guest software attempted to execute WBINVD
#define VM_EXIT_XSETBV          55      // Guest software attempted to execute XSETBV
#define VM_EXIT_APIC_WRITE      56      // Guest software completed a write to the virtual-APIC page
#define VM_EXIT_RDRAND          57      // Guest software attempted to execute RDRAND
#define VM_EXIT_INVPCID         58      // Guest software attempted to execute INVPCID
#define VM_EXIT_VMFUNC          59      // Guest software invoked a VM function with the VMFUNC instruction
#define VM_EXIT_RDSEED          61      // Guest software attempted to execute RDSEED
#define VM_EXIT_XSAVES          63      // Guest software attempted to execute XSAVES
#define VM_EXIT_XRSTORS         64      // Guest software attempted to execute XRSTORS
