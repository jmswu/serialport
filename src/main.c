#include <stdio.h>
#include "Unity/unity.h"
#include "serialport.h"

#define UNUSED_PARAM(x)     ((void)x)

int test_done_message(void);

/* test need to be done with a loop back serial port */
#define PORT_NUM_LOOP_BACK          (12)
#define PORT_NUM_NONE_EXIST_1       (30)
#define PORT_NUM_NONE_EXIST_2       (31)
void TEST_SERIALPORT_CONNECT_NONE_EXIST_PORT_NUMBER(void);
void TEST_SERIALPORT_CONNECT_SUCCESSFULLY(void);
void TEST_SERIALPORT_CONNECT_WRONG_PARITY(void);
void TEST_SERIALPORT_CONNECT_WRONG_STOPBITS(void);
void TEST_SERIALPORT_CONNECT_SIMPLE_SUCCESSFULLY(void);
void TEST_SERIALPORT_CONNECT_SIMPLE_NONE_EXIST_PORT_NUMBER(void);
void TEST_SERIALPORT_SEND_AND_RECEIVE_ONE_BYTE(void);

int main(int argc, char **argv)
{

    UNUSED_PARAM(argc);
    UNUSED_PARAM(argv);

    TEST_SERIALPORT_CONNECT_NONE_EXIST_PORT_NUMBER();
    TEST_SERIALPORT_CONNECT_SUCCESSFULLY();
    TEST_SERIALPORT_CONNECT_WRONG_PARITY();
    TEST_SERIALPORT_CONNECT_WRONG_STOPBITS();
    TEST_SERIALPORT_CONNECT_SIMPLE_SUCCESSFULLY();
    TEST_SERIALPORT_CONNECT_SIMPLE_NONE_EXIST_PORT_NUMBER();
    TEST_SERIALPORT_SEND_AND_RECEIVE_ONE_BYTE();

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
    HANDLE handle = serialPort_connectSimple(PORT_NUM_LOOP_BACK, 19200);

    for(unsigned i = 0; i < 256; i++)
    {
        serialPort_sendByte(handle, i);
        uint8_t rxData = serialPort_getOneByte(handle);
        TEST_ASSERT_EQUAL_UINT8(i, rxData);
    }

    serialPort_disconnect(handle);
}