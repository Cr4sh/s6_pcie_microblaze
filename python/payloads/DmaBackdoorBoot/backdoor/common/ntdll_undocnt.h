
/************************************************************/
/*                                                          */ 
/*  nt!KeServiceDescriptorTable structure                   */
/*                                                          */
/************************************************************/

typedef struct SERVICE_DESCRIPTOR_ENTRY
{
    PVOID	*ServiceTableBase;
    PULONG	ServiceCounterTableBase;
    ULONG	NumberOfServices;
    PUCHAR	ParamTableBase;

} SERVICE_DESCRIPTOR_ENTRY,
*PSERVICE_DESCRIPTOR_ENTRY;

typedef struct _SERVICE_DESCRIPTOR_TABLE 
{
    SERVICE_DESCRIPTOR_ENTRY Entry[2];

} SERVICE_DESCRIPTOR_TABLE,
*PSERVICE_DESCRIPTOR_TABLE; 

/************************************************************/
/*                                                          */ 
/*  Some structures for native API functions                */
/*                                                          */
/************************************************************/

typedef enum _SYSTEM_INFORMATION_CLASS 
{
    SystemBasicInformation,
    SystemProcessorInformation,             // obsolete...delete
    SystemPerformanceInformation,
    SystemTimeOfDayInformation,
    SystemPathInformation,
    SystemProcessInformation,
    SystemCallCountInformation,
    SystemDeviceInformation,
    SystemProcessorPerformanceInformation,
    SystemFlagsInformation,
    SystemCallTimeInformation,
    SystemModuleInformation,
    SystemLocksInformation,
    SystemStackTraceInformation,
    SystemPagedPoolInformation,
    SystemNonPagedPoolInformation,
    SystemHandleInformation,
    SystemObjectInformation,
    SystemPageFileInformation,
    SystemVdmInstemulInformation,
    SystemVdmBopInformation,
    SystemFileCacheInformation,
    SystemPoolTagInformation,
    SystemInterruptInformation,
    SystemDpcBehaviorInformation,
    SystemFullMemoryInformation,
    SystemLoadGdiDriverInformation,
    SystemUnloadGdiDriverInformation,
    SystemTimeAdjustmentInformation,
    SystemSummaryMemoryInformation,
    SystemMirrorMemoryInformation,
    SystemPerformanceTraceInformation,
    SystemObsolete0,
    SystemExceptionInformation,
    SystemCrashDumpStateInformation,
    SystemKernelDebuggerInformation,
    SystemContextSwitchInformation,
    SystemRegistryQuotaInformation,
    SystemExtendServiceTableInformation,
    SystemPrioritySeperation,
    SystemVerifierAddDriverInformation,
    SystemVerifierRemoveDriverInformation,
    SystemProcessorIdleInformation,
    SystemLegacyDriverInformation,
    SystemCurrentTimeZoneInformation,
    SystemLookasideInformation,
    SystemTimeSlipNotification,
    SystemSessionCreate,
    SystemSessionDetach,
    SystemSessionInformation,
    SystemRangeStartInformation,
    SystemVerifierInformation,
    SystemVerifierThunkExtend,
    SystemSessionProcessInformation,
    SystemLoadGdiDriverInSystemSpace,
    SystemNumaProcessorMap,
    SystemPrefetcherInformation,
    SystemExtendedProcessInformation,
    SystemRecommendedSharedDataAlignment,
    SystemComPlusPackage,
    SystemNumaAvailableMemory,
    SystemProcessorPowerInformation,
    SystemEmulationBasicInformation,
    SystemEmulationProcessorInformation,
    SystemExtendedHandleInformation,
    SystemLostDelayedWriteInformation,
    SystemBigPoolInformation,
    SystemSessionPoolTagInformation,
    SystemSessionMappedViewInformation,
    SystemHotpatchInformation,
    SystemObjectSecurityMode,
    SystemWatchdogTimerHandler,
    SystemWatchdogTimerInformation,
    SystemLogicalProcessorInformation,
    SystemWow64SharedInformation,
    SystemRegisterFirmwareTableInformationHandler,
    SystemFirmwareTableInformation,
    SystemModuleInformationEx,
    SystemVerifierTriageInformation,
    SystemSuperfetchInformation,
    SystemMemoryListInformation,
    SystemFileCacheInformationEx,
    MaxSystemInfoClass  // MaxSystemInfoClass should always be the last enum

} SYSTEM_INFORMATION_CLASS;

typedef struct _RTL_PROCESS_MODULE_INFORMATION 
{
    HANDLE Section;                 // Not filled in
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    UCHAR  FullPathName[ 256 ];

} RTL_PROCESS_MODULE_INFORMATION, 
*PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES 
{
    ULONG NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[ 1 ];

} RTL_PROCESS_MODULES, 
*PRTL_PROCESS_MODULES;

typedef enum _SHUTDOWN_ACTION 
{
    ShutdownNoReboot,
    ShutdownReboot,
    ShutdownPowerOff

} SHUTDOWN_ACTION, 
*PSHUTDOWN_ACTION;

typedef struct _DIRECTORY_BASIC_INFORMATION 
{
    UNICODE_STRING ObjectName;
    UNICODE_STRING ObjectTypeName;

} DIRECTORY_BASIC_INFORMATION, 
*PDIRECTORY_BASIC_INFORMATION;

typedef struct _SYSTEM_TIME_OF_DAY_INFORMATION 
{
    LARGE_INTEGER BootTime;
    LARGE_INTEGER CurrentTime;
    LARGE_INTEGER TimeZoneBias;
    ULONG CurrentTimeZoneId;

} SYSTEM_TIME_OF_DAY_INFORMATION, 
*PSYSTEM_TIME_OF_DAY_INFORMATION;

typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO 
{
    USHORT UniqueProcessId;
    USHORT CreatorBackTraceIndex;
    UCHAR ObjectTypeIndex;
    UCHAR HandleAttributes;
    USHORT HandleValue;
    PVOID Object;
    ULONG GrantedAccess;

} SYSTEM_HANDLE_TABLE_ENTRY_INFO, 
*PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

typedef struct _SYSTEM_HANDLE_INFORMATION 
{
    ULONG NumberOfHandles;
    SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[ 1 ];

} SYSTEM_HANDLE_INFORMATION, 
*PSYSTEM_HANDLE_INFORMATION;

typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    USHORT TlsIndex;
    LIST_ENTRY HashLinks;
    PVOID SectionPointer;
    ULONG CheckSum;
    ULONG TimeDateStamp;

} LDR_DATA_TABLE_ENTRY, 
*PLDR_DATA_TABLE_ENTRY;

typedef struct _PEB_LDR_DATA 
{
    ULONG Length;
    BOOLEAN Initialized;
    PVOID SsHandle;
    LIST_ENTRY ModuleListLoadOrder;
    LIST_ENTRY ModuleListMemoryOrder;
    LIST_ENTRY ModuleListInitOrder;

} PEB_LDR_DATA, 
*PPEB_LDR_DATA;

/************************************************************/
/*                                                          */ 
/*  Prototypes for native and kernel API functions          */
/*                                                          */
/************************************************************/

typedef enum _KPROFILE_SOURCE 
{
    ProfileTime,
    ProfileAlignmentFixup,
    ProfileTotalIssues,
    ProfilePipelineDry,
    ProfileLoadInstructions,
    ProfilePipelineFrozen,
    ProfileBranchInstructions,
    ProfileTotalNonissues,
    ProfileDcacheMisses,
    ProfileIcacheMisses,
    ProfileCacheMisses,
    ProfileBranchMispredictions,
    ProfileStoreInstructions,
    ProfileFpInstructions,
    ProfileIntegerInstructions,
    Profile2Issue,
    Profile3Issue,
    Profile4Issue,
    ProfileSpecialInstructions,
    ProfileTotalCycles,
    ProfileIcacheIssues,
    ProfileDcacheAccesses,
    ProfileMemoryBarrierCycles,
    ProfileLoadLinkedIssues,
    ProfileMaximum

} KPROFILE_SOURCE, 
*PKPROFILE_SOURCE;

typedef NTSTATUS (WINAPI * func_NtQueryIntervalProfile)(
    KPROFILE_SOURCE ProfileSource,
    PULONG Interval
);

typedef NTSTATUS (WINAPI * func_NtQuerySystemInformation)(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);

#define func_ZwQuerySystemInformation func_NtQuerySystemInformation

typedef NTSTATUS (NTAPI * func_NtQuerySystemTime)(PLARGE_INTEGER SystemTime);

typedef NTSTATUS (NTAPI * func_NtSetSystemTime)(
    PLARGE_INTEGER SystemTime,
    PLARGE_INTEGER PreviousTime
);

typedef NTSTATUS (WINAPI * func_NtAllocateVirtualMemory)(
    HANDLE ProcessHandle,
    PVOID *BaseAddress,
    ULONG_PTR ZeroBits,
    PSIZE_T RegionSize,
    ULONG AllocationType,
    ULONG Protect 
);

typedef NTSTATUS (WINAPI * func_NtFreeVirtualMemory)(
    HANDLE ProcessHandle,
    PVOID *BaseAddress,
    PSIZE_T RegionSize,
    ULONG FreeType 
);

typedef enum _SECTION_INHERIT 
{
    ViewShare = 1,
    ViewUnmap = 2

} SECTION_INHERIT;

typedef NTSTATUS (WINAPI * func_NtMapViewOfSection)(
    HANDLE SectionHandle,
    HANDLE ProcessHandle,
    OUT PVOID *BaseAddress,
    ULONG_PTR ZeroBits,
    SIZE_T CommitSize,
    PLARGE_INTEGER SectionOffset,
    PSIZE_T ViewSize,
    SECTION_INHERIT InheritDisposition,
    ULONG AllocationType,
    ULONG Win32Protect
);

typedef NTSTATUS (WINAPI * func_NtUnmapViewOfSection)(
    HANDLE ProcessHandle,
    PVOID BaseAddress
);

typedef NTSTATUS (WINAPI * func_NtLoadDriver)(
    PUNICODE_STRING RegistryPath
);

typedef NTSTATUS (WINAPI * func_NtUnloadDriver)(
    PUNICODE_STRING RegistryPath
);

typedef NTSTATUS (WINAPI * func_NtMakeTemporaryObject)(
    HANDLE Handle
);

typedef NTSTATUS (WINAPI * func_NtOpenFile)(
    PHANDLE FileHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PIO_STATUS_BLOCK IoStatusBlock,
    ULONG ShareAccess,
    ULONG OpenOptions
);

typedef NTSTATUS (WINAPI * func_NtCreateFile)(
    PHANDLE FileHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PIO_STATUS_BLOCK IoStatusBlock,
    PLARGE_INTEGER AllocationSize,
    ULONG FileAttributes,
    ULONG ShareAccess,
    ULONG CreateDisposition,
    ULONG CreateOptions,
    PVOID EaBuffer,
    ULONG EaLength
);

typedef NTSTATUS (WINAPI * func_NtOpenDirectoryObject)(
    PHANDLE DirectoryHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes
);

typedef enum _PROCESSINFOCLASS 
{
    ProcessBasicInformation,
    ProcessQuotaLimits,
    ProcessIoCounters,
    ProcessVmCounters,
    ProcessTimes,
    ProcessBasePriority,
    ProcessRaisePriority,
    ProcessDebugPort,
    ProcessExceptionPort,
    ProcessAccessToken,
    ProcessLdtInformation,
    ProcessLdtSize,
    ProcessDefaultHardErrorMode,
    ProcessIoPortHandlers,
    ProcessPooledUsageAndLimits,
    ProcessWorkingSetWatch,
    ProcessUserModeIOPL,
    ProcessEnableAlignmentFaultFixup,
    ProcessPriorityClass,
    ProcessWx86Information,
    ProcessHandleCount,
    ProcessAffinityMask,
    ProcessPriorityBoost,
    ProcessDeviceMap,
    ProcessSessionInformation,
    ProcessForegroundInformation,
    ProcessWow64Information,
    ProcessImageFileName,
    ProcessLUIDDeviceMapsEnabled,
    ProcessBreakOnTermination,
    ProcessDebugObjectHandle,
    ProcessDebugFlags,
    ProcessHandleTracing,
    ProcessIoPriority,
    ProcessExecuteFlags,
    ProcessResourceManagement,
    ProcessCookie,
    ProcessImageInformation,
    MaxProcessInfoClass

} PROCESSINFOCLASS;


typedef struct _PROCESS_ACCESS_TOKEN {

    //
    // Handle to Primary token to assign to the process.
    // TOKEN_ASSIGN_PRIMARY access to this token is needed.
    //

    HANDLE Token;

    //
    // Handle to the initial thread of the process.
    // A process's access token can only be changed if the process has
    // no threads or one thread.  If the process has no threads, this
    // field must be set to NULL.  Otherwise, it must contain a handle
    // open to the process's only thread.  THREAD_QUERY_INFORMATION access
    // is needed via this handle.

    HANDLE Thread;

} PROCESS_ACCESS_TOKEN, *PPROCESS_ACCESS_TOKEN;


typedef struct _PROCESS_BASIC_INFORMATION 
{
    NTSTATUS ExitStatus;
    PVOID PebBaseAddress;
    ULONG_PTR AffinityMask;
    LONG BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR InheritedFromUniqueProcessId;

} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;

typedef NTSTATUS (WINAPI * func_NtQueryInformationProcess)(
    HANDLE ProcessHandle,
    PROCESSINFOCLASS ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength
);

typedef NTSTATUS (WINAPI * func_NtSetInformationProcess)(
    HANDLE ProcessHandle,
    PROCESSINFOCLASS ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength
);

typedef VOID (WINAPI * func_RtlInitUnicodeString)(
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString
);

typedef VOID (WINAPI * func_RtlInitAnsiString)(
    PANSI_STRING DestinationString,
    PCSTR SourceString
);

typedef NTSTATUS (WINAPI * func_RtlAnsiStringToUnicodeString)(
    PUNICODE_STRING DestinationString,
    PANSI_STRING SourceString,
    BOOLEAN AllocateDestinationString
);

typedef BOOLEAN (WINAPI * func_RtlEqualUnicodeString)(
    UNICODE_STRING *String1,
    UNICODE_STRING *String2,
    BOOLEAN CaseInSensitive
);

typedef VOID (WINAPI * func_RtlFreeUnicodeString)(
    PUNICODE_STRING UnicodeString
);

typedef NTSTATUS (WINAPI * func_NtOpenDirectoryObject)(
    PHANDLE DirectoryHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes
);

typedef NTSTATUS (WINAPI * func_NtQueryDirectoryObject)(
    HANDLE DirectoryHandle,
    PVOID Buffer,
    ULONG BufferLength,
    BOOLEAN ReturnSingleEntry,
    BOOLEAN RestartScan,
    PULONG Context,
    PULONG ReturnLength
);

typedef NTSTATUS (WINAPI * func_NtOpenSymbolicLinkObject)(
    PHANDLE SymbolicLinkHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes
);

typedef NTSTATUS (WINAPI * func_NtQuerySymbolicLinkObject)(
    HANDLE SymbolicLinkHandle,
    PUNICODE_STRING TargetName,
    PULONG ReturnLength
);

typedef PIMAGE_NT_HEADERS (WINAPI * func_RtlImageNtHeader)(PVOID ModuleAddress);

typedef PVOID (WINAPI * func_RtlImageDirectoryEntryToData)( 
    PVOID Base, 
    BOOLEAN MappedAsImage, 
    USHORT DirectoryEntry, 
    PULONG Size 
);

typedef PIMAGE_BASE_RELOCATION (WINAPI * func_LdrProcessRelocationBlock)(
    ULONG_PTR VA,
    ULONG SizeOfBlock,
    PUSHORT NextOffset,
    LONG_PTR Diff
);
