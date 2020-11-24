import sys, os, time

from linux_uio import *

class LinuxAxiDMA(object):

    # AXI DMA channel registers space
    axi_dma_dtype = numpy.dtype([ ( 'control',  '<u4', 1 ),
                                  ( 'status',   '<u4', 1 ),
                                  ( 'res_1',    '<u4', 4 ),
                                  ( 'addr_lo',  '<u4', 1 ),
                                  ( 'addr_hi',  '<u4', 1 ),
                                  ( 'res_2',    '<u4', 2 ),
                                  ( 'length',   '<u4', 1 ),
                                  ( 'padding',  '<u4', 1 )])

    class Error(Exception):

        pass

    class Timeout(Exception):

        pass

    class Channel(object):

        # control register bits
        CR_START      = 0x00000001
        CR_RESET      = 0x00000004
        CR_IRQEN_IOC  = 0x00001000 # enable completion interrupt
        CR_IRQEN_DLY  = 0x00002000 # enable delay interrupt
        CR_IRQEN_ERR  = 0x00004000 # enable error interrupt

        # status register bits
        ST_HALTED     = 0x00000001
        ST_IDLE       = 0x00000002
        ST_ERR_INT    = 0x00000010 # internal error
        ST_ERR_SLV    = 0x00000020 # slave error
        ST_ERR_DEC    = 0x00000040 # decode error

        def __init__(self, dtype, uio):

            self.dtype, self.uio = dtype, uio

        def reset(self):

            # reset AXI DMA channel
            self.dtype.control = self.CR_RESET
            self.dtype.control = 0

        def start(self, buff, irq_enable = False):

            buff_addr, buff_size = buff
            control = self.CR_START

            if irq_enable:

                self.uio.intr_enable()

                # enable completion and error interrupts
                control |= self.CR_IRQEN_IOC
                control |= self.CR_IRQEN_ERR

            # set up buffer address
            self.dtype.addr_hi = 0
            self.dtype.addr_lo = buff_addr            
            
            # start transfer
            self.dtype.control = control
            self.dtype.length = buff_size

        def wait(self, timeout = None):

            t = time.time()

            while True:

                if self.is_idle() or self.is_halted():

                    # transfer completed
                    break

                if timeout is not None and time.time() - t > timeout:

                    # timeout occurred
                    return -1

            return self.get_error()

        def get_error(self):

            # return error bits
            return self.dtype.status & (self.ST_ERR_INT | self.ST_ERR_SLV | self.ST_ERR_DEC)

        def is_halted(self):

            return self.dtype.status & self.ST_HALTED != 0

        def is_idle(self):

            return self.dtype.status & self.ST_IDLE != 0        

    def __init__(self, uio_name):

        # open UIO device
        self.uio = LinuxUIO(uio_name)

        # get numpy structs for each AXI DMA channel
        mm2s, s2mm = self.uio.as_recarray(2, dtype = self.axi_dma_dtype)

        self.mm2s = self.Channel(mm2s, self.uio)
        self.s2mm = self.Channel(s2mm, self.uio)
        
        # reset AXI DMA engine
        self.reset()

    def reset(self):

        # reset each channel
        map(lambda channel: channel.reset(), [ self.mm2s, self.s2mm ])

    def _process(self, channel, buff, timeout = None):

        # start transfer
        channel.start(buff)  

        # wait for the completion
        code = channel.wait(timeout = timeout)  

        if code == -1:

            self.reset()

            raise(self.Timeout('AXI DMA transfer timeout'))

        elif code != 0:

            raise(self.Error('AXI DMA transfer error 0x%x' % code))

    def recv(self, buff, timeout = None):

        self._process(self.s2mm, buff, timeout = timeout)

        # get number of copied bytes
        return self.s2mm.dtype.length

    def send(self, buff, timeout = None):

        self._process(self.mm2s, buff, timeout = timeout)

    def close(self):

        self.uio.close()

#
# EoF
#
