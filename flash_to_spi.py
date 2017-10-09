#!/usr/bin/env python

import sys, os

FLASH_SIZE = 8 * 1024 * 1024

FLASHROM = 'flashrom'
PROGRAMMER = 'linux_spi:dev=/dev/spidev1.0'

LAYOUT = 'spi_rom.layout'
TEMP = 'spi_rom.tmp'

REMOTE = None

def main():

    if len(sys.argv) >= 2:

        path = sys.argv[1]
        size = os.path.getsize(path)

        with open(LAYOUT, 'wb') as fd:

            fd.write('%.8x:%.8x main' % (0, size - 1))

        with open(TEMP, 'wb') as fd:

            with open(path, 'rb') as fd_s:

                fd.write(fd_s.read())

            fd.write('\0' * (FLASH_SIZE - size))

        if REMOTE:

            assert os.system('scp spi_rom.* %s:/tmp' % REMOTE) == 0

            return os.system('ssh %s %s -p %s --layout /tmp/%s --image main -w /tmp/%s' % \
                             (REMOTE, FLASHROM, PROGRAMMER, LAYOUT, TEMP))

        else:

            return os.system('%s -p %s --layout %s --image main -w %s' % \
                             (FLASHROM, PROGRAMMER, LAYOUT, TEMP))

    else:

        print('USAGE: flash_to_spi.py <bin_file>')

    return -1

if __name__ == '__main__':

    sys.exit(main())

#
# EoF
#

