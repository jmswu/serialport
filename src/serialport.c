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
	HANDLE hPort;
	TCHAR portName[100];
	wsprintf(portName, TEXT("\\\\.\\COM%d"), portNumber);
	hPort = CreateFile(portName,            //port name 
		GENERIC_READ | GENERIC_WRITE,       //Read/Write   				 
		0,            // No Sharing                               
		NULL,         // No Security                              
		OPEN_EXISTING,// Open existing port only                     
		0,            // Non Overlapped I/O                           
		NULL);        // Null for Comm Devices

    return hPort;
}

static BOOL setCommunicationBaudRate(HANDLE handle, unsigned baudRate)
{
	DCB dcbSerialParams = { 0 };
	BOOL Status;
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	Status = GetCommState(handle, &dcbSerialParams);
	if (Status == FALSE)
		return FALSE;
	dcbSerialParams.BaudRate = baudRate;
	Status = SetCommState(handle, &dcbSerialParams);
	return Status;
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
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
    return SetCommTimeouts(handle, &timeouts);
}

HANDLE serialPort_connect(unsigned portNumber, unsigned baudRate, unsigned dataBits, unsigned parity, unsigned stopBits)
{
    HANDLE hPort = openCommunication(portNumber);

    if (hPort == INVALID_HANDLE_VALUE)
    {
        serialPort_disconnect(hPort);
        return INVALID_HANDLE_VALUE;
    }
        
	if (setCommunicationTimeouts(hPort) == FALSE)
    {
        serialPort_disconnect(hPort);
        return INVALID_HANDLE_VALUE;
    }

	if (SetCommMask(hPort, EV_RXCHAR) == FALSE)
    {
        serialPort_disconnect(hPort);
        return INVALID_HANDLE_VALUE;
    }

    if (setCommunicationBaudRate(hPort, baudRate) == FALSE)
    {
        serialPort_disconnect(hPort);
        return INVALID_HANDLE_VALUE;
    }

    if (setCommunicationDataBits(hPort, dataBits == FALSE))
    {
        serialPort_disconnect(hPort);
        return INVALID_HANDLE_VALUE;
    }

    if (setCommunicationParity(hPort, parity) == FALSE)
    {
        serialPort_disconnect(hPort);
        return INVALID_HANDLE_VALUE;
    }

    if (setCommunicationStopBits(hPort, stopBits) == FALSE)
    {
        serialPort_disconnect(hPort);
        return INVALID_HANDLE_VALUE;
    }

    return hPort;      
}

void serialPort_disconnect(HANDLE handle)
{
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
    }
}