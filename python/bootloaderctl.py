#!/usr/bin/env python

import sys, os
import serial
from struct import pack, unpack

SERIAL_BAUDRATE = 115200

FLASH_IMAGE_START = 0x100000
FLASH_WRITE_MAX_BYTES = 0x100

NET_CONFIG_MAGIC = 0x0c0c0c0c

uart_open = lambda name: serial.Serial(name, SERIAL_BAUDRATE)
uart_close = lambda fd: fd.close()

flash_mode_exit = lambda fd: fd.write('q')

def flash_erase(fd, addr, size):

    ptr = 0

    print('Erasing flash...')

    while ptr < size:

        fd.write('e')
        fd.write(pack('I', addr + ptr))
        fd.write(pack('I', 0x100))

        assert fd.read(1) == '\0'
        assert fd.read(1) == '\0'        

        ptr += 0x100

def flash_read(fd, addr, size):

    fd.write('r')
    fd.write(pack('I', addr))
    fd.write(pack('I', size))

    assert fd.read(1) == '\0'

    data = fd.read(size)
    assert len(data) == size

    return data

def flash_write(fd, addr, data):    

    ptr = 0

    flash_erase(fd, addr, len(data))

    while ptr < len(data):

        temp = data[ptr : ptr + FLASH_WRITE_MAX_BYTES]

        print('Writing 0x%x bytes at 0x%.8x' % (len(temp), addr + ptr))

        fd.write('w')
        fd.write(pack('I', addr + ptr))
        fd.write(pack('I', len(temp)))

        assert fd.read(1) == '\0'

        fd.write(temp)

        assert fd.read(1) == '\0'        
        assert fd.read(len(temp)) == temp        

        ptr += len(temp)    

def update_config(fd, address, netmask, gateway, port):

    parse_addr = lambda addr: map(lambda b: int(b), addr.split('.'))

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
    
    args = [ NET_CONFIG_MAGIC ] + address + netmask + gateway + [ port ]

    flash_write(fd, 0, pack('=IBBBBBBBBBBBBH', *args))

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

    if not console:

        with serial.Serial(dev, SERIAL_BAUDRATE, timeout = 5) as fd:

            fd.write('p')

            if fd.read(1) != 'p':

                print('[!] Target currently is not in update mode')
                return -1

    if file_path is not None:

        with open(file_path, 'rb') as fd:

            data = fd.read()

        print('[+] Flasing %d bytes from "%s"...' % (len(data), file_path))

    elif net_config is not None:

        print('[+] Updating board settings...')

    elif boot:

        print('[+] Exitting from update mode...')

    with uart_open(dev) as fd:

        if file_path is not None:

            flash_write(fd, FLASH_IMAGE_START, data)

            print('[+] DONE')

        elif net_config is not None:

            update_config(fd, *net_config)

            print('[+] DONE')

        elif boot:
        
            flash_mode_exit(fd)   

        if boot or console:        

            try:

                while True: sys.stdout.write(fd.read(1))

            except KeyboardInterrupt:

                pass    

    return 0

if __name__ == '__main__':

    exit(main())

#
# EoF
#
