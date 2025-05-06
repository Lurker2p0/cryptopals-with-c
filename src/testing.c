/*
This file is for runnning the expected test for all of the challenges
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "setone.h"

void s1c1_test(){
    int INPUT_LEN = 96;
    //INPUT_LEN = 6;
    char* input  = malloc(INPUT_LEN*sizeof(char));

    strcpy(input,"49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
    //strcpy(input, "999999");
    printf("Applying base64 translation to:\n%s\n", input);

    char* output  = malloc(INPUT_LEN*sizeof(char)); //allocate for output

    solve_s1c1(input,output,INPUT_LEN); // takes string input
    printf("it returned:\n%s\n", output);

    printf("the answer should have been:\n%s\n", "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t");
    
}