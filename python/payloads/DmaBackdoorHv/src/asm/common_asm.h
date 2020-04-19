
VOID * EFIAPI get_addr(VOID);

/*
    Stubs for hooked functions
*/

EFI_STATUS EFIAPI _OpenProtocol(
    EFI_HANDLE Handle, 
    EFI_GUID *Protocol, 
    VOID **Interface, 
    EFI_HANDLE AgentHandle, 
    EFI_HANDLE ControllerHandle, 
    UINT32 Attributes
);

EFI_STATUS EFIAPI _ExitBootServices(
    EFI_HANDLE ImageHandle,
    UINTN Key
);
