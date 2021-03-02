#ifndef __SERIAL_PORT_H__
#define __SERIAL_PORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint32_t serialPort_getLibraryVersion(void);

int serialPort_connect(unsigned portNumber, unsigned baud);

#ifdef __cplusplus
}
#endif

#endif
