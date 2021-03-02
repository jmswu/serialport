#ifndef __SERIAL_PORT_H__
#define __SERIAL_PORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <Windows.h>

#define INVALID_HANDLE_VALUE    ((HANLE) -1)

uint32_t    serialPort_getLibraryVersion(void);

HANDLE      serialPort_connect(unsigned portNumber, unsigned baud, unsigned dataBits, unsigned priority, unsigned stopBits);
HANDLE      serialPort_connectSimple(unsigned portNumber, unsigned baud);
void        serialPort_disconnect(HANDLE handle);
void        serialPort_sendByte(HANDLE handle, uint8_t data);
void        serialPort_sendArray(HANDLE handle, uint8_t *data, size_t dataLength);
int         serialPort_getNumberOfBytes(HANDLE handle);
uint8_t     serialPort_getOneByte(HANDLE handle);
int         serialPort_getArray(HANDLE handle, uint8_t *data, size_t dataLength);

#ifdef __cplusplus
}
#endif

#endif
