#!/usr/bin/env python

import sys, os
from optparse import OptionParser

try:

    import pefile

except ImportError:

    print('ERROR: pefile is not installed')
    exit(-1)

# digital signature blob
SIGNATURE = os.path.join(os.path.dirname(sys.argv[0]), 'signature.bin')

def main():

    parser = OptionParser()

    parser.add_option("-i", "--input-path", dest = "input", default = None,
        help = "input binary path")

    parser.add_option("-o", "--output-path", dest = "output", default = None,
        help = "output binary path")

    options, _ = parser.parse_args()

    if options.input is None or options.output is None:

        print('USAGE: trustlet_prepare.py -i <input_file> -o <output_file>')
        return -1

    print('[+] Loading input file "%s"' % options.input)

    with open(SIGNATURE, 'rb') as fd:

        # read PE signature blob
        sign = fd.read()

    # load input file
    pe = pefile.PE(options.input)

    # get last section
    section = pe.sections[-1]

    # get security directory
    sec_dir = pe.OPTIONAL_HEADER.DATA_DIRECTORY[pefile.DIRECTORY_ENTRY['IMAGE_DIRECTORY_ENTRY_SECURITY']]

    # calculate signature address and size
    sign_addr = section.PointerToRawData + section.SizeOfRawData 
    sign_size = len(sign)

    # update security directory
    sec_dir.VirtualAddress = sign_addr
    sec_dir.Size = sign_size

    # save resulting PE file
    data = pe.write() + sign

    print('[+] Saving output file "%s"' % options.output)

    with open(options.output, 'wb') as fd:

        # write output file
        fd.write(data)

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#
