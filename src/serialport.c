#include <stdint.h>
#include "serialport.h"

#define SERIAL_PORT_VER_MAJOR       (0U)
#define SERIAL_PORT_VER_MINOR       (0U)
#define SERIAL_PORT_VER_PATCH       (1U)
#define SERIAL_PORT_VER_NUMBER      ((SERIAL_PORT_VER_MAJOR << 16) | (SERIAL_PORT_VER_MINOR << 8) | (SERIAL_PORT_VER_PATCH))

static HANDLE   openCommunication(unsigned portNumber);
static BOOL     setCommunicationTimeouts(HANDLE handle);
static BOOL     setCommunicationBaudRate(HANDLE handle, unsigned baudRate);
static BOOL     setCommunicationDataBits(HANDLE handle, unsigned dataBits);
static BOOL     setCommunicationParity(HANDLE handle, unsigned parity);
static BOOL     setCommunicationStopBits(HANDLE handle, unsigned stopBits);

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
	BOOL Status;
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	Status = GetCommState(handle, &dcbSerialParams);
	if (Status == FALSE)
		return FALSE;
	dcbSerialParams.ByteSize = dataBits;
	Status = SetCommState(handle, &dcbSerialParams);
	return Status;
}

static BOOL setCommunicationParity(HANDLE handle, unsigned parity)
{
	DCB dcbSerialParams = { 0 };
	BOOL Status;
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	Status = GetCommState(handle, &dcbSerialParams);
	if (Status == FALSE)
		return FALSE;
	dcbSerialParams.Parity = parity;
	Status = SetCommState(handle, &dcbSerialParams);
	return Status;
}

static BOOL setCommunicationStopBits(HANDLE handle, unsigned stopBits)
{
	DCB dcbSerialParams = { 0 };
	BOOL Status;
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	Status = GetCommState(handle, &dcbSerialParams);
	if (Status == FALSE)
		return FALSE;
	dcbSerialParams.StopBits = stopBits;
	Status = SetCommState(handle, &dcbSerialParams);
	return Status;
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

HANDLE serialPort_connect(unsigned portNumber, unsigned baudRate, unsigned dataBits, unsigned parity, unsigned stopBits)
{
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

void serialPort_disconnect(HANDLE handle)
{
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
    }
}

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

void serialPort_sendByte(HANDLE handle, uint8_t oneByte)
{
    if (handle == INVALID_HANDLE_VALUE) return;
    serialPort_sendArray(handle, &oneByte, 1);
}

DWORD serialPort_sendArray(HANDLE handle, const uint8_t *data, size_t dataLength)
{
	DWORD  numberOfbytesWritten = 0;

    if (handle == INVALID_HANDLE_VALUE) return numberOfbytesWritten;
    if (data == NULL) return numberOfbytesWritten;

	WriteFile(handle,
		    data,
		    dataLength,
		    &numberOfbytesWritten,
		    NULL);
    return numberOfbytesWritten;
}

DWORD serialPort_getNumberOfBytes(HANDLE handle)
{
    struct _COMSTAT status;
    int numberOfBytesAvailable = 0;
    unsigned long etat;

    if (handle != INVALID_HANDLE_VALUE)
    {
        ClearCommError(handle, &etat, &status);
        numberOfBytesAvailable = status.cbInQue;
    }
    
    return numberOfBytesAvailable;
}

DWORD serialPort_getArray(HANDLE handle, uint8_t *data, size_t dataLength)
{
    DWORD numberOfBytesRead = 0;

    if (handle == INVALID_HANDLE_VALUE) return numberOfBytesRead;
    if (data == NULL) return numberOfBytesRead;

    ReadFile(handle, data, dataLength, &numberOfBytesRead, NULL);
    return numberOfBytesRead;
}