/*
This file is for runnning the expected test for all of the challenges
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "setone.h"

void s1c1_test(){

    printf("\n~~~~~~~~~~~~~~~ SET 1: CHALLENGE 1 ~~~~~~~~~~~~~~~\n");

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
    
    free(input);
    free(output);
    
}

void s1c2_test(){

    printf("\n~~~~~~~~~~~~~~~ SET 1: CHALLENGE 2 ~~~~~~~~~~~~~~~\n");
    int INPUT_LEN = 36;
    //INPUT_LEN = 6;
    char* buf1  = malloc(INPUT_LEN*sizeof(char));
    char* buf2  = malloc(INPUT_LEN*sizeof(char));


    strcpy(buf1,"1c0111001f010100061a024b53535009181c");
    strcpy(buf2,"686974207468652062756c6c277320657965");

    //strcpy(input, "999999");
    printf("Applying XOR translation to \n%s and %s\n", buf1, buf2);

    char* output  = malloc(INPUT_LEN*sizeof(char)); //allocate for output

    solve_s1c2(buf1,buf2,output, INPUT_LEN); // takes string input
    printf("it returned:\n%s\n", output);

    printf("real answer:\n%s\n", "746865206b696420646f6e277420706c6179");
    free(buf1);
    free(buf2);
    free(output);
    
}

void s1c3_test(){
    printf("\n~~~~~~~~~~~~~~~ SET 1: CHALLENGE 3 ~~~~~~~~~~~~~~~\n");

    int INPUT_LEN = 68;
    //INPUT_LEN = 6;
    char* input  = malloc(INPUT_LEN*sizeof(char));

    char* output  = malloc(sizeof(char)*INPUT_LEN/2); //allocate for output

    strcpy(input,"1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");

    //strcpy(input, "999999");
    printf("Attempting to break XOR byte encryption on:\n%s\n", input);
    solve_s1c3(input,output, INPUT_LEN); // takes string input
    printf("it returned:\n%s\n", output);
    printf("real answer:\n%s\n", "Cooking MC's like a pound of bacon");
}


void s1c4_test(){
    
    printf("\n~~~~~~~~~~~~~~~ SET 1: CHALLENGE 4 ~~~~~~~~~~~~~~~\n");
    printf("Running decryption on the file:\n");
    FILE *file = fopen("data/s1c4.txt", "r");
    char* output = malloc(sizeof(char)*60/2);
    solve_s1c4(file, output);

    printf("The code returned:\n%s", output);

    printf("The answer is:\n%s", "Now that the party is jumping\n");


}


void s1c5_test(){
    
    printf("\n~~~~~~~~~~~~~~~ SET 1: CHALLENGE 5 ~~~~~~~~~~~~~~~\n");
    printf("Encrypting with repeating XOR KEY:\n");

    char* input = malloc(sizeof(char)*74);
    char* key = malloc(sizeof(char)*3);
    char* output = malloc(sizeof(char)*74*2);

    strcpy(input,"Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal");
    strcpy(key,"ICE");

    printf("The input is is:\n%s\n", input);
    printf("The key is is:\n%s\n", key);

    solve_s1c5(input, output, key, 3, 74);

    printf("The output is:\n%s\n", output);

    printf("The answer is:\n%s\n", "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f");

}