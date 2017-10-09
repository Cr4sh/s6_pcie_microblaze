#!/bin/sh

echo Generating download.bit
data2mem -bm sdk/microblaze_hw_platform/system_bd.bmm -bt sdk/microblaze_hw_platform/system.bit -bd sdk/srec_bootloader_0/Debug/srec_bootloader_0.elf tag microblaze_0 -o b sdk/microblaze_hw_platform/download.bit 

echo Generating s6_pcie_microblaze.bin
promgen -w -p bin -c FF -o s6_pcie_microblaze -s 8192 -u 0000 sdk/microblaze_hw_platform/download.bit -spi

echo Generating s6_pcie_microblaze.mcs
promgen -w -p mcs -c FF -o s6_pcie_microblaze -s 8192 -u 0000 sdk/microblaze_hw_platform/download.bit -spi

echo Generating main_0.srec
mb-objcopy -O srec sdk/main_0/Debug/main_0.elf sdk/main_0/Debug/main_0.srec

