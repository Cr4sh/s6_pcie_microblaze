
PROJ = DmaBackdoorHv

ARCH_64 = X64
PATH_64 = ..\..\Build\Ovmf$(ARCH_64)\DEBUG_VS2013x86\$(ARCH_64)\Cr4sh\$(PROJ)\$(PROJ)\OUTPUT

VENDOR_ID = 10ee
DEVICE_ID = 1337

build:
	build
	@if exist $(PATH_64)\$(PROJ).efi copy $(PATH_64)\$(PROJ).efi .\$(PROJ)_$(ARCH_64).efi /Y
	@if exist $(PATH_64)\$(PROJ).pdb copy $(PATH_64)\$(PROJ).pdb .\$(PROJ)_$(ARCH_64).pdb /Y

rom:	
	EfiRom -f 0x$(VENDOR_ID) -i 0x$(DEVICE_ID) -o $(PROJ)_$(ARCH_64)_$(VENDOR_ID)_$(DEVICE_ID).rom -e $(PROJ)_$(ARCH_64).efi
	EfiRom -d $(PROJ)_$(ARCH_64)_$(VENDOR_ID)_$(DEVICE_ID).rom
