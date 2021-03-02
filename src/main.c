#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Unity/unity.h"
#include "serialport.h"

#define UNUSED_PARAM(x)     ((void)x)

int test_done_message(void);

/* test need to be done with a loop back serial port */
#define PORT_NUM_LOOP_BACK          (12)
#define PORT_NUM_NONE_EXIST_1       (30)
#define PORT_NUM_NONE_EXIST_2       (31)
void TEST_SERIALPORT_CONNECT_NONE_EXIST_PORT_NUMBER(void);
void TEST_SERIALPORT_CONNECT_SIMPLE_NONE_EXIST_PORT_NUMBER(void);
void TEST_SERIALPORT_CONNECT_SUCCESSFULLY(void);
void TEST_SERIALPORT_CONNECT_SIMPLE_SUCCESSFULLY(void);
void TEST_SERIALPORT_CONNECT_WRONG_PARITY(void);
void TEST_SERIALPORT_CONNECT_WRONG_STOPBITS(void);
void TEST_SERIALPORT_SEND_AND_RECEIVE_ONE_BYTE(void);
void TEST_SERIALPORT_SEND_ONE_BYTE_INVALID_HANDLE(void);
void TEST_SERIALPORT_RECEIVED_ONE_BYTE_INVALID_HANDLE(void);
void TEST_SERIALPORT_SEND_AND_RECEIVE_ARRAY(void);
void TEST_SERIALPORT_GET_NUMBER_OF_BYTES(void);
void TEST_SERIALPORT_GET_NUMBER_OF_BYTES_INVALID_HANDLE(void);

void test_helper_fill_random_byte(uint8_t *array, unsigned arrayLength);
void test_helper_wait_for_data_to_be_received(unsigned baudRate, unsigned dataLength);

int main(int argc, char **argv)
{

    UNUSED_PARAM(argc);
    UNUSED_PARAM(argv);

    srand(time(NULL));

    TEST_SERIALPORT_CONNECT_NONE_EXIST_PORT_NUMBER();
    TEST_SERIALPORT_CONNECT_SUCCESSFULLY();
    TEST_SERIALPORT_CONNECT_WRONG_PARITY();
    TEST_SERIALPORT_CONNECT_WRONG_STOPBITS();
    TEST_SERIALPORT_CONNECT_SIMPLE_SUCCESSFULLY();
    TEST_SERIALPORT_CONNECT_SIMPLE_NONE_EXIST_PORT_NUMBER();
    TEST_SERIALPORT_SEND_AND_RECEIVE_ONE_BYTE();
    TEST_SERIALPORT_SEND_AND_RECEIVE_ARRAY();
    TEST_SERIALPORT_GET_NUMBER_OF_BYTES();
    TEST_SERIALPORT_GET_NUMBER_OF_BYTES_INVALID_HANDLE();
    TEST_SERIALPORT_SEND_ONE_BYTE_INVALID_HANDLE();
    TEST_SERIALPORT_RECEIVED_ONE_BYTE_INVALID_HANDLE();

    return test_done_message();
}

void setUp(void){}
void tearDown(void){}

int test_done_message(void)
{
    printf("--------------------------------------------------------------------\n");
    printf("Testing serial port library v%u.%u.%u\n",
        (serialPort_getLibraryVersion() >> 16) & 0xFF,
        (serialPort_getLibraryVersion() >> 8) & 0xFF,
        (serialPort_getLibraryVersion() >> 0)& 0xFF);
    printf("If you only see this message, it means all tests are passed\n");
    printf("--------------------------------------------------------------------\n");

    return 0;
}

void TEST_SERIALPORT_CONNECT_NONE_EXIST_PORT_NUMBER(void)
{
    HANDLE handle_1 = serialPort_connect(PORT_NUM_NONE_EXIST_1, 9600, 8, NOPARITY, ONESTOPBIT);
    TEST_ASSERT_EQUAL(INVALID_HANDLE_VALUE, handle_1);
    serialPort_disconnect(handle_1);

    HANDLE handle_2 = serialPort_connect(PORT_NUM_NONE_EXIST_2, 9600, 8, NOPARITY, ONESTOPBIT);
    TEST_ASSERT_EQUAL(INVALID_HANDLE_VALUE, handle_2);
    serialPort_disconnect(handle_2);
}

void TEST_SERIALPORT_CONNECT_SUCCESSFULLY(void)
{
    HANDLE handle = serialPort_connect(PORT_NUM_LOOP_BACK, 9600, 8, NOPARITY, ONESTOPBIT);
    TEST_ASSERT_NOT_EQUAL(INVALID_HANDLE_VALUE, handle);
    serialPort_disconnect(handle);
}

void TEST_SERIALPORT_CONNECT_WRONG_PARITY(void)
{
    HANDLE handle_1 = serialPort_connect(PORT_NUM_LOOP_BACK, 9600, 8, 5, ONESTOPBIT);
    TEST_ASSERT_EQUAL(INVALID_HANDLE_VALUE, handle_1);
    serialPort_disconnect(handle_1);

    HANDLE handle_2 = serialPort_connect(PORT_NUM_LOOP_BACK, 9600, 8, ~0, ONESTOPBIT);
    TEST_ASSERT_EQUAL(INVALID_HANDLE_VALUE, handle_2);
    serialPort_disconnect(handle_2);
}

void TEST_SERIALPORT_CONNECT_WRONG_STOPBITS(void)
{
    HANDLE handle_1 = serialPort_connect(PORT_NUM_LOOP_BACK, 9600, 8, NOPARITY, 3);
    TEST_ASSERT_EQUAL(INVALID_HANDLE_VALUE, handle_1);
    serialPort_disconnect(handle_1);

    HANDLE handle_2 = serialPort_connect(PORT_NUM_LOOP_BACK, 9600, 8, NOPARITY, ~0);
    TEST_ASSERT_EQUAL(INVALID_HANDLE_VALUE, handle_2);
    serialPort_disconnect(handle_2);
}

void TEST_SERIALPORT_CONNECT_SIMPLE_SUCCESSFULLY(void)
{
    HANDLE handle = serialPort_connectSimple(PORT_NUM_LOOP_BACK, 9600);
    TEST_ASSERT_NOT_EQUAL(INVALID_HANDLE_VALUE, handle);
    serialPort_disconnect(handle);
}

void TEST_SERIALPORT_CONNECT_SIMPLE_NONE_EXIST_PORT_NUMBER(void)
{
    HANDLE handle_1 = serialPort_connectSimple(PORT_NUM_NONE_EXIST_1, 9600);
    TEST_ASSERT_EQUAL(INVALID_HANDLE_VALUE, handle_1);
    serialPort_disconnect(handle_1);

    HANDLE handle_2 = serialPort_connectSimple(PORT_NUM_NONE_EXIST_2, 9600);
    TEST_ASSERT_EQUAL(INVALID_HANDLE_VALUE, handle_2);
    serialPort_disconnect(handle_2);
}

void TEST_SERIALPORT_SEND_AND_RECEIVE_ONE_BYTE(void)
{
    const unsigned BAUD_RATE = 19200;
    HANDLE handle = serialPort_connectSimple(PORT_NUM_LOOP_BACK, BAUD_RATE);

    /* test with sequcnecial data */
    for(unsigned i = 0; i < 256; i++)
    {
        serialPort_sendByte(handle, i);
        test_helper_wait_for_data_to_be_received(BAUD_RATE, 1);
        uint8_t rxData = serialPort_getOneByte(handle);
        TEST_ASSERT_EQUAL_UINT8(i, rxData);
    }

    /* test with random data */
    for(unsigned i = 0; i < 256; i++)
    {
        uint8_t txData = rand() & 0xFF;
        serialPort_sendByte(handle, txData);
        test_helper_wait_for_data_to_be_received(BAUD_RATE, 1);
        uint8_t rxData = serialPort_getOneByte(handle);
        TEST_ASSERT_EQUAL_UINT8(txData, rxData);
    }

    serialPort_disconnect(handle);
}

void TEST_SERIALPORT_SEND_ONE_BYTE_INVALID_HANDLE(void)
{
    HANDLE handle = serialPort_connectSimple(PORT_NUM_LOOP_BACK, 19200);

    /* test with random data */
    for(unsigned i = 0; i < 256; i++)
    {
        uint8_t txData = rand() & 0xFF;
        serialPort_sendByte(NULL, txData);
        uint8_t rxData = serialPort_getOneByte(handle);
        TEST_ASSERT_EQUAL_UINT8(0, rxData);
    }

    serialPort_disconnect(handle);
}

void TEST_SERIALPORT_RECEIVED_ONE_BYTE_INVALID_HANDLE(void)
{
    HANDLE handle = serialPort_connectSimple(PORT_NUM_LOOP_BACK, 19200);

    /* test with random data */
    for(unsigned i = 0; i < 256; i++)
    {
        uint8_t txData = rand() & 0xFF;
        serialPort_sendByte(handle, txData);
        uint8_t rxData = serialPort_getOneByte(NULL);
        TEST_ASSERT_EQUAL_UINT8(0, rxData);
    }

    serialPort_disconnect(handle);
}

void TEST_SERIALPORT_SEND_AND_RECEIVE_ARRAY(void)
{
    const unsigned BAUD_RATE = 57600;
    HANDLE handle = serialPort_connectSimple(PORT_NUM_LOOP_BACK, BAUD_RATE);

    const unsigned TEST_ARRAY_SIZE = 256;
    uint8_t txArray[TEST_ARRAY_SIZE];
    uint8_t rxArray[TEST_ARRAY_SIZE];

    test_helper_fill_random_byte(txArray, TEST_ARRAY_SIZE);

    DWORD txDataLength = serialPort_sendArray(handle, txArray, TEST_ARRAY_SIZE);
    test_helper_wait_for_data_to_be_received(BAUD_RATE, TEST_ARRAY_SIZE);
    DWORD rxDataLength = serialPort_getArray(handle, rxArray, TEST_ARRAY_SIZE);

    TEST_ASSERT_EQUAL(txDataLength, TEST_ARRAY_SIZE);
    TEST_ASSERT_EQUAL(rxDataLength, TEST_ARRAY_SIZE);
    TEST_ASSERT_EQUAL(rxDataLength, txDataLength);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(txArray, rxArray, TEST_ARRAY_SIZE);

    serialPort_disconnect(handle);
}

void TEST_SERIALPORT_GET_NUMBER_OF_BYTES(void)
{
    const unsigned BAUD_RATE = 57600;
    HANDLE handle = serialPort_connectSimple(PORT_NUM_LOOP_BACK, BAUD_RATE);

    unsigned randomDataLength = rand() & 0xFF;
    for(unsigned i = 0; i < randomDataLength; i++)
    {
        uint8_t randomByte = rand() & 0xFF;
        serialPort_sendByte(handle, randomByte);
    }

    test_helper_wait_for_data_to_be_received(BAUD_RATE, randomDataLength);

    unsigned numberOfDataAvailable = serialPort_getNumberOfBytes(handle);

    TEST_ASSERT_EQUAL(randomDataLength, numberOfDataAvailable);

    serialPort_disconnect(handle);
}

void TEST_SERIALPORT_GET_NUMBER_OF_BYTES_INVALID_HANDLE(void)
{
    const unsigned BAUD_RATE = 57600;
    HANDLE handle = serialPort_connectSimple(PORT_NUM_LOOP_BACK, BAUD_RATE);

    unsigned randomDataLength = rand() & 0xFF;
    for(unsigned i = 0; i < randomDataLength; i++)
    {
        uint8_t randomByte = rand() & 0xFF;
        serialPort_sendByte(handle, randomByte);
    }

    test_helper_wait_for_data_to_be_received(BAUD_RATE, randomDataLength);

    unsigned numberOfDataAvailable = serialPort_getNumberOfBytes(NULL);

    TEST_ASSERT_EQUAL_UINT(0, numberOfDataAvailable);

    serialPort_disconnect(handle);
}


void test_helper_fill_random_byte(uint8_t *array, unsigned arrayLength)
{
    if (array == NULL) return;

    for(unsigned i = 0; i < arrayLength; i++)
    {
        array[i] = rand() & 0xFF;
    }
}

void test_helper_wait_for_data_to_be_received(unsigned baudRate, unsigned dataLength)
{
    const float TIME_ONE_BYTE_IN_ms = ((float)1000/(float)baudRate) * 10;
    const float TIME_ROUNDING_ERROR_IN_ms = 1;
    const float TIME_TOTAL_IN_ms = (TIME_ONE_BYTE_IN_ms * dataLength) + TIME_ROUNDING_ERROR_IN_ms;
    Sleep((unsigned)TIME_TOTAL_IN_ms);
}