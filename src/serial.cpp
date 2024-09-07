#include "serial.h"

std::string parityToString(ParityType parity) {
    switch (parity) {
        case NONE:
            return "None";
        case ODD:
            return "Odd";
        case EVEN:
            return "Even";
        default:
            return "Unknown";
    }
}

int baudRateToInt(BaudRate baud) {
    switch (baud) {
        case BAUD_9600:
            return 9600;
        case BAUD_14400:
            return 14400;
        case BAUD_19200:
            return 19200;
        case BAUD_38400:
            return 38400;
        case BAUD_57600:
            return 57600;
        case BAUD_115200:
            return 115200;
        case BAUD_230400:
            return 230400;
        case BAUD_460800:
            return 460800;
        case BAUD_921600:
            return 921600;
        default:
            return -1;
    }
}

SerialMode serialGetConfig(uint8_t dataBits, ParityType parity, uint8_t stopBits, bool flowControl) {
    SerialMode config = M_SERIAL_8N1;

    // Bits
    switch (dataBits) {
        case 5:
            config = (stopBits == 1) ? M_SERIAL_5N1 : M_SERIAL_5N2;
            break;
        case 6:
            config = (stopBits == 1) ? M_SERIAL_6N1 : M_SERIAL_6N2;
            break;
        case 7:
            config = (stopBits == 1) ? M_SERIAL_7N1 : M_SERIAL_7N2;
            break;
        case 8:
            config = (stopBits == 1) ? M_SERIAL_8N1 : M_SERIAL_8N2;
            break;
    }

    // Parity
    if (parity == ODD) {
        config = static_cast<SerialMode>(config | 0x1);
    } else if (parity == EVEN) {
        config = static_cast<SerialMode>(config | 0x2);
    }

    if (flowControl) {
        // TODO: Should we handle this ?
    }

    return config;
}

