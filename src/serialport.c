#include <stdint.h>
#include <stdio.h>
#include "serialport.h"

#define SERIAL_PORT_VER_MAJOR       (0U)
#define SERIAL_PORT_VER_MINOR       (0U)
#define SERIAL_PORT_VER_PATCH       (2U)
#define SERIAL_PORT_VER_NUMBER      ((SERIAL_PORT_VER_MAJOR << 16) | (SERIAL_PORT_VER_MINOR << 8) | (SERIAL_PORT_VER_PATCH))

static HANDLE   openCommunication(unsigned portNumber);
static BOOL     setCommunicationTimeouts(HANDLE handle);
static BOOL     setCommunicationBaudRate(HANDLE handle, unsigned baudRate);
static BOOL     setCommunicationDataBits(HANDLE handle, unsigned dataBits);
static BOOL     setCommunicationParity(HANDLE handle, unsigned parity);
static BOOL     setCommunicationStopBits(HANDLE handle, unsigned stopBits);
static BOOL     isVaildParity(unsigned parity);
static BOOL     isValidStopBits(unsigned stopBits);
static BOOL     isValidHandle(HANDLE handle);

/*! \brief  Get library version number
 *! \retval version number
 */
uint32_t serialPort_getLibraryVersion(void)
{
    return SERIAL_PORT_VER_NUMBER;
}

static HANDLE openCommunication(unsigned portNumber)
{
	HANDLE handle;
	TCHAR portName[32];
	wsprintf(portName, TEXT("\\\\.\\COM%d"), portNumber);
	handle = CreateFile(portName,           //port name 
		GENERIC_READ | GENERIC_WRITE,       //Read/Write   				 
		0,            // No Sharing                               
		NULL,         // No Security                              
		OPEN_EXISTING,// Open existing port only                     
		0,            // Non Overlapped I/O                           
		NULL);        // Null for Comm Devices

    return handle;
}

static BOOL setCommunicationBaudRate(HANDLE handle, unsigned baudRate)
{
	DCB dcbSerialParams = { 0 };
	BOOL status;
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	status = GetCommState(handle, &dcbSerialParams);
	if (status == FALSE)
		return FALSE;
	dcbSerialParams.BaudRate = baudRate;
	status = SetCommState(handle, &dcbSerialParams);
	return status;
}

static BOOL setCommunicationDataBits(HANDLE handle, unsigned dataBits)
{
	DCB dcbSerialParams = { 0 };
	BOOL status;
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	status = GetCommState(handle, &dcbSerialParams);
	if (status == FALSE)
		return FALSE;
	dcbSerialParams.ByteSize = dataBits;
	status = SetCommState(handle, &dcbSerialParams);
	return status;
}

static BOOL setCommunicationParity(HANDLE handle, unsigned parity)
{
	DCB dcbSerialParams = { 0 };
	BOOL status;
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	status = GetCommState(handle, &dcbSerialParams);
	if (status == FALSE)
		return FALSE;
	dcbSerialParams.Parity = parity;
	status = SetCommState(handle, &dcbSerialParams);
	return status;
}

static BOOL setCommunicationStopBits(HANDLE handle, unsigned stopBits)
{
	DCB dcbSerialParams = { 0 };
	BOOL status;
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	status = GetCommState(handle, &dcbSerialParams);
	if (status == FALSE)
		return FALSE;
	dcbSerialParams.StopBits = stopBits;
	status = SetCommState(handle, &dcbSerialParams);
	return status;
}

static BOOL isVaildParity(unsigned parity)
{
    const unsigned ALL_PARITY_TYPE[] = {
        NOPARITY,
        EVENPARITY,
        MARKPARITY,
        ODDPARITY,
        SPACEPARITY,
    };

    for(unsigned i = 0; i < sizeof(ALL_PARITY_TYPE) / sizeof(ALL_PARITY_TYPE[0]); i++)
    {
        if (ALL_PARITY_TYPE[i] == parity)
            return TRUE;
    }

    return FALSE;
}

static BOOL isValidStopBits(unsigned stopBits)
{
    const unsigned ALL_STOPBITS_TYPE[] = {
        ONESTOPBIT,
        ONE5STOPBITS,
        TWOSTOPBITS,
    };

    for(unsigned i = 0; i < sizeof(ALL_STOPBITS_TYPE) / sizeof(ALL_STOPBITS_TYPE[0]); i++)
    {
        if (ALL_STOPBITS_TYPE[i] == stopBits)
            return TRUE;
    }

    return FALSE;
}

static BOOL isValidHandle(HANDLE handle)
{
    return (handle != INVALID_HANDLE_VALUE) && (handle != NULL) ? TRUE : FALSE;
}

static BOOL setCommunicationTimeouts(HANDLE handle)
{
	COMMTIMEOUTS timeouts = { 0 };
	// timeouts.ReadIntervalTimeout = 50;
	// timeouts.ReadTotalTimeoutConstant = 50;
	// timeouts.ReadTotalTimeoutMultiplier = 10;
	// timeouts.WriteTotalTimeoutConstant = 50;
	// timeouts.WriteTotalTimeoutMultiplier = 10;
    return SetCommTimeouts(handle, &timeouts);
}

/*! \brief  Connect to a com port
 *!         Return a handle if open successfully. Return INVALID_HANDLE_VALUE 
 *!         if there is error.
 *! \param  unsigned        - com port number
 *! \param  unsigned        - baud rate
 *! \param  unsigned        - number of bits in a byte
 *! \param  unsigned        - parity bytes
 *! \param  unsigned        - number of stop bits
 *! \retval HANDLE          - com port handle
 */
HANDLE serialPort_connect(unsigned portNumber, unsigned baudRate, unsigned dataBits, unsigned parity, unsigned stopBits)
{

    if (isVaildParity(parity) == FALSE)
        return INVALID_HANDLE_VALUE;
    
    if (isValidStopBits(stopBits) == FALSE)
        return INVALID_HANDLE_VALUE;

    HANDLE handle = openCommunication(portNumber);

    if (handle == INVALID_HANDLE_VALUE)
    {
        serialPort_disconnect(handle);
        return INVALID_HANDLE_VALUE;
    }
        
	if (setCommunicationTimeouts(handle) == FALSE)
    {
        serialPort_disconnect(handle);
        return INVALID_HANDLE_VALUE;
    }

	if (SetCommMask(handle, 0) == FALSE)
    {
        serialPort_disconnect(handle);
        return INVALID_HANDLE_VALUE;
    }

    if (setCommunicationBaudRate(handle, baudRate) == FALSE)
    {
        serialPort_disconnect(handle);
        return INVALID_HANDLE_VALUE;
    }

    if (setCommunicationDataBits(handle, dataBits == FALSE))
    {
        serialPort_disconnect(handle);
        return INVALID_HANDLE_VALUE;
    }

    if (setCommunicationParity(handle, parity) == FALSE)
    {
        serialPort_disconnect(handle);
        return INVALID_HANDLE_VALUE;
    }

    if (setCommunicationStopBits(handle, stopBits) == FALSE)
    {
        serialPort_disconnect(handle);
        return INVALID_HANDLE_VALUE;
    }

    return handle;      
}

/*! \bried  Close or disconnect a com port
 *! \param HANDLE           - com port handle
 */ 
void serialPort_disconnect(HANDLE handle)
{
    if (isValidHandle(handle))
    {
        CloseHandle(handle);
    }
}

/*! \bried  A simpler version of serial port connection
 *! \param  unsigned        - com port number
 *! \param  unsigned        - baud rate  
 *! \retval HANDLE          - com port handle
 */  
HANDLE serialPort_connectSimple(unsigned portNumber, unsigned baudRate)
{
    const unsigned DATA_BITS = 8;
    const unsigned STOP_BITS = ONESTOPBIT;
    const unsigned PARITY = NOPARITY;

    return serialPort_connect(portNumber,
        baudRate,
        DATA_BITS,
        PARITY,
        STOP_BITS);
}

/*! \bried  Send one byte
 *!         If the handle is invalid, nothing will be send
 *! \param  HANDLE          - com port handle
 *! \param  uint8_t         - byte to be send
 */  
void serialPort_sendOneByte(HANDLE handle, uint8_t oneByte)
{
    if (isValidHandle(handle))
    {
        serialPort_sendArray(handle, &oneByte, 1);
    } 
}

/*! \bried  Send an array of data
 *!         If the handle is invalid, nothing will be send
 *! \param  HANDLE          - com port handle
 *! \param  uint8_t*        - pointer to data
 *! \param  size_t          - size of the array
 *! \retval DWORD           - actual number of bytes send
 */  
DWORD serialPort_sendArray(HANDLE handle, const uint8_t * const data, size_t dataLength)
{
	DWORD  numberOfbytesWritten = 0;

    if (isValidHandle(handle) == FALSE) return numberOfbytesWritten;
    if (data == NULL) return numberOfbytesWritten;

	WriteFile(handle,
		    data,
		    dataLength,
		    &numberOfbytesWritten,
		    NULL);
    return numberOfbytesWritten;
}

/*! \bried  Get the number of bytes available in the buffer
 *! \param  HANDLE          - com port handle
 *! \retval DWORD           - number of bytes available
 */  
DWORD serialPort_getNumberOfBytes(HANDLE handle)
{
    struct _COMSTAT status;
    int numberOfBytesAvailable = 0;
    unsigned long errorStatus;

    if (isValidHandle(handle))
    {
        ClearCommError(handle, &errorStatus, &status);
        numberOfBytesAvailable = status.cbInQue;
    }
    
    return numberOfBytesAvailable;
}

/*! \bried  Get one byte for the serial port buffer
 *!         If handle is invalid, it will always reutnr 0
 *! \param  HANDLE          - com port handle
 *! \retval uint8_t         - data received
 */  
uint8_t serialPort_getOneByte(HANDLE handle)
{
    uint8_t oneByte = 0;
    if (isValidHandle(handle))
    {
        serialPort_getArray(handle, &oneByte, 1);
    }
    return oneByte;
}

/*! \bried  Receive an array of data
 *!         If the handle is invalid, nothing will be received
 *! \param  HANDLE          - com port handle
 *! \param  uint8_t*        - pointer to data buffer
 *! \param  size_t          - buffer size
 *! \retval DWORD           - actual number of bytes received
 */  
DWORD serialPort_getArray(HANDLE handle, uint8_t * const data, size_t dataLength)
{
    DWORD numberOfBytesRead = 0;

    if (isValidHandle(handle) == FALSE) return numberOfBytesRead;
    if (data == NULL) return numberOfBytesRead;

    if (serialPort_getNumberOfBytes(handle) > 0)
        ReadFile(handle, data, dataLength, &numberOfBytesRead, NULL);

    return numberOfBytesRead;
}