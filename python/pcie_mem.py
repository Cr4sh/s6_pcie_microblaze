#!/usr/bin/env python

import sys, os

from pcie_lib import *

def main():       

    if len(sys.argv) < 3:

        print('USAGE: pcie_mem.py <address> <size> [output_file_path]')
        return -1

    addr = int(sys.argv[1], 16)
    size = int(sys.argv[2], 16)
    
    # open device
    dev = TransactionLayer()    

    assert dev_id_encode(*dev.bus_id) != 0

    print('[+] PCI-E link with target is up')
    print('[+] Device address is %.2x:%.2x.%x' % dev.bus_id)

    if len(sys.argv) >= 4:

        path, ptr = sys.argv[3], 0

        # save data into the file
        with open(path, 'wb') as fd:

            while ptr < size:

                print('[+] Reading 0x%x' % (addr + ptr))
                
                # read memory contents
                fd.write(dev.mem_read(addr + ptr, PAGE_SIZE))
                fd.flush()

                ptr += PAGE_SIZE
        
        print('%d bytes written into the %s' % (size, path))

    else:

        # read memory contents
        data = dev.mem_read(addr, size)  

        print(hexdump(data, addr = addr))

    dev.close()

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#
