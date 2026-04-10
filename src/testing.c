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

    int INPUT_LEN = 97; // made it a little bigger to account for the null character lol
    //INPUT_LEN = 6;
    char* input  = malloc(INPUT_LEN*sizeof(char));

    strcpy(input,"49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
    //strcpy(input, "999999");
    printf("string len = %lu\n", strlen(input));
    printf("Applying base64 translation to:\n%s\n", input);

    char* output  = malloc(INPUT_LEN*sizeof(char)); //allocate for output

    solve_s1c1(input, output, INPUT_LEN); // takes string input
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

    uint8_t* input_bytes  = malloc(sizeof(char)*INPUT_LEN/2); //allocate for output
    char* output  = malloc(sizeof(char)*INPUT_LEN/2); //allocate for output

    strcpy(input,"1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");

    char_to_byte_array(input, input_bytes, INPUT_LEN);
    for(int i = 0; i<INPUT_LEN/2; i++){
        printf("%x ", input_bytes[i]);
    }
    printf("\n");



    //strcpy(input, "999999");
    printf("Attempting to break XOR byte encryption on:\n%s\n", input);
    uint8_t key = solve_s1c3((char*) input_bytes, output, INPUT_LEN/2); // takes string input
    printf("it returned:\n%s\n", output);
    printf("real answer:\n%s\n", "Cooking MC's like a pound of bacon");
    free(input);
    free(output);
}

void s1c4_test(){
    
    printf("\n~~~~~~~~~~~~~~~ SET 1: CHALLENGE 4 ~~~~~~~~~~~~~~~\n");
    printf("Running decryption on the file:\n");
    FILE *file = fopen("data/s1c4.txt", "r");
    char* output = malloc(sizeof(char)*60/2);
    solve_s1c4(file, output);

    printf("The code returned:\n%s\n", output);

    printf("The answer is:\n%s", "Now that the party is jumping\n");
    free(output);
    fclose(file);


}

void s1c5_test(){
    
    printf("\n~~~~~~~~~~~~~~~ SET 1: CHALLENGE 5 ~~~~~~~~~~~~~~~\n");
    printf("Encrypting with repeating XOR KEY:\n");

    char* input; 
    char* key;
    char* output;

    input = malloc(sizeof(char)*74);

    key = malloc(sizeof(char)*3);
    printf("MADE IT THIS FAR\n");

    output = malloc(sizeof(char)*74*2);
    printf("MADE IT Out of MALLOC\n");



    strcpy(input, "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal");
    strcpy(key, "ICE");

    printf("The input is is:\n%s\n", input);
    printf("The key is is:\n%s\n", key);

    solve_s1c5(input, output, key, 3, 74);

    printf("The output is:\n%s\n", output);

    printf("The answer is:\n%s\n", "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f");
    
    free(input);
    free(output);
    free(key);
}

void s1c6_test(){
    printf("\n~~~~~~~~~~~~~~~ SET 1: CHALLENGE 6 ~~~~~~~~~~~~~~~\n");
    //FILE* file_pointer = fopen("data/s1c6.txt", "rb");
    FILE* file_pointer = fopen("data/s1c6.txt", "rb");
    
    if (file_pointer == NULL) {
        perror("Error opening file");
    }

    fseek(file_pointer, 0, SEEK_END);
    long file_size = ftell(file_pointer); // this is all the characters in the file.
    rewind(file_pointer);

    char* output = malloc(sizeof(char)*(file_size*6/8)); // good lord this is not the way to allocate memory :)
    solve_s1c6(file_pointer, output);
    
    printf("%s", output);
    

    fclose(file_pointer);
    free(output);

}

void s1c7_test(){
    printf("\n~~~~~~~~~~~~~~~ SET 1: CHALLENGE 7 ~~~~~~~~~~~~~~~\n");
    FILE* file = fopen("data/s1c7.txt", "rb");
    char *buffer;
    long file_size;
    if (file == NULL) {
        perror("Error opening file");
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file); // this is all the characters in the file.
    rewind(file);

    buffer = (char*)malloc(sizeof(char) * (file_size + 1)); // +1 for \0
    if (buffer == NULL) {
        perror("Error allocating memory");
        fclose(file);
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read != file_size) {
        perror("Error reading file");
        fclose(file);
    }

    buffer[file_size] = '\0'; // buffer is a string object representing the file.

    //printf(buffer);
    
    char * strip_buffer = (char*)malloc(sizeof(char) * (file_size + 1)); // +1 for \0
    int i = 0;  //buffer iterator
    int buf_len = 0; //strip iterator and final length
    
    while(buffer[i] != '\0'){
        if(buffer[i] == '\n'){
            i++;
            continue;
        }
        else {
            strip_buffer[buf_len] = buffer[i];
            i++;
            buf_len++;
        }
    }
    strip_buffer[buf_len] = '\0';
    
    uint8_t * byte_array = (uint8_t*)malloc(sizeof(uint8_t) * ((buf_len * 6) / 8));
    int array_len = (buf_len * 6) / 8; // lenght of th byte array...last bytes might be padding
    // POTENTIAL HACKY FIXTODO: fix the hacky thing did a - 1 for padding.

    //printf("Before: %s\n", strip_buffer);
    base64_to_bytes(strip_buffer, byte_array, buf_len, array_len);
    //printf("After: ");
    //for(i = 0; i < array_len; i++)
      //  printf("%x ", byte_array[i]);
    
    char* key = "YELLOW SUBMARINE"; // yes it is actually the key
    int p_len;
    uint8_t* plaintext; // will be allocated in the functions so it's gotta be freed outside of it
    int key_len = 16; // becasue 128 = 8 * 16. 
    p_len = array_len;
    plaintext =(uint8_t*)malloc(sizeof(uint8_t) * (p_len)); // +1 for \0

    solve_s1c7(byte_array, key, plaintext, array_len, key_len, p_len);
    
    printf("plaintext: ");
    printf("%s", plaintext);
    printf("END TESTing\n");
    free(plaintext); 
    free(byte_array);
    free(strip_buffer);

    fclose(file);


}

void s1c8_test(){
    printf("\n~~~~~~~~~~~~~~~ SET 1: CHALLENGE 8 ~~~~~~~~~~~~~~~\n");
    FILE* file = fopen("data/s1c8.txt", "rb");
    char *buffer;
    long file_size;
    if (file == NULL) {
        perror("Error opening file");
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file); // this is all the characters in the file.
    rewind(file);

    buffer = (char*)malloc(sizeof(char) * (file_size + 1)); // +1 for \0
    if (buffer == NULL) {
        perror("Error allocating memory");
        fclose(file);
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read != file_size) {
        perror("Error reading file");
        fclose(file);
    }
    int i; int j;
    int num_lines = 0;
    int buf_len = 0; //strip iterator and final length
    int linelen = 0;

    uint8_t byte_array[204][160];
    uint8_t temp[160] = {0x00};
    char temp_c[320];
    int c_i = 0;
    int k = 0; // row counter
    int col = 0; //col counter, inner for
    while(buffer[i] != '\0'){
        if(buffer[i] == '\n'){
            char_to_byte_array(temp_c, temp, 320);
            for(col = 0; col < 160; col++)
                byte_array[k][col] = temp[col];
            k++;
            i++;
            num_lines++;
            c_i = 0;
            continue;
        }
        else {
            temp_c[c_i] = buffer[i];
            c_i++;
            i++;
            linelen++;
        }
    }

    printf("number of lines = %d, linelen = %d\n", num_lines, linelen/num_lines/2);
    if(num_lines != 204)
        printf("missallocated array 204!\n");
    if(linelen/num_lines/2 != 160)
        printf("missallocated 160!\n");

    linelen = linelen/num_lines/2;

    int line_index = solve_s1c8(byte_array, num_lines);
    
    uint8_t cipher[160];
    printf("the line is:\n");

    for(i = 0; i < 160; i++){
        cipher[i] = byte_array[132][i];
        printf("%x ", cipher[i]);
    }
    printf("\n");


    fclose(file);
    
}