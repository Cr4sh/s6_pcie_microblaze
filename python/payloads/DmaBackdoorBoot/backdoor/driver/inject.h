
extern "C" NTSTATUS NTAPI _ZwProtectVirtualMemory(
    HANDLE ProcessHandle,
    PVOID *BaseAddress,
    PSIZE_T NumberOfBytesToProtect,
    ULONG NewAccessProtection,
    PULONG OldAccessProtection
);

BOOLEAN InjectIntoStartedProcess(PCLIENT_ID ClientId, PEPROCESS Process, PVOID Data, ULONG DataSize);
