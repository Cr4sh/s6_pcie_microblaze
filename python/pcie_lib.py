#!/usr/bin/env python

import sys, os, time, random, socket, select, signal, unittest
from struct import pack, unpack
from threading import Thread
from multiprocessing.dummy import Pool as ThreadPool
from abc import ABCMeta, abstractmethod

from pcie_lib_config import *

PAGE_SIZE = 0x1000

align_up = lambda x, a: ((x + a - 1) // a) * a
align_down = lambda x, a: (x // a) * a

#
# configuration space registers
#
CFG_VENDOR_ID           = 0x00
CFG_DEVICE_ID           = 0x02
CFG_COMMAND             = 0x04
CFG_STATUS              = 0x06
CFG_REVISION            = 0x08
CFG_CLASS_PROG          = 0x09
CFG_CLASS_DEVICE        = 0x0a
CFG_CACHE_LINE_SIZE     = 0x0c
CFG_LATENCY_TIMER       = 0x0d
CFG_HEADER_TYPE         = 0x0e
CFG_BIST                = 0x0f
CFG_BASE_ADDRESS_0      = 0x10
CFG_BASE_ADDRESS_1      = 0x14
CFG_BASE_ADDRESS_2      = 0x18
CFG_BASE_ADDRESS_3      = 0x1c
CFG_BASE_ADDRESS_4      = 0x20
CFG_BASE_ADDRESS_5      = 0x24
CFG_CARDBUS_CIS         = 0x28
CFG_SUBSYSTEM_VENDOR_ID = 0x2c
CFG_SUBSYSTEM_ID        = 0x2e
CFG_ROM_ADDRESS         = 0x30
CFG_INTERRUPT_LINE      = 0x3c
CFG_INTERRUPT_PIN       = 0x3d
CFG_MIN_GNT             = 0x3e
CFG_MAX_LAT             = 0x3f

cfg_regs = {

            CFG_VENDOR_ID: ( 2, 'VENDOR_ID'           ),
            CFG_DEVICE_ID: ( 2, 'DEVICE_ID'           ),
              CFG_COMMAND: ( 2, 'COMMAND'             ),
               CFG_STATUS: ( 2, 'STATUS'              ),
             CFG_REVISION: ( 1, 'REVISION'            ),
           CFG_CLASS_PROG: ( 1, 'CLASS_PROG'          ),
         CFG_CLASS_DEVICE: ( 2, 'CLASS_DEVICE'        ),
      CFG_CACHE_LINE_SIZE: ( 1, 'CACHE_LINE_SIZE'     ),
        CFG_LATENCY_TIMER: ( 1, 'LATENCY_TIMER'       ),
          CFG_HEADER_TYPE: ( 1, 'HEADER_TYPE'         ),
                 CFG_BIST: ( 1, 'BIST'                ),
       CFG_BASE_ADDRESS_0: ( 4, 'BASE_ADDRESS_0'      ),
       CFG_BASE_ADDRESS_1: ( 4, 'BASE_ADDRESS_1'      ),
       CFG_BASE_ADDRESS_2: ( 4, 'BASE_ADDRESS_2'      ),
       CFG_BASE_ADDRESS_3: ( 4, 'BASE_ADDRESS_3'      ),
       CFG_BASE_ADDRESS_4: ( 4, 'BASE_ADDRESS_4'      ),
       CFG_BASE_ADDRESS_5: ( 4, 'BASE_ADDRESS_5'      ),
          CFG_CARDBUS_CIS: ( 4, 'CARDBUS_CIS'         ),
  CFG_SUBSYSTEM_VENDOR_ID: ( 2, 'SUBSYSTEM_VENDOR_ID' ),
         CFG_SUBSYSTEM_ID: ( 2, 'SUBSYSTEM_ID'        ),
          CFG_ROM_ADDRESS: ( 4, 'ROM_ADDRESS'         ),
       CFG_INTERRUPT_LINE: ( 1, 'INTERRUPT_LINE'      ),
        CFG_INTERRUPT_PIN: ( 1, 'INTERRUPT_PIN'       ),
              CFG_MIN_GNT: ( 1, 'MIN_GNT'             ),
              CFG_MAX_LAT: ( 1, 'MAX_LAT'             ) }

FMT_3_NO_DATA = 0
FMT_4_NO_DATA = 1
FMT_3_DATA    = 2
FMT_4_DATA    = 3 

tlp_types = { 0x00: 'MRd32',   0x20: 'MRd64',
              0x01: 'MRdLk32', 0x21: 'MRdLk64', 
              0x40: 'MWr32',   0x60: 'MWr64',
              0x02: 'IORd',    0x42: 'IOWr',
              0x04: 'CfgRd0',  0x44: 'CfgWr0',
              0x05: 'CfgRd1',  0x45: 'CfgWr1',
              0x0A: 'Cpl',     0x4A: 'CplD',
              0x0B: 'CplLk',   0x4B: 'CplLkD' }


dev_id_decode = lambda val: ((val >> 8) & 0xff, (val >> 3) & 0x1f, (val >> 0) & 0x07)
dev_id_encode = lambda bus, dev, func: ((bus << 8) | (dev << 3) | (func << 0))
dev_id_str    = lambda bus, dev, func: '%.2x:%.2x.%x' % (bus, dev, func)


def tlp_type_name(dw0): 

    return tlp_types[(dw0 >> 24) & 0xff]


def tlp_type_from_name(name):

    for key, val in tlp_types.items():

        if val == name:

            return ((key >> 5) & 0x3), ((key >> 0) & 0x1f)


def endpoint_init(*args, **kvargs):

    if Conf.device_type == DEVICE_TYPE_TCP:

        # TCP/IP transport
        return EndpointTcp(*args, **kvargs)

    elif Conf.device_type == DEVICE_TYPE_SERIAL:

        # serial port transport
        return EndpointSerial(*args, **kvargs)

    elif Conf.device_type == DEVICE_TYPE_UIO:

        # UIO transport for Zynq based design
        return EndpointUIO(*args, **kvargs)

    else:

        raise(Exception('Unknown device type'))


def hexdump(data, width = 16, addr = 0):

    ret = ''

    def quoted(data):

        # replace non-alphanumeric characters
        return ''.join(map(lambda b: b if b.isalnum() else '.', data))

    while data:

        line = data[: width]
        data = data[width :]

        # put hex values
        s = map(lambda b: '%.2x' % ord(b), line)
        s += [ '  ' ] * (width - len(line))

        # put ASCII values
        s = '%s | %s' % (' '.join(s), quoted(line))

        if addr is not None: 

            # put address
            s = '%.8x: %s' % (addr, s)
            addr += len(line)

        ret += s + '\n'

    return ret


class Device(object):

    __metaclass__ = ABCMeta

    class Error(Exception): 

        pass

    class Timeout(Exception): 

        pass    

    @abstractmethod
    def read(self, size, timeout = None):

        pass

    @abstractmethod
    def write(self, data):

        pass

    @abstractmethod
    def close(self):

        pass


class Socket(Device):

    def __init__(self, addr):

        assert addr is not None

        self.addr = addr
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        self.sock.connect(addr)

    def read(self, size, timeout = None):

        ret = ''

        assert self.sock is not None
        
        # check if there's any data to receive
        fd_read, fd_write, fd_err = select.select([ self.sock ], [], [], timeout)

        if self.sock in fd_err:

            # error occurred
            raise(self.Error('Connection error'))

        if not self.sock in fd_read:

            # timeout occurred
            raise(self.Timeout('Socket read timeout'))         

        while len(ret) < size:
            
            # receive needed amount of data
            data = self.sock.recv(size - len(ret))
            if len(data) == 0:

                # connection was closed by remote host
                raise(self.Error('Connection closed')) 

            ret += data            

        return ret

    def write(self, data):

        assert self.sock is not None        

        self.sock.sendall(data)

    def close(self):

        if self.sock is not None:

            self.sock.close()
            self.sock = None


class Serial(Device):

    def __init__(self, device, baud):

        try:

            import serial

        except ImportError:

            raise(Exception('pyserial is not installed'))

        self.device = serial.Serial(device, baud, timeout = 0)
        self.device.flush()

    def read(self, size, timeout = None):

        ret, started = '', time.time()

        assert self.device is not None

        while len(ret) < size:

            if timeout is not None:

                if time.time() - started > timeout:

                    # timeout occurred
                    raise(self.Timeout('Serial port read timeout'))
            
            # receive needed amount of data
            ret += self.device.read(size - len(ret))

        return ret

    def write(self, data):

        assert self.device is not None        

        while len(data) > 0:
            
            size = self.device.write(data)
            data = data[size :]

    def close(self):

        if self.device is not None:

            self.device.close()
            self.device = None


class Endpoint(object):    

    RECV_TIMEOUT = 3

    ROM_CHUNK_LEN = 0x80

    status_bus_id = lambda self, s: s & 0xffff

    class ErrorNotReady(Exception): 

        pass

    class ErrorTimeout(Exception): 

        pass

    def __init__(self, bus_id = None, force = False, timeout = None):

        self.bus_id = bus_id
        self.timeout = self.RECV_TIMEOUT if timeout is None else timeout

        # check connection        
        self.ping()

        if self.bus_id is None:

            # obtain bus id from the device
            bus_id = self.get_bus_id()

            if bus_id != 0:

                self.bus_id = dev_id_decode(bus_id)

            elif not force:

                raise(self.ErrorNotReady('PCI-E endpoint is not configured by root complex yet'))  

    def get_bus_id(self):

        return self.status_bus_id(self.get_status()) 

    def set_timeout(self, timeout):

        self.timeout = timeout

    def cfg_reg(self, cfg_reg):

        try:

            # get register name and size
            cfg_size, cfg_name = cfg_regs[cfg_reg]

        except KeyError:

            raise(Exception('Unknown configuration space register'))

        # read register value
        return self.cfg_read(cfg_reg, cfg_size = cfg_size)

    def cfg_read_1(self, cfg_addr): 

        return self.cfg_read(cfg_addr, cfg_size = 1)

    def cfg_read_2(self, cfg_addr): 

        return self.cfg_read(cfg_addr, cfg_size = 2)

    def cfg_read_4(self, cfg_addr): 

        return self.cfg_read(cfg_addr, cfg_size = 4)

    @abstractmethod
    def cfg_read(self, cfg_addr, cfg_size = 4):

        raise(NotImplementedError())

    @abstractmethod
    def set_resident(self, on):

        pass

    @abstractmethod
    def set_rom_log(self, on):

        pass

    @abstractmethod
    def test(self, test_size):

        pass

    @abstractmethod
    def ping(self):

        pass

    @abstractmethod
    def reset(self):

        pass

    @abstractmethod
    def get_status(self):

        pass

    @abstractmethod
    def read(self):

        pass

    @abstractmethod
    def write(self, data):

        pass

    @abstractmethod
    def rom_load(self, data, progress_cb = None):

        pass

    @abstractmethod
    def rom_erase(self):

        pass

    @abstractmethod
    def rom_size(self):

        pass

    @abstractmethod
    def close(self):

        pass

    
class EndpointStream(Endpoint):

    ENV_DEVICE = 'DEVICE'

    #
    # protocol control codes
    #
    CTL_PING                = 0
    CTL_RESET               = 1
    CTL_STATUS              = 2
    CTL_TLP_SEND            = 3
    CTL_TLP_RECV            = 4
    CTL_SUCCESS             = 5
    CTL_ERROR_FAILED        = 6
    CTL_ERROR_TIMEOUT       = 7
    CTL_CONFIG              = 8
    CTL_TEST                = 9
    CTL_RESIDENT_ON         = 10 
    CTL_RESIDENT_OFF        = 11
    CTL_ROM_WRITE           = 12
    CTL_ROM_ERASE           = 13
    CTL_ROM_LOG_ON          = 14
    CTL_ROM_LOG_OFF         = 15
    CTL_ROM_SIZE            = 16
        
    def _read(self, no_timeout = False):

        return unpack('=BB', self.device.read(1 + 1, timeout = None if no_timeout else self.timeout))

    def _write(self, *args):

        self.device.write(pack('=BB', *args))    

    def set_resident(self, on):

        # send resident mode configuration request
        self._write(self.CTL_RESIDENT_ON if on else self.CTL_RESIDENT_OFF, 0)

    def set_rom_log(self, on):

        # send ROM access log configuration request
        self._write(self.CTL_ROM_LOG_ON if on else self.CTL_ROM_LOG_OFF, 0)

    def test(self, test_size):

        # send test request
        self._write(self.CTL_TEST, test_size)

        # receive reply
        code, size = self._read()

        assert code == self.CTL_SUCCESS and size == test_size

        # receive reply data
        return self.device.read(size, timeout = self.timeout)

    def ping(self):

        # send ping request
        self._write(self.CTL_PING, 0)

        try:

            # receive reply
            code, size = self._read()

        except self.device.Timeout:

            raise(self.ErrorTimeout('Device timeout occurred'))

        assert code == self.CTL_SUCCESS and size == 0

    def reset(self):

        # send reset request
        self._write(self.CTL_RESET, 0)

        # receive reply
        code, size = self._read()

        assert code == self.CTL_SUCCESS and size == 0

    def get_status(self):

        # send get status request
        self._write(self.CTL_STATUS, 0)

        # receive reply
        code, size = self._read()

        assert code == self.CTL_SUCCESS and size == 4

        # receive reply data
        return unpack('<I', self.device.read(size, timeout = self.timeout))[0]    

    def read(self):

        ret = []

        # send read TLP request
        self._write(self.CTL_TLP_RECV, 0)

        try:

            # receive reply
            code, size = self._read()

        except self.device.Timeout:

            raise(self.ErrorTimeout('TLP read timeout occurred'))

        if code == self.CTL_ERROR_TIMEOUT:

            raise(self.ErrorTimeout('TLP read timeout occurred'))

        assert code == self.CTL_TLP_RECV
        assert size > 8 and size % 4 == 0

        # receive reply data
        data = self.device.read(size, timeout = self.timeout)

        for i in range(0, size / 4):

            ret.append(unpack('<I', data[i * 4 : (i + 1) * 4])[0])        

        return ret

    def write(self, data):

        assert len(data) > 2

        # TLP send request
        buff = pack('=BB', self.CTL_TLP_SEND, len(data) * 4)

        for i in range(0, len(data)):

            # send request data
            buff += pack('<I', data[i])

        self.device.write(buff)

        # receive reply
        code, size = self._read()

        assert code == self.CTL_SUCCESS and size == 0    

    def cfg_read(self, cfg_addr, cfg_size = 4):

        assert cfg_size in [ 1, 2, 4 ]

        # get register number from register address
        reg_num = cfg_addr / 4
        reg_off = cfg_addr % 4

        data = ''

        for i in range(0, 2):

            # send config space read request
            self.device.write(pack('<BBI', self.CTL_CONFIG, 4, reg_num + i))

            # receive reply
            code, size = self._read()

            assert code == self.CTL_SUCCESS and size == 4

            # receive reply data
            data += self.device.read(size, timeout = self.timeout)

        # get register value from readed data
        data = data[reg_off : reg_off + cfg_size]

        return unpack('<' + { 1: 'B', 2: 'H', 4: 'I' }[cfg_size], data)[0]    

    def rom_load(self, data, progress_cb = None):

        chunk_ptr = 0
        chunk_len = self.ROM_CHUNK_LEN
        total_len, percent_prev = len(data), None

        # sanity check
        assert len(data) < self.rom_size()

        while len(data) > 0:
            
            chunk = data[: chunk_len]

            # ROM write request
            buff = pack('<BBI', self.CTL_ROM_WRITE, len(chunk) + 4, chunk_ptr) + chunk

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

    def rom_erase(self):

        # send ROM erase request
        self._write(self.CTL_ROM_ERASE, 0)

        # receive reply (erase command might take a while)
        code, size = self._read(no_timeout = True)

        assert code == self.CTL_SUCCESS and size == 0   

    def rom_size(self):

        # send get ROM size request
        self._write(self.CTL_ROM_SIZE, 0)

        # receive reply
        code, size = self._read()

        assert code == self.CTL_SUCCESS and size == 4

        # receive reply data
        return unpack('<I', self.device.read(size, timeout = self.timeout))[0]    


class EndpointTcp(EndpointStream):

    def __init__(self, device = None, *args, **kvargs):

        if device is None:

            try:

                # obtain device address from environment variable
                host, port = os.getenv(self.ENV_DEVICE).strip().split(':')
                device = ( host, int(port) )

            except: pass

        # initialize TCP/IP based device
        self.device = Socket(addr = Conf.addr if device is None else device)

        # initialize base class
        super(EndpointTcp, self).__init__(*args, **kvargs)


class EndpointSerial(EndpointStream):

    def __init__(self, device = None, *args, **kvargs):

        if device is None:

            try:

                # obtain device name from environment variable
                device = os.getenv(self.ENV_DEVICE).strip()

            except: pass            

        # initialize serial based device
        self.device = Serial(device = Conf.device if device is None else device, baud = Conf.baud)

        # initialize base class
        super(EndpointSerial, self).__init__(*args, **kvargs)


class EndpointUIO(Endpoint):

    MAX_TLP_LEN = PAGE_SIZE / 4

    # Linux UIO device names
    UIO_NAME_MEM = 'scratch_mem'
    UIO_NAME_DMA_0 = 'dma_0'
    UIO_NAME_DMA_1 = 'dma_1'
    UIO_NAME_GPIO = 'gpio'

    def __init__(self, device = None, *args, **kvargs):

        import linux_uio
        import linux_axi_dma

        # open AXI DMA engines
        self.dma_tlp = linux_axi_dma.LinuxAxiDMA(self.UIO_NAME_DMA_0)
        self.dma_cfg = linux_axi_dma.LinuxAxiDMA(self.UIO_NAME_DMA_1)

        # open I/O memory
        self.mem = linux_uio.LinuxUIO(self.UIO_NAME_MEM)
        self.buff = self.mem.as_ndarray(dtype = 'uint32')

        # initialize recv buffer for TLPs
        self.buff_rx = self.buff[: self.MAX_TLP_LEN]

        # initialize send buffer for TLPs
        self.buff_tx = self.buff[self.MAX_TLP_LEN : self.MAX_TLP_LEN * 2]

        # open AXI GPIO
        self.gpio = linux_uio.LinuxUIO(self.UIO_NAME_GPIO)

        # initialize base class
        super(EndpointUIO, self).__init__(*args, **kvargs)

    def get_status(self):

        # read PCI-E device ID from the GPIO
        return self.gpio.as_ndarray(1, 'uint32')[0]

    def ping(self):

        pass

    def reset(self):

        self.dma_tlp.reset()
        self.dma_cfg.reset()

    def read(self):

        try:

            # receive data
            size = self.dma_tlp.recv(self.mem.phys_buff(self.buff_rx), timeout = self.timeout)

        except self.dma_tlp.Timeout:

            raise(self.ErrorTimeout('TLP read timeout occurred'))        

        assert size >= 12
        assert size % 4 == 0

        return self.buff_rx[: size / 4]

    def write(self, data):

        # copy TLP into the send buffer
        self.buff_tx[: len(data)] = data

        try:

            # send data
            self.dma_tlp.send(self.mem.phys_buff(self.buff_tx[: len(data)]), timeout = self.timeout)

        except self.dma_tlp.Timeout:

            raise(self.ErrorTimeout('TLP write timeout occurred'))     

    def cfg_read(self, cfg_addr, cfg_size = 4):

        assert cfg_size in [ 1, 2, 4 ]

        # get register number from register address
        reg_num = cfg_addr / 4
        reg_off = cfg_addr % 4

        data = ''

        for i in range(0, 2):

            addr = reg_num + i

            # put address two times to assert cfg_mgmt_rd_en for two clock cycles
            self.buff_tx[: 2] = [ addr ] * 2

            try:

                # send configuration spcae read request
                self.dma_cfg.send(self.mem.phys_buff(self.buff_tx[: 2]), timeout = self.timeout)

                # receive data
                size = self.dma_cfg.recv(self.mem.phys_buff(self.buff_rx), timeout = self.timeout)

            except self.dma_cfg.Timeout:

                raise(self.ErrorTimeout('Config space access timeout occurred'))

            assert size == 4

            data += pack('I', self.buff_rx[0])

        # get register value from readed data
        data = data[reg_off : reg_off + cfg_size]

        return unpack('<' + { 1: 'B', 2: 'H', 4: 'I' }[cfg_size], data)[0]       

    def close(self):

        self.dma_tlp.close()
        self.dma_cfg.close()

        self.mem.close()
        self.gpio.close()

    def set_resident(self, on):

        raise(NotImplementedError())

    def set_rom_log(self, on):

        raise(NotImplementedError())

    def test(self, test_size):

        raise(NotImplementedError())    

    def rom_load(self, data, progress_cb = None):

        raise(NotImplementedError())

    def rom_erase(self):

        raise(NotImplementedError())

    def rom_size(self):

        raise(NotImplementedError())


class EndpointTest(unittest.TestCase):

    TEST_ADDR = 0x1000

    def test_link(self):

        dev = endpoint_init()  

        # MRd TLP        
        tlp_tx = [ 0x20000001,
                   0x000000ff | (dev.get_bus_id() << 16),                   
                   0x00000000,
                   self.TEST_ADDR ]    

        to_str = lambda tlp: ' '.join(map(lambda dw: '0x%.8x' % dw, tlp))

        print('TLP TX: %s\n' % to_str(tlp_tx))

        dev.write(tlp_tx)

        tlp_rx = dev.read()

        print('TLP RX: %s\n' % to_str(tlp_rx))


class TransactionLayer(object):

    ENV_DEBUG_TLP = 'DEBUG_TLP'

    #
    # Maximum bytes of data per each MWr and MRd TLP
    #
    MEM_WR_TLP_LEN = 0x04
    MEM_RD_TLP_LEN = 0x40

    # align memory reads and writes
    MEM_ALIGN = 0x4

    mem_write_1 = lambda self, addr, v: self.mem_write(addr, pack('B', v))
    mem_write_2 = lambda self, addr, v: self.mem_write(addr, pack('H', v))
    mem_write_4 = lambda self, addr, v: self.mem_write(addr, pack('I', v))
    mem_write_8 = lambda self, addr, v: self.mem_write(addr, pack('Q', v))

    mem_read_1 = lambda self, addr: unpack('B', self.mem_read(addr, 1))[0]
    mem_read_2 = lambda self, addr: unpack('H', self.mem_read(addr, 2))[0]
    mem_read_4 = lambda self, addr: unpack('I', self.mem_read(addr, 4))[0]
    mem_read_8 = lambda self, addr: unpack('Q', self.mem_read(addr, 8))[0]

    cfg_read_1 = lambda self, cfg_addr: self.ep.cfg_read_1(cfg_addr)
    cfg_read_2 = lambda self, cfg_addr: self.ep.cfg_read_2(cfg_addr)
    cfg_read_4 = lambda self, cfg_addr: self.ep.cfg_read_4(cfg_addr)    

    cfg_read = lambda self, cfg_addr, cfg_size: self.ep.cfg_read(cfg_addr, cfg_size)

    cfg_reg = lambda self, cfg_reg: self.ep.cfg_reg(cfg_reg)

    class ErrorBadCompletion(Exception): 

        pass

    class Packet(object): 

        # bus:device.function
        src_name = lambda self, src: '%.2x:%.2x.%x' % ((src >> 8) & 0xff,
                                                       (src >> 3) & 0x1f,
                                                       (src >> 0) & 0x07)

        get_data = lambda self: pack('>' + ('I' * self.h_length), *self.data)

        def __init__(self, tlp = None):

            if tlp is not None: self.decode(tlp)

        def decode(self, tlp):
            
            self.tlp = tlp
            self.tlp_size = 1

            assert len(tlp) > 0 

            # decode TLP header
            self.h_prefix = (tlp[0] >> 31) & 0x1
            self.h_format = (tlp[0] >> 29) & 0x3
            self.h_type   = (tlp[0] >> 24) & 0x1f
            self.h_length = (tlp[0] >>  0) & 0x3ff

            type_name = tlp_type_name(tlp[0])

            if hasattr(self, 'tlp_type'):

                assert self.tlp_type == type_name

            else:

                self.tlp_type = type_name

            # TODO: TLP prefixes decoding
            assert self.h_prefix == 0

            # check TLP size
            if   self.h_format == FMT_3_NO_DATA: self.tlp_size += 2
            elif self.h_format == FMT_4_NO_DATA: self.tlp_size += 3
            elif self.h_format == FMT_3_DATA:    self.tlp_size += 2 + self.h_length
            elif self.h_format == FMT_4_DATA:    self.tlp_size += 3 + self.h_length

            assert len(tlp) == self.tlp_size            

            # determinate header length
            self.header_size = 3 if self.h_format in [ FMT_3_NO_DATA, 
                                                       FMT_3_DATA ] else 4

            self.header = tlp[0 : self.header_size]

            if self.h_format in [ FMT_3_NO_DATA, FMT_4_NO_DATA ]:

                assert len(tlp) == self.header_size

                self.data = None

            else:

                self.data = tlp[self.header_size :]        

            # decode the rest of the TLP header           
            self.h_req_id = dev_id_decode((tlp[1] >> 16) & 0xffff)

        def decode_addr(self):

            self.h_tag = (self.tlp[1] >> 8) & 0xff
            self.h_last_dw_be = (self.tlp[1] >> 4) & 0xf
            self.h_first_dw_be = (self.tlp[1] >> 0) & 0xf

            if self.header_size == 3:

                # 32-bit address
                self.addr = (self.tlp[2] & 0xfffffffc)

            elif self.header_size == 4:

                # 64-bit address
                self.addr = (self.tlp[3] & 0xfffffffc) | (self.tlp[2] << 32)

        def decode_cfg_addr(self):

            self.h_tag = (self.tlp[1] >> 8) & 0xff
            self.h_last_dw_be = (self.tlp[1] >> 4) & 0xf
            self.h_first_dw_be = (self.tlp[1] >> 0) & 0xf

            self.h_device = dev_id_decode((self.tlp[2] >> 16) & 0xffff)
            self.h_reg = (self.tlp[2] >> 2) & 0x3ff

        def decode_completion(self):

            self.h_completer = dev_id_decode((self.tlp[1] >> 16) & 0xffff)
            self.h_requester = dev_id_decode((self.tlp[2] >> 16) & 0xffff)
            self.h_byte_count = (self.tlp[1] >> 0) & 0xfff
            self.h_tag = (self.tlp[2] >> 8) & 0xff

        def encode(self):

            assert self.tlp_type in tlp_types.values()            

            self.h_prefix = 0;
            self.h_format, self.h_type = tlp_type_from_name(self.tlp_type)            

            # determinate header length            
            self.header_size = 3 if self.h_format in [ FMT_3_NO_DATA, 
                                                       FMT_3_DATA ] else 4

            self.tlp = []
            self.tlp_size = self.header_size

            if self.h_format in [ FMT_3_DATA, FMT_4_DATA ]:

                self.tlp_size += self.h_length            

            self.tlp.append((self.h_prefix << 31) | \
                            (self.h_format << 29) | \
                            (self.h_type   << 24) | \
                            (self.h_length << 0))

        def encode_addr(self):

            self.h_first_dw_be = 0xf                
            self.h_last_dw_be = 0xf if self.header_size == 4 else 0

            self.tlp.append((dev_id_encode(*self.h_req_id) << 16) |
                            (self.h_tag << 8) |
                            (self.h_last_dw_be << 4) |
                            (self.h_first_dw_be << 0))

            if self.header_size == 3:                

                assert self.addr & 0xfffffffc == self.addr
                assert self.addr < 0xffffffff

                # 32-bit address
                self.tlp.append(self.addr)

            elif self.header_size == 4:

                assert self.addr & 0xfffffffffffffffc == self.addr
                assert self.addr < 0xffffffffffffffff

                # 64-bit address
                self.tlp.append(self.addr >> 32)
                self.tlp.append(self.addr & 0xffffffff)

            # update header contents
            self.header = self.tlp[0 : self.header_size]

        def encode_cfg_addr(self):

            self.h_first_dw_be = 0xf            
            self.h_last_dw_be = 0

            self.tlp.append((dev_id_encode(*self.h_req_id) << 16) |
                            (self.h_tag << 8) |
                            (self.h_last_dw_be << 4) |
                            (self.h_first_dw_be << 0))

            self.tlp.append((dev_id_encode(*self.h_device) << 16) |
                            (self.h_reg << 2))

            # update header contents
            self.header = self.tlp[0 : self.header_size]

        def log(self, name = '', quiet = False):

            tlp_str = lambda data: ' '.join(map(lambda dw: '0x%.8x' % dw, data))

            format = (' ' * len(name)) + '      %s\n'

            s = 'TLP %s: size = 0x%.2x, source = %s, type = %s\n' % \
                (name, self.tlp_size, dev_id_str(*self.h_req_id), self.tlp_type)            

            if hasattr(self, 'log_ex'):

                # log type specific info
                s += format % self.log_ex()

            # TLP header raw dump
            s += '\n'
            s += format % tlp_str(self.header)

            if self.data is not None:

                # TLP data raw dump
                s += format % tlp_str(self.data)

            if not quiet: sys.stdout.write('%s\n' % s)

            return s


    class PacketMRd32(Packet): 

        tlp_type = 'MRd32'

        def __init__(self, req = None, addr = None, bytes_read = None, tag = None, tlp = None):

            TransactionLayer.Packet.__init__(self, tlp = tlp)

            if tlp is None:

                self.data = None

                self.h_tag = random.randrange(0, 0xff) if tag is None else tag
                self.h_req_id, self.addr, self.bytes_read = req, addr, bytes_read

                # create raw TLP from specified arguments
                self.encode()

        def decode(self, tlp):

            # decode packet header
            TransactionLayer.Packet.decode(self, tlp)

            # decode address word
            self.decode_addr()

            self.bytes_read = self.h_length * 4

        def encode(self):

            assert self.bytes_read % 4 == 0

            self.h_length = self.bytes_read / 4

            # encode packet header
            TransactionLayer.Packet.encode(self)

            # encode address dword
            self.encode_addr()

        def log_ex(self):

            return 'tag = 0x%.2x, bytes = 0x%x, addr = 0x%.8x' % \
                   (self.h_tag, self.bytes_read, self.addr)    


    class PacketMWr32(Packet): 

        tlp_type = 'MWr32'

        def __init__(self, req = None, addr = None, data = None, tag = None, tlp = None):

            TransactionLayer.Packet.__init__(self, tlp = tlp)

            if tlp is None:

                self.data = data if isinstance(data, list) else [ data ]

                self.h_tag = random.randrange(0, 0xff) if tag is None else tag
                self.h_req_id, self.addr, self.bytes_write = req, addr, len(self.data) * 4

                # create raw TLP from specified arguments
                self.encode()

        def decode(self, tlp):

            # decode packet header
            TransactionLayer.Packet.decode(self, tlp)

            # decode address word
            self.decode_addr()

            self.bytes_write = self.h_length * 4

        def encode(self):

            assert self.bytes_write % 4 == 0

            self.h_length = self.bytes_write / 4

            # encode packet header
            TransactionLayer.Packet.encode(self)

            # encode address dword
            self.encode_addr()

            self.tlp += self.data

        def log_ex(self):

            return 'tag = 0x%.2x, bytes = 0x%x, addr = 0x%.8x' % \
                   (self.h_tag, self.bytes_write, self.addr)


    class PacketMRd64(PacketMRd32): 

        tlp_type = 'MRd64'


    class PacketMWr64(PacketMWr32): 

        tlp_type = 'MWr64'


    class PacketCfgRd0(Packet): 

        tlp_type = 'CfgRd0'

        def __init__(self, req = None, dev = None, reg = None, tlp = None):

            TransactionLayer.Packet.__init__(self, tlp = tlp)

            if tlp is None:

                self.data = None
                self.h_req_id, self.h_device, self.h_reg, self.h_tag = req, dev, reg, 0

                # create raw TLP from specified arguments
                self.encode()

        def decode(self, tlp):

            # decode packet header
            TransactionLayer.Packet.decode(self, tlp)

            # decode address word
            self.decode_cfg_addr()

        def encode(self):

            self.h_length = 1

            # encode packet header
            TransactionLayer.Packet.encode(self)

            # encode address dword
            self.encode_cfg_addr()

        def log_ex(self):

            return 'tag = 0x%.2x, register = 0x%.2x, device = %s' % \
                   (self.h_tag, self.h_reg, dev_id_str(*self.h_device))   


    class PacketCfgRd1(PacketCfgRd0): 

        tlp_type = 'CfgRd1'


    class PacketCplD(Packet): 

        tlp_type = 'CplD'

        def decode(self, tlp):

            # decode packet header
            TransactionLayer.Packet.decode(self, tlp)

            # decode completion information dwords
            self.decode_completion()

        def log_ex(self):

            return 'tag = 0x%.2x, bytes = %d, req = %s, comp = %s' % \
                   (self.h_tag, self.h_byte_count, dev_id_str(*self.h_requester), \
                                                   dev_id_str(*self.h_completer))    

    def __init__(self, *args, **kvargs):

        # initialize link layer
        self.ep = endpoint_init(*args, **kvargs)

        self.bus_id = self.ep.bus_id        

    def log_all(self): 

        try:

            val = os.getenv(self.ENV_DEBUG_TLP)
            if val is not None:

                return int(val) != 0

        except: pass
        return False

    def read(self, raw = False):

        data = self.ep.read()

        # return not decoded TLP data if needed
        if raw: return data

        name = 'Packet' + tlp_type_name(data[0])

        # create appropriate object for each TLP type or use common one
        tlp = getattr(self, name)(tlp = data) if hasattr(self, name) else \
              self.Packet(tlp = data)

        if self.log_all() and isinstance(tlp, self.Packet):

            tlp.log(name = 'RX')

        return tlp

    def write(self, data):

        if self.log_all() and isinstance(data, self.Packet):

            data.log(name = 'TX')

        # get raw data in case when Packet instance was passed
        data = data.tlp if isinstance(data, self.Packet) else data

        self.ep.write(data)    

    def bridge(self, log = False, handler = None):        

        while True:

            # read incoming TLP
            tlp = self.read()
            tlp = tlp if handler is None else handler(self, tlp)

            # forward TLP
            if tlp is not None: self.write(tlp)

            # print TLP
            if log: tlp.log()

    def _mem_read(self, addr, size):

        output = ''
        chunk_size, ptr = min(size, self.MEM_RD_TLP_LEN), 0  

        assert addr % self.MEM_ALIGN == 0  
        assert size % self.MEM_ALIGN == 0

        assert self.bus_id is not None

        # read memory by blocks
        while ptr < size:

            chunk_addr = addr + ptr
            
            # memory r/w TLP should reside to the single memory page
            max_chunk_size = PAGE_SIZE if chunk_addr & 0xfff == 0 else \
                             (align_up(chunk_addr, PAGE_SIZE) - chunk_addr)

            cur_chunk_size = min(chunk_size, max_chunk_size)

            # create 64-bit memory read TLP            
            tlp_tx = self.PacketMRd64(self.bus_id, chunk_addr, cur_chunk_size)

            # send TLP to the system
            self.write(tlp_tx)

            data = ''

            while len(data) < cur_chunk_size:

                # read reply
                tlp_rx = self.read()

                if not isinstance(tlp_rx, self.PacketCplD):

                    raise(self.ErrorBadCompletion('Bad MRd TLP completion received'))

                assert tlp_tx.h_tag == tlp_rx.h_tag
                
                # decode data
                data += tlp_rx.get_data()

            output += data
            ptr += cur_chunk_size

        return output    

    def _mem_write(self, addr, data):

        size, chunk_size, ptr = len(data), min(len(data), self.MEM_WR_TLP_LEN), 0  

        assert addr % self.MEM_ALIGN == 0  
        assert size % self.MEM_ALIGN == 0

        assert self.bus_id is not None

        # read memory by blocks
        while ptr < size:

            chunk_addr = addr + ptr

            # memory r/w TLP should reside to the single memory page
            max_chunk_size = PAGE_SIZE if chunk_addr & 0xfff == 0 else \
                             (align_up(chunk_addr, PAGE_SIZE) - chunk_addr)

            cur_chunk_size = min(chunk_size, max_chunk_size)

            # get data chunk as dwords list
            tlp_data = unpack('>' + ('I' * (cur_chunk_size / 4)), data[ptr : ptr + cur_chunk_size])

            # create 64-bit memory read TLP
            tlp = self.PacketMWr64(self.bus_id, chunk_addr, list(tlp_data))

            # send TLP to the system
            self.write(tlp)

            ptr += cur_chunk_size

    def mem_read(self, addr, size):

        align = self.MEM_ALIGN

        read_addr = align_down(addr, align)
        read_size = align_up(size, align)

        if read_addr != addr or read_size != size:

            read_size += align

        ptr = addr - read_addr

        # align memory read request by MEM_ALIGN byte boundary
        return self._mem_read(read_addr, read_size)[ptr : ptr + size]

    def mem_write(self, addr, data):

        align, size = self.MEM_ALIGN, len(data)

        write_addr = align_down(addr, align)
        write_size = align_up(size, align)

        if write_addr != addr or write_size != size:

            write_size += align

        # read the existing data
        write_data = self._mem_read(write_addr, write_size)

        ptr = addr - write_addr
        
        # align memory write request by MEM_ALIGN byte boundary
        self._mem_write(write_addr, write_data[: ptr] + data + write_data[ptr + size :])

    def close(self):

        self.ep.close()


class TransactionLayerTest(unittest.TestCase):

    TEST_ADDR = 0x1000    

    def test_tlp(self):

        dev = TransactionLayer()

        dev.write(dev.PacketMRd64(dev.bus_id, 0, 4))
        dev.read()

    def test_mem(self):

        dev = TransactionLayer()

        data = dev.mem_read(self.TEST_ADDR, 0x100)

        dev.mem_write(self.TEST_ADDR, data)

    def test_normal(self, addr = TEST_ADDR):

        dev = TransactionLayer()

        val = 0x0102030405060708

        old = dev.mem_read_8(addr)

        dev.mem_write_8(addr, val)

        assert dev.mem_read_1(addr) == val & 0xff
        assert dev.mem_read_2(addr) == val & 0xffff
        assert dev.mem_read_4(addr) == val & 0xffffffff
        assert dev.mem_read_8(addr) == val

        dev.mem_write_8(addr, old)

    def test_unaligned(self, addr = TEST_ADDR):

        dev = TransactionLayer()

        val = int(time.time())

        old = dev.mem_read_8(addr)

        dev.mem_write_8(addr, 0)
        dev.mem_write_4(addr + 1, val)

        assert dev.mem_read_8(addr) == val << 8

        dev.mem_write_8(addr, 0)
        dev.mem_write_4(addr + 2, val)

        assert dev.mem_read_8(addr) == val << 16

        dev.mem_write_8(addr, 0)
        dev.mem_write_4(addr + 3, val)

        assert dev.mem_read_8(addr) == val << 24

        dev.mem_write_8(addr, old)

    def test_cross_page(self):

        self.test_normal(addr = self.TEST_ADDR - 1)
        
        self.test_unaligned(addr = self.TEST_ADDR - 2)

        self.test_normal(addr = self.TEST_ADDR - 2)
        
        self.test_unaligned(addr = self.TEST_ADDR - 3)

        self.test_normal(addr = self.TEST_ADDR - 3)
        
        self.test_unaligned(addr = self.TEST_ADDR - 4) 

#
# EoF
#
