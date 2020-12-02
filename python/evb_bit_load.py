#!/usr/bin/env python

import sys, os
from optparse import OptionParser

from pcie_lib import *

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

    parser.add_option('-l', '--load', dest = 'load', default = None,
        help = 'load bitstream from file')

    parser.add_option('-e', '--erase', dest = 'erase', default = False, action = 'store_true',
        help = 'erase bitstream')    

    # parse command line
    options, _ = parser.parse_args()

    if not options.erase and options.load is None :

        print('USAGE: bit_load.py --load <bitstream_file_path>')
        print('       bit_load.py --erase')
        return -1

    print('[+] Opening device...')
    
    # open device
    dev = BitstreamLoader(force = True)    

    if options.load is not None or options.erase:

        print('[+] Erasing memory...')

        # erase bitstream contents
        dev.bit_erase()

    if options.load is not None:

        with open(options.load, 'rb') as fd:
            
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

    print('[+] Done')

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#
