#!/usr/bin/env python

import sys, os

FLASH_SIZE = 8 * 1024 * 1024

FLASHROM = 'flashrom'

# needed files
LAYOUT = 'spi_rom.layout'
TEMP = 'spi_rom.tmp'

def main():

    if len(sys.argv) >= 3:

        prog = sys.argv[1]
        path = sys.argv[2]

        remote = sys.argv[3] if len(sys.argv) >= 4 else None

        size = os.path.getsize(path)

        with open(LAYOUT, 'wb') as fd:

            # write flash layout file
            fd.write('%.8x:%.8x main' % (0, size - 1))

        with open(TEMP, 'wb') as fd:

            with open(path, 'rb') as fd_s:

                fd.write(fd_s.read())

            # write padding
            fd.write('\0' * (FLASH_SIZE - size))

        if remote:

            # copy needed files to remote machine
            assert os.system('scp spi_rom.* %s:/tmp' % remote) == 0

            # execute flashrom
            return os.system('ssh %s %s -p %s --layout /tmp/%s --image main -w /tmp/%s' % \
                             (remote, FLASHROM, prog, LAYOUT, TEMP))

        else:

            return os.system('%s -p %s --layout %s --image main -w %s' % \
                             (FLASHROM, prog, LAYOUT, TEMP))

    else:

        print('USAGE: flash_to_spi.py <programmer> <bin_file> [remote]')

    return -1

if __name__ == '__main__':

    sys.exit(main())

#
# EoF
#

