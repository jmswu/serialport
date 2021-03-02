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

int main(int argc, char **argv)
{

    UNUSED_PARAM(argc);
    UNUSED_PARAM(argv);

    TEST_SERIALPORT_CONNECT_NONE_EXIST_PORT_NUMBER();

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