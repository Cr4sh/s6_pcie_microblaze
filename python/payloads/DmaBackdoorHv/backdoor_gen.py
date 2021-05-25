import sys, os
from optparse import OptionParser, make_option

try:

    import pdbparse

except ImportError:

    print('pdbparse is not installed')
    exit()

# Hyper-V backdoor body function
BACKDOOR_START = 'HyperVBackdoor'

# end marker
BACKDOOR_END = 'HyperVBackdoorEnd'

def symbol_offset(pdb, name):

    sym_list = pdb.STREAM_GSYM.globals
    sec_list = pdb.STREAM_SECT_HDR.sections

    for sym in sym_list:

        if not hasattr(sym, 'name') or not hasattr(sym, 'offset'):

            # not what we are looking for
            continue

        try:
            
            # get RVA of the section where symbol is located
            base = sec_list[sym.segment - 1].VirtualAddress

        except IndexError:

            continue

        if sym.name == name:

            # return RVA of needed symbol
            return base + sym.offset

    return None

def main():

    option_list = [

        make_option('-b', '--bin', dest = 'bin', default = None,
            help = 'DmaBackdoorHv_x64.efi binary file path'),

        make_option('-p', '--pdb', dest = 'pdb', default = None,
            help = 'DmaBackdoorHv_x64.pdb debug symbols file path'),

        make_option('-o', '--output', dest = 'output', default = None,
            help = 'file path to save extracted backdoor code')
    ]

    parser = OptionParser(option_list = option_list)
    options, args = parser.parse_args()

    if options.bin is None or options.pdb is None or options.output is None:

        print('Insufficient arguments, try --help')
        return -1

    print('[+] Loading debug symbols from %s' % options.pdb)

    # load debug symbols file
    pdb = pdbparse.parse(options.pdb)

    # obtain needed symbols
    bd_start = symbol_offset(pdb, BACKDOOR_START)
    bd_end = symbol_offset(pdb, BACKDOOR_END)

    if bd_start is None:

        print('ERROR: Symbol %s() is not found' % BACKDOOR_START)
        return -1

    if bd_end is None:

        print('ERROR: Symbol %s() is not found' % BACKDOOR_END)
        return -1

    assert bd_end > bd_start

    print('[+] %s() symbol offset is 0x%x' % (BACKDOOR_START, bd_start))
    print('[+] %s() symbol offset is 0x%x' % (BACKDOOR_END, bd_end))

    with open(options.bin, 'rb') as fd:

        # read backdoor body from the binary
        fd.seek(bd_start)
        data = fd.read(bd_end - bd_start)

    with open(options.output, 'wb') as fd:

        # write backdoor body to the output file
        fd.write(data)

    print('[+] %d bytes of backdoor body written into the %s' % (len(data), options.output))

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#
