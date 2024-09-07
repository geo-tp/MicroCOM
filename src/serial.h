#ifndef SERIAL_H
#define SERIAL_H

#include <string>

enum SerialMode {
    M_SERIAL_5N1 = 0x8000010,
    M_SERIAL_6N1 = 0x8000014,
    M_SERIAL_7N1 = 0x8000018,
    M_SERIAL_8N1 = 0x800001c,
    M_SERIAL_5N2 = 0x8000030,
    M_SERIAL_6N2 = 0x8000034,
    M_SERIAL_7N2 = 0x8000038,
    M_SERIAL_8N2 = 0x800003c,
    M_SERIAL_5E1 = 0x8000012,
    M_SERIAL_6E1 = 0x8000016,
    M_SERIAL_7E1 = 0x800001a,
    M_SERIAL_8E1 = 0x800001e,
    M_SERIAL_5E2 = 0x8000032,
    M_SERIAL_6E2 = 0x8000036,
    M_SERIAL_7E2 = 0x800003a,
    M_SERIAL_8E2 = 0x800003e,
    M_SERIAL_5O1 = 0x8000013,
    M_SERIAL_6O1 = 0x8000017,
    M_SERIAL_7O1 = 0x800001b,
    M_SERIAL_8O1 = 0x800001f,
    M_SERIAL_5O2 = 0x8000033,
    M_SERIAL_6O2 = 0x8000037,
    M_SERIAL_7O2 = 0x800003b,
    M_SERIAL_8O2 = 0x800003f
};

enum ParityType {
    NONE,
    ODD,
    EVEN,
    PARITY_COUNT
};
std::string parityToString(ParityType parity);

enum BaudRate {
    BAUD_9600,
    BAUD_14400,
    BAUD_19200,
    BAUD_38400,
    BAUD_57600,
    BAUD_115200,
    BAUD_230400,
    BAUD_460800,
    BAUD_921600,
    BAUDRATE_COUNT
};
int baudRateToInt(BaudRate baud);

SerialMode serialGetConfig(uint8_t dataBits, ParityType parity, uint8_t stopBits, bool flowControl);

#endif
