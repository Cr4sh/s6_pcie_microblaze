#!/usr/bin/env python

import sys, os
from optparse import OptionParser, make_option

from pcie_lib import *

def dump(dev, file_path, addr, size):

    with open(file_path, 'wb') as fd:

        ptr = 0

        while ptr < size:

            addr_dump = addr + ptr

            try:

                fd.write(dev.mem_read(addr_dump, PAGE_SIZE))             

            except dev.ErrorBadCompletion:

                fd.write('\0' * PAGE_SIZE)

                print('[!] - Error while reading memory page at 0x%x' % addr_dump)

            fd.flush()
            ptr += PAGE_SIZE

def scan(dev, dir_path, addr_from, addr_to):

    ret, addr, prev, size = [], addr_from, None, None

    while addr < addr_to:

        try:

            # check for accessible meory page
            dev.mem_read(addr, 4)    

            if prev is None:

                prev, size = addr, PAGE_SIZE

            else:

                size += PAGE_SIZE

        except dev.ErrorBadCompletion: 

            if prev is not None:

                addr_begin = prev
                addr_end = prev + size - 1

                print('[+] Accessible physical memory found at 0x%.16x:0x%.16x' % \
                      (addr_begin, addr_end))

                if dir_path is not None:

                    file_path = os.path.join(dir_path, '%x_%x.bin' % (addr_begin, addr_end))

                    print('[~] - Dumping memory region into %s' % fname)

                    dump(dev, file_path, addr_begin, size)

                ret.append(( prev, size ))

            prev = size = None

        if addr % 0x1000000 == 0:

            print('[~] Checking memory page at 0x%.16x' % addr)

        addr += PAGE_SIZE

    return ret

def main():       

    option_list = [

        make_option('-a', '--addr', dest = 'addr', default = None,
            help = 'start address of physical memory scanning'),

        make_option('-s', '--size', dest = 'size', default = None,
            help = 'number of bytes to scan'),

        make_option('-d', '--dump', dest = 'dump', default = None,
            help = 'folder path to save duped memory contents') ]    

    options, _ = OptionParser(option_list = option_list).parse_args()
    
    # open uart
    dev = TransactionLayer()    

    addr = int(options.addr, 16) if options.addr is not None else 0x0000000000000000
    size = int(options.size, 16) if options.size is not None else 0x00007ffffffff000

    assert addr % PAGE_SIZE == 0
    assert size % PAGE_SIZE == 0

    assert dev_id_encode(*dev.bus_id) != 0

    print('[+] PCI-E link with target is up')
    print('[+] Device address is %.2x:%.2x.%x' % dev.bus_id)
    
    if options.dump is not None and not os.path.isdir(options.dump):

        os.mkdir(options.dump)

    # scan physical memory space
    scan(dev, options.dump, addr, addr + size)

    dev.close()

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#
