
#ifndef _SERIAL_H_
#define _SERIAL_H_

#define SERIAL_PORT_0 0x3F8 /* COM1, ttyS0 */
#define SERIAL_PORT_1 0x2F8 /* COM2, ttyS1 */
#define SERIAL_PORT_2 0x3E8 /* COM3, ttyS2 */
#define SERIAL_PORT_3 0x2E8 /* COM4, ttyS3 */

VOID SerialPortInitialize(UINT16 Port, UINTN Baudrate);

VOID SerialPortWrite(UINT16 Port, UINT8 Data);
UINT8 SerialPortRead(UINT16 Port);

#endif
