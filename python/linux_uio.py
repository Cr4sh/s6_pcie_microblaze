import sys, os, mmap, ctypes
from struct import pack, unpack

try:

    import numpy

except ImportError:

    print('ERROR: numpy is not installed')
    exit(0)

class LinuxUIO(object):

    DEV_PATH = '/dev'
    UIO_PATH = '/sys/class/uio'

    class UIOError(Exception):

        pass

    def __init__(self, name):

        # find UIO device by name
        self.dev = self.find_name(name)

        # get memory region size
        self.size = self.get_size(self.dev)

        # create mapping
        self.fd = os.open(os.path.join(self.DEV_PATH, self.dev), os.O_RDWR)
        self.mm = mmap.mmap(self.fd, self.size)

        # get virtual and physical address
        self.addr_phys = self.get_addr(self.dev)
        self.addr_virt = ctypes.addressof(ctypes.c_void_p.from_buffer(self.mm))

        # get current events count
        self.event_count = self.get_event_count(self.dev)

    def as_ndarray(self, items = None, dtype = 'uint8'):

        dtype = numpy.dtype(dtype)

        if items is None:

            # calculate array length
            items = len(self.mm) / dtype.itemsize

        return numpy.ndarray(items, dtype, buffer = self.mm)

    def as_recarray(self, items, dtype):

        return numpy.recarray(items, dtype, buf = self.mm)

    def _get_string(self, path):

        with open(path, 'rb') as fd:

            # return string value
            return fd.read().strip()

    def _get_long(self, path):

        with open(path, 'rb') as fd:
            
            data = fd.read().strip()

            try:

                # return number
                return int(data, 16 if data.find('0x') == 0 else 10)

            except ValueError:

                raise(self.UIOError('Invalid value for "%s"' % path))                

    def get_name(self, dev):

        return self._get_string(os.path.join(self.UIO_PATH, dev, 'name'))

    def get_event_count(self, dev):

        return self._get_long(os.path.join(self.UIO_PATH, dev, 'event'))

    def get_addr(self, dev, map = 0):

        return self._get_long(os.path.join(self.UIO_PATH, dev, 'maps', 'map%d' % map, 'addr'))

    def get_size(self, dev, map = 0):

        return self._get_long(os.path.join(self.UIO_PATH, dev, 'maps', 'map%d' % map, 'size'))

    def get_offset(self, dev, map = 0):

        return self._get_long(os.path.join(self.UIO_PATH, dev, 'maps', 'map%d' % map, 'offset'))

    def find_name(self, name):        

        # enumerate available UIO devices
        for dev in os.listdir(self.UIO_PATH):

            # read device name
            if self.get_name(dev) == name:

                return dev

        raise(self.UIOError('Unable to find UIO device "%s"' % name))

    def virt_to_phys(self, addr):
        
        if addr < self.addr_virt or \
           addr > self.addr_virt + self.size: 

            raise(self.UIOError('Invalid virtual address 0x%x' % addr))

        # convert virtual address to physical address
        return self.addr_phys + (addr - self.addr_virt)

    def phys_buff(self, buff):
        
        return self.virt_to_phys( buff.ctypes.data ), buff.nbytes

    def read(self):

        return unpack('@I', os.read(self.fd, 4))[0]

    def write(self, val):

        os.write(self.fd, pack('@I', val))

    def intr_enable(self):

        # write to the descriptor enables interrupts
        self.write(1)

    def intr_wait(self):

        # read from the descriptor waits for the interrupt
        num = self.read()
        ret = num - self.event_count

        # update event count
        self.event_count = num

        return ret

    def close(self):

        if self.mm is not None:

            self.mm.close()
            self.mm = None

        if self.fd is not None:

            os.close(self.fd)
            self.fd = None

#
# EoF
#
