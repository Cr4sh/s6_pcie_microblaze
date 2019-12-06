#!/usr/bin/env python

import sys, os
from optparse import OptionParser

from pcie_lib import *

def main():       

    parser = OptionParser()

    parser.add_option('-r', '--reg', dest = 'reg', default = None,
        help = 'register name')

    parser.add_option('-x', '--hex', dest = 'hex', default = False, action = 'store_true',
        help = 'print raw hexadecimal dump')

    # parse command line
    options, _ = parser.parse_args()
    
    # open device
    dev = TransactionLayer()    

    assert dev_id_encode(*dev.bus_id) != 0

    print('[+] PCI-E link with target is up')
    print('[+] Device address is %.2x:%.2x.%x' % dev.bus_id)

    if options.reg:

        try:

            # get register address by name
            cfg_addr = getattr(dev, 'CFG_' + options.reg)

        except AttributeError:

            print('ERROR: Unknown configuration space register')
            return -1

        # get register name and size
        cfg_size, cfg_name = dev.cfg_regs[cfg_addr]

        print('%s = 0x%x' % (cfg_name, dev.cfg_read(cfg_addr, cfg_size = cfg_size)))

    elif options.hex:

        cfg_addr = 0

        print('')

        # print PCI configuration space contents
        while cfg_addr < 0x100:

            print('%.4x: 0x%.4x 0x%.4x' % (cfg_addr, dev.cfg_read_2(cfg_addr), \
                                                     dev.cfg_read_2(cfg_addr + 2)))

            cfg_addr += 4

        print('')

    else:

        print('')

        # print PCI configuration space registers
        for cfg_addr in dev.cfg_regs.keys():

            # get register name and size
            cfg_size, cfg_name = dev.cfg_regs[cfg_addr]

            print('%20s = 0x%x' % (cfg_name, dev.cfg_read(cfg_addr, cfg_size = cfg_size)))    

        print('')
    
    dev.close()

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#
