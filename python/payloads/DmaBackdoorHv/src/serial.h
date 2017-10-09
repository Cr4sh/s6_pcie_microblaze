
#ifndef _SERIAL_H_
#define _SERIAL_H_

#define SERIAL_PORT_0 0x3F8 /* COM1, ttyS0 */

VOID SerialPortInitialize(UINT16 Port, UINTN Baudrate);

VOID SerialPortWrite(UINT16 Port, UINT8 Data);
UINT8 SerialPortRead(UINT16 Port);

#endif
