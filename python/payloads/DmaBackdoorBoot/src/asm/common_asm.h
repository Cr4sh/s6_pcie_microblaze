
VOID * EFIAPI get_addr(VOID);

/*
    Stubs for hooked functions
*/

EFI_STATUS EFIAPI _ExitBootServices(
    EFI_HANDLE ImageHandle,
    UINTN Key
);
