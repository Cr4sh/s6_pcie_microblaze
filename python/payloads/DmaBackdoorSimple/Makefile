
PROJ = DmaBackdoorSimple

ARCH_64 = X64
PATH_64 = ../../Build/Ovmf$(ARCH_64)/DEBUG_VS2008x86/$(ARCH_64)/Cr4sh/$(PROJ)/$(PROJ)/OUTPUT

VENDOR_ID = 10ee
DEVICE_ID = 1337

build_64:

	build
	test -f $(PATH_64)/$(PROJ).efi && cp $(PATH_64)/$(PROJ).efi ./$(PROJ)_$(ARCH_64).efi
	test -f $(PATH_64)/$(PROJ).pdb && cp $(PATH_64)/$(PROJ).pdb ./$(PROJ)_$(ARCH_64).pdb

rom:
	
	EfiRom -f 0x$(VENDOR_ID) -i 0x$(DEVICE_ID) -o $(PROJ)_$(ARCH_64)_$(VENDOR_ID)_$(DEVICE_ID).rom -e $(PROJ)_$(ARCH_64).efi
	EfiRom -d $(PROJ)_$(ARCH_64)_$(VENDOR_ID)_$(DEVICE_ID).rom
