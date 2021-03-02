#include <stdio.h>

#define UNUSED_PARAM(x)     ((void)x)

int main(int argc, char **argv)
{

    UNUSED_PARAM(argc);
    UNUSED_PARAM(argv);
    
    printf("Hello world!\n");
    return 0;
}

void setUp(void){}
void tearDown(void){}