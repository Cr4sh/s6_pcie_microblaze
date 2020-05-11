#!/usr/bin/env python

import sys, os
from optparse import OptionParser

from pcie_lib import *

def main():       

    parser = OptionParser()

    parser.add_option('-l', '--load', dest = 'load', default = None,
        help = 'load option ROM from file')

    parser.add_option('-e', '--erase', dest = 'erase', default = False, action = 'store_true',
        help = 'erase option ROM')

    parser.add_option('--log-on', dest = 'log_on', default = False, action = 'store_true',
        help = 'enable option ROM access log into the debug UART')

    parser.add_option('--log-off', dest = 'log_off', default = False, action = 'store_true',
        help = 'disable option ROM access log')

    # parse command line
    options, _ = parser.parse_args()

    if not options.erase and options.load is None and not options.log_on and not options.log_off:

        print('USAGE: pcie_rom_ctl.py --load <rom_file_path>')
        print('       pcie_rom_ctl.py --erase')
        return -1

    print('[+] Opening PCI-E device...')
    
    # open device
    dev = TransactionLayer(force = True)

    print('[+] Enabling resident mode...')

    dev.set_resident(True)    

    if options.load is not None or options.erase:

        print('[+] Erasing ROM...')

        # erase ROM contents
        dev.rom_erase()

    if options.load is not None:

        with open(options.load, 'rb') as fd:
            
            # read ROM contents from file
            data = fd.read()

        # query max rom size
        max_size = dev.rom_size()

        print('[+] Maximum ROM size for this device is %d bytes' % max_size)

        if len(data) > max_size:

            print('ERROR: Specified ROM is too large')
            return -1

        print('[+] Loading %d bytes of ROM...' % len(data))

        def progress_cb(percent):

            sys.stdout.flush()
            sys.stdout.write('\r[+] %d%% completed' % percent)

        # write ROM to the device
        dev.rom_load(data, progress_cb = progress_cb)

        sys.stdout.write('\n')

    if options.log_on:

        print('[+] Enabling ROM access log...')

        dev.set_rom_log(True)

    elif options.log_off:

        print('[+] Disabling ROM access log...')

        dev.set_rom_log(False)

    print('[+] Done')

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#
