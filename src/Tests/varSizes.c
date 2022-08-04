#include <stdio.h>
#include "pico/stdlib.h"

int main(void)
{
    
    stdio_init_all();
    //int a = -4294967295;
    int a = 2147483648;
    
    printf("\nOn RP2040\n");
    
    printf("int is %lu bytes.\n", sizeof(int));
    printf("a is %i bytes long.\n", sizeof(a));
    printf("long int is %lu bytes.\n", sizeof(long int));
    printf("char is %lu bytes.\n", sizeof(char));
    printf("float is %lu bytes.\n", sizeof(float));
    printf("double is %lu bytes.\n", sizeof(double));
    printf("long double is %lu bytes.\n", sizeof(long double));
    return 0;
    

}
