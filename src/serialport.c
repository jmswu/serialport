#include <stdint.h>
#include "serialport.h"

#define SERIAL_PORT_VER_MAJOR       (0U)
#define SERIAL_PORT_VER_MINOR       (0U)
#define SERIAL_PORT_VER_PATCH       (1U)
#define SERIAL_PORT_VER_NUMBER      ((SERIAL_PORT_VER_MAJOR << 16) | (SERIAL_PORT_VER_MINOR << 8) | (SERIAL_PORT_VER_PATCH))


uint32_t serialPort_getLibraryVersion(void)
{
    return SERIAL_PORT_VER_NUMBER;
}

int serialPort_connect(unsigned portNumber, unsigned baud)
{
    (void)portNumber;
    (void)baud;
    return 1;
}