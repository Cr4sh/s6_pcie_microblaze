
DEVICE_TYPE_TCP     = 1
DEVICE_TYPE_SERIAL  = 2

class Conf:

    device_type = DEVICE_TYPE_TCP

    # serial port device
    device = '/dev/ttyUSB0'

    # serial port baudrate
    baud = 115200

    # board IP address and port
    addr = ( '192.168.2.247', 28472 )

#
# EoF
#
