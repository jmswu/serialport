#ifndef __SERIAL_PORT_H__
#define __SERIAL_PORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <Windows.h>

uint32_t    serialPort_getLibraryVersion(void);

HANDLE      serialPort_connect(unsigned portNumber, unsigned baudRate, unsigned dataBits, unsigned parity, unsigned stopBits);
HANDLE      serialPort_connectSimple(unsigned portNumber, unsigned baudRate);
void        serialPort_disconnect(HANDLE handle);
void        serialPort_sendByte(HANDLE handle, uint8_t oneByte);
DWORD       serialPort_sendArray(HANDLE handle, const uint8_t *data, size_t dataLength);
DWORD       serialPort_getNumberOfBytes(HANDLE handle);
uint8_t     serialPort_getOneByte(HANDLE handle);
DWORD       serialPort_getArray(HANDLE handle, uint8_t *data, size_t dataLength);

#ifdef __cplusplus
}
#endif

#endif
