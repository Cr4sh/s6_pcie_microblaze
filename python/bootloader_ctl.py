#!/usr/bin/env python

import sys, os
from struct import pack, unpack

try:

    import serial

except ImportError:

    raise(Exception('pyserial is not installed'))

SERIAL_BAUDRATE = 115200
SERIAL_TIMEOUT = 5 # in seconds

# SREC image location
FLASH_IMAGE_START = 0x100000
FLASH_WRITE_MAX_BYTES = 0x100

# NET_CONFIG structure location
NET_CONFIG_ADDR = 0
NET_CONFIG_MAGIC = 0x0C0C0C0C

# bootloader commands
CTL_ERASE = 'e'
CTL_WRITE = 'w'
CTL_READ  = 'r'
CTL_PING  = 'p'
CTL_QUIT  = 'q'

# bootloader status codes
STATUS_SUCCESS = '\x00'
STATUS_ERROR = '\xFF'

uart_open = lambda name, timeout: serial.Serial(name, SERIAL_BAUDRATE, timeout = timeout)
uart_close = lambda fd: fd.close()

flash_mode_exit = lambda fd: fd.write(CTL_QUIT)

def flash_erase(fd, addr, size):

    ptr = 0

    print('Erasing flash...')

    while ptr < size:

        # send flash erase command
        fd.write(CTL_ERASE)
        fd.write(pack('I', addr + ptr))
        fd.write(pack('I', 0x100))

        assert fd.read(1) == STATUS_SUCCESS
        assert fd.read(1) == STATUS_SUCCESS

        ptr += 0x100

def flash_read(fd, addr, size):

    # send flash read command
    fd.write(CTL_READ)
    fd.write(pack('I', addr))
    fd.write(pack('I', size))

    assert fd.read(1) == STATUS_SUCCESS

    # read needed amount of data
    data = fd.read(size)
    assert len(data) == size

    return data

def flash_write(fd, addr, data):    

    ptr = 0

    # erase flash contents first
    flash_erase(fd, addr, len(data))

    while ptr < len(data):

        temp = data[ptr : ptr + FLASH_WRITE_MAX_BYTES]

        print('Writing 0x%x bytes at 0x%.8x' % (len(temp), addr + ptr))

        # send flash write command
        fd.write(CTL_WRITE)
        fd.write(pack('I', addr + ptr))
        fd.write(pack('I', len(temp)))

        assert fd.read(1) == STATUS_SUCCESS

        fd.write(temp)

        # verify written data
        assert fd.read(1) == STATUS_SUCCESS
        assert fd.read(len(temp)) == temp        

        ptr += len(temp)    

def update_config(fd, address, netmask, gateway, port):

    parse_addr = lambda addr: map(lambda b: int(b), addr.split('.'))

    # parse IP address strings
    address = parse_addr(address)
    netmask = parse_addr(netmask)
    gateway = parse_addr(gateway)    

    assert len(address) == 4
    assert len(netmask) == 4
    assert len(gateway) == 4

    port = int(port)

    print('')
    print(' Address: %d.%d.%d.%d' % tuple(address))
    print(' Netmask: %d.%d.%d.%d' % tuple(netmask))
    print(' Gateway: %d.%d.%d.%d' % tuple(gateway))
    print('    Port: %d' % port)
    print('')
    
    # construct NET_CONFIG structure
    args = [ NET_CONFIG_MAGIC ] + address + netmask + gateway + [ port ]

    # write NET_CONFIG into the flash
    flash_write(fd, NET_CONFIG_ADDR, pack('=IBBBBBBBBBBBBH', *args))

def help():

    print('USAGE: bootloaderctl.py <dev> --boot')
    print('       bootloaderctl.py <dev> --console')
    print('       bootloaderctl.py <dev> --flash <file>')
    print('       bootloaderctl.py <dev> --config <string>')

def main():

    if len(sys.argv) < 2:

        help()
        return -1

    dev = sys.argv[1]
    file_path = net_config = None
    boot = console = False

    if len(sys.argv) > 3 and sys.argv[2] == '--flash':

        file_path = sys.argv[3]

    elif len(sys.argv) > 3 and sys.argv[2] == '--config':

        net_config = sys.argv[3].split(':')

        assert len(net_config) == 4

    elif len(sys.argv) > 2 and sys.argv[2] == '--boot':

        boot = True

    elif len(sys.argv) > 2 and sys.argv[2] == '--console':

        console = True

    else:

        help()
        return -1

    print('[+] Opening device "%s"...' % dev)

    # open serial port
    fd = uart_open(dev, SERIAL_TIMEOUT)
    fd.flush()

    if not console:

        update_mode = False
        
        fd.timeout = 1

        for i in range(0, 3):

            fd.write(CTL_PING)

            # check for active flash update mode
            if fd.read(1) == CTL_PING:

                update_mode = True
                break

        fd.timeout = SERIAL_TIMEOUT
                
        if not update_mode:

            print('ERROR: Target currently is not in update mode')
            return -1

    if file_path is not None:

        with open(file_path, 'rb') as file:

            # read SREC image contents to flash
            data = file.read()

        print('[+] Flasing %d bytes from "%s"...' % (len(data), file_path))

    elif net_config is not None:

        print('[+] Updating board settings...')

    elif boot:

        print('[+] Exitting from update mode...')

    if file_path is not None:

        # write SREC image into the flash
        flash_write(fd, FLASH_IMAGE_START, data)

        print('[+] DONE')

    elif net_config is not None:

        # write network configuration into the flash
        update_config(fd, *net_config)

        print('[+] DONE')

    elif boot:
    
        # boot into the flashed SREC image
        flash_mode_exit(fd)   

    if boot or console:        

        try:

            while True: 

                sys.stdout.write(fd.read(1))

        except KeyboardInterrupt:

            pass    

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#
