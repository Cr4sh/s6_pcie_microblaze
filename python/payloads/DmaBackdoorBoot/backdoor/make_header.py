#!/usr/bin/env python

import sys, os
from optparse import OptionParser

LINE_WIDTH = 16

def main():

    parser = OptionParser()

    parser.add_option("-i", "--input-path", dest = "input", default = None,
        help = "input binary path")

    parser.add_option("-o", "--output-path", dest = "output", default = None,
        help = "output C header path")

    parser.add_option("-n", "--name", dest = "name", default = None,
        help = "variable name")

    options, _ = parser.parse_args()

    if options.input is None or options.output is None or options.name is None:

        print('USAGE: trustlet_prepare.py -i <input_file> -o <output_file> -n <var_name>')
        return -1

    print('[+] Loading input file "%s"' % options.input)

    with open(options.input, 'rb') as fd:

        # read binary contents
        data = fd.read()

    header = '// %d bytes readed from %s\n' % (len(data), os.path.basename(options.input))
    header += 'unsigned char %s[] = \n{\n' % options.name
    lines = []

    while len(data) > 0:

        lines.append(', '.join(map(lambda b: '0x%.2x' % ord(b), data[: LINE_WIDTH])))
        data = data[LINE_WIDTH :]

    header += ',\n'.join(lines)
    header += '\n};\n'

    print('[+] Saving output file "%s"' % options.output)

    with open(options.output, 'wb') as fd:

        # write C header contents
        fd.write(header)

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#