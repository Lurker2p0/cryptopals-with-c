#include <stdio.h>
#include <stdint.h>
#include "setone.h"


int main(){
    printf("\nProgram is running:\n");
    char x = bas64_int(0x1);
    printf("%c\n", x);
}


//I need a function to take a string in here and 