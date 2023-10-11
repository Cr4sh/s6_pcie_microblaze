#!/usr/bin/env python

import sys, os
from optparse import OptionParser

from pcie_lib import *

# struct SCAN_CONF location
SCAN_CONF_OFFS = 8

# struct SCAN_CONF size
SCAN_CONF_SIZE = 8 * 4

# used to override default EFI_SYSTEM_TABLE scan settings
SCAN_CONF_SIGN = 0x524444414e414353

EFI_MAX_ADDR = (1 << 32)

# DOS header signature
HEADER_MAGIC = 'MZ'

class BitstreamLoader(EndpointSerial):

    CTL_BIT_WRITE = 0xfe
    CTL_BIT_ERASE = 0xff

    BIT_MAX_SIZE = 0x150000

    def bit_load(self, data, progress_cb = None):

        chunk_ptr = 0
        chunk_len = self.ROM_CHUNK_LEN
        total_len, percent_prev = len(data), None

        while len(data) > 0:
            
            chunk = data[: chunk_len]

            # bitstream write request
            buff = pack('<BBI', self.CTL_BIT_WRITE, len(chunk) + 4, chunk_ptr) + chunk

            self.device.write(buff)

            # receive reply
            code, size = self._read()

            assert code == self.CTL_SUCCESS and size == 0

            # go to the next chunk
            data = data[chunk_len :]
            chunk_ptr += chunk_len

            percent = int(float(100) / (float(total_len) / chunk_ptr))

            if percent_prev != percent and progress_cb is not None and percent <= 100:

                progress_cb(percent)
                percent_prev = percent

        if progress_cb is not None:

            progress_cb(100)

    def bit_erase(self):

        # send bitstream erase request
        self._write(self.CTL_BIT_ERASE, 0)

        # receive reply (erase command might take a while)
        code, size = self._read(no_timeout = True)

        assert code == self.CTL_SUCCESS and size == 0

def main():

    parser = OptionParser()

    parser.add_option('--bit-load', dest = 'bit_load', default = None,
        help = 'load bitstream from file')

    parser.add_option('--bit-erase', dest = 'bit_erase', default = False, action = 'store_true',
        help = 'erase bitstream')

    parser.add_option('--rom-load', dest = 'rom_load', default = None,
        help = 'load payload ROM from file')

    parser.add_option('--rom-erase', dest = 'rom_erase', default = False, action = 'store_true',
        help = 'erase payload ROM')

    parser.add_option('--scan-start', dest = 'scan_start', default = None,
        help = 'override memory scan start address')

    parser.add_option('--scan-end', dest = 'scan_end', default = None,
        help = 'override memory scan end address')

    parser.add_option('--scan-step', dest = 'scan_step', default = None,
        help = 'override memory scan step')

    # parse command line
    options, _ = parser.parse_args()

    # default values
    scan_start = scan_end = scan_step = 0

    check_addr = lambda addr: addr >= PAGE_SIZE and addr < EFI_MAX_ADDR and addr % PAGE_SIZE == 0

    if not options.bit_erase and options.bit_load is None and \
       not options.rom_erase and options.rom_load is None:

        print('USAGE: evb_ctl.py --bit-load <bitstream_file_path>')
        print('       evb_ctl.py --rom-load <payload_file_path>')
        print('       evb_ctl.py --bit-erase')
        print('       evb_ctl.py --rom-erase')
        return -1

    # scan_end sanity check
    if options.scan_start is not None and options.scan_end is None:

        print('ERROR: --scan-end must be specified')
        return -1

    # scan_start sanity check
    if options.scan_end is not None and options.scan_start is None:

        print('ERROR: --scan-start must be specified')
        return -1

    # scan_step sanity check
    if options.scan_step is not None and options.scan_start is None:

        print('ERROR: --scan-start and --scan-end must be specified')
        return -1

    # rom_load stanity check
    if options.scan_start is not None and options.rom_load is None:

        print('ERROR: --rom-load must be specified')
        return -1

    # scan_start address check
    if options.scan_start is not None:

        try:

            # convert number
            scan_start = int(options.scan_start, 16)

        except ValueError:

            print('ERROR: Invalid --scan-start specified')
            return -1

        if not check_addr(scan_start):

            print('ERROR: Invalid --scan-start address specified')
            return -1

    # scan_end address check
    if options.scan_end is not None:

        try:

            # convert number
            scan_end = int(options.scan_end, 16)

        except ValueError:

            print('ERROR: Invalid --scan-end specified')
            return -1

        if not check_addr(scan_end):

            print('ERROR: Invalid --scan-end address specified')
            return -1

    # scan_step value check
    if options.scan_step is not None:

        try:

            # convert number
            scan_step = int(options.scan_step, 16)

        except ValueError:

            print('ERROR: Invalid --scan-step specified')
            return -1

        if not check_addr(scan_step):

            print('ERROR: Invalid --scan-step value specified')
            return -1

    # start-end stanity check
    if scan_start != 0 and scan_start <= scan_end:

        print('ERROR: --scan-start must be greather than --start-end')
        return -1

    print('[+] Opening device...')
    
    # open device
    dev = BitstreamLoader(force = True)

    if options.bit_load is not None or options.bit_erase:

        print('[+] Erasing memory...')

        # erase bitstream contents
        dev.bit_erase()

    if options.bit_load is not None:

        with open(options.bit_load, 'rb') as fd:
            
            # read bitstream contents from file
            data = fd.read()

        if len(data) > dev.BIT_MAX_SIZE:

            print('ERROR: Specified bitstream is too large')
            return -1

        print('[+] Loading %d bytes of bitstream...' % len(data))

        def progress_cb(percent):

            sys.stdout.flush()
            sys.stdout.write('\r[+] %d%% completed' % percent)

        # write bitstream to the device
        dev.bit_load(data, progress_cb = progress_cb)

        sys.stdout.write('\n')

    if options.rom_load is not None or options.rom_erase:

        print('[+] Erasing ROM...')

        # erase ROM contents
        dev.rom_erase()

    if options.rom_load is not None:

        with open(options.rom_load, 'rb') as fd:

            # read ROM contents from file
            data = fd.read()

        if data.find(HEADER_MAGIC) != 0:

            print('ERROR: Specified ROM is not PE image')
            return -1

        # check if we need struct SCAN_CONF
        if scan_start != 0:

            print('[+] Peparing struct SCAN_CONF...')

            # prepare struct SCAN_CONF data
            scan_conf = pack('QQQQ', SCAN_CONF_SIGN, scan_start, scan_end, scan_step)

            # modify payload image DOS header
            data = data[: SCAN_CONF_OFFS] + scan_conf + data[SCAN_CONF_OFFS + SCAN_CONF_SIZE :]

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

    print('[+] Done')

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#
