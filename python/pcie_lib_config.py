
DEVICE_TYPE_TCP     = 1
DEVICE_TYPE_SERIAL  = 2
DEVICE_TYPE_UIO     = 3
DEVICE_TYPE_DRIVER  = 4

class Conf:

    device_type = DEVICE_TYPE_TCP

    #
    # DEVICE_TYPE_SERIAL options
    #

    # serial port device
    device = '/dev/ttyUSB0'

    # serial port baudrate
    baud = 115200

    #
    # DEVICE_TYPE_TCP options
    #

    # board IP address and port
    addr = ( '192.168.2.247', 28472 )

#
# EoF
#
