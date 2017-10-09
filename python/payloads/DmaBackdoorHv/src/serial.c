#include <PiPei.h>

#include "serial.h"

/*
    UART Register Offsets
*/
#define BAUD_LOW_OFFSET         0x00
#define BAUD_HIGH_OFFSET        0x01
#define IER_OFFSET              0x01
#define LCR_SHADOW_OFFSET       0x01
#define FCR_SHADOW_OFFSET       0x02
#define IR_CONTROL_OFFSET       0x02
#define FCR_OFFSET              0x02
#define EIR_OFFSET              0x02
#define BSR_OFFSET              0x03
#define LCR_OFFSET              0x03
#define MCR_OFFSET              0x04
#define LSR_OFFSET              0x05
#define MSR_OFFSET              0x06

/*
    UART Register Bit Defines
*/
#define LSR_TXRDY               0x20
#define LSR_RXDA                0x01
#define DLAB                    0x01

#define BAUDRATE_MAX 115200

/*
    UART Settings
*/
UINT8 m_Data = 8;
UINT8 m_Stop = 1;
UINT8 m_Parity = 0;
UINT8 m_BreakSet = 0;
//--------------------------------------------------------------------------------------
/*
    Initialize the serial device hardware.
*/
VOID SerialPortInitialize(UINT16 Port, UINTN Baudrate)
{
    // Map 5..8 to 0..3
    UINT8 Data = (UINT8)(m_Data - (UINT8)5);

    // Calculate divisor for baud generator
    UINTN Divisor = BAUDRATE_MAX / Baudrate;    

    // Set communications format
    UINT8 OutputData = (UINT8)((DLAB << 7) | (m_BreakSet << 6) | (m_Parity << 3) | (m_Stop << 2) | Data);
    __outbyte((UINTN)(Port + LCR_OFFSET), OutputData);

    // Configure baud rate
    __outbyte((UINTN)(Port + BAUD_HIGH_OFFSET), (UINT8)(Divisor >> 8));
    __outbyte((UINTN)(Port + BAUD_LOW_OFFSET), (UINT8)(Divisor & 0xff));

    // Switch back to bank 0
    OutputData = (UINT8)((~DLAB << 7) | (m_BreakSet << 6) | (m_Parity << 3) | (m_Stop << 2) | Data);
    __outbyte((UINTN)(Port + LCR_OFFSET), OutputData);
}
//--------------------------------------------------------------------------------------
/*
    Write data to serial device.
*/
VOID SerialPortWrite(UINT16 Port, UINT8 Data)
{
    UINT8 Status = 0;

    do 
    {
        // Wait for the serail port to be ready
        Status = __inbyte(Port + LSR_OFFSET);

    } while ((Status & LSR_TXRDY) == 0);

    __outbyte(Port, Data);
}
//--------------------------------------------------------------------------------------
/*
    Reads data from a serial device.
*/
UINT8 SerialPortRead(UINT16 Port)
{
    UINT8 Status = 0;

    do 
    {
        // Wait for the serail port to be ready
        Status = __inbyte(Port + LSR_OFFSET);

    } while ((Status & LSR_RXDA) == 0);

    return __inbyte(Port);
}
//--------------------------------------------------------------------------------------
// EoF
