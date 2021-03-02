#include <stdio.h>
#include "Unity/unity.h"
#include "serialport.h"

#define UNUSED_PARAM(x)     ((void)x)

int test_done_message(void);

int main(int argc, char **argv)
{

    UNUSED_PARAM(argc);
    UNUSED_PARAM(argv);

    //TEST_ASSERT_EQUAL(0, 1);

    printf("Hello world!\n");
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