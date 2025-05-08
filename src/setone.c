/*
This file will contain the helper functions that can solve the problems
*/

 #include <stdint.h>
 #include <string.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <ctype.h>


// takes an integer and returns a base64 char
char bas64_int(uint8_t cipher){
    //taken from https://github.com/To1ne/cryptopals-c
    switch (cipher){
        case 0 ... 25: return ('A' + cipher);
        case 26 ... 51: return ('a' + (cipher-26));
        case 52 ... 61: return ('0'+ (cipher-52));
        case 62: return '+';
        case 63: return '/';
        default:
            break;
    }
    return '?';
}

//takes a char, returns a hex uint8_t
uint8_t char_to_hex(char input){
    switch(input){
        case '0'...'9': return (input - '0');
        case 'a': return 0xa;
        case 'b': return 0xb;
        case 'c': return 0xc;
        case 'd': return 0xd;
        case 'e': return 0xe;
        case 'f': return 0xf;
        default:
            break;
    }
    return -1;
}

//checks the lsbs of a byte and returns a char
char hex_to_char(uint8_t input){
    switch(input & 0x0F){
        case 0x0: return '0';
        case 0x1: return '1';
        case 0x2: return '2';
        case 0x3: return '3';
        case 0x4: return '4';
        case 0x5: return '5';
        case 0x6: return '6';
        case 0x7: return '7';
        case 0x8: return '8';
        case 0x9: return '9';
        case 0xa: return 'a';
        case 0xb: return 'b';
        case 0xc: return 'c';
        case 0xd: return 'd';
        case 0xe: return 'e';
        case 0xf: return 'f';
        default:
            break;
    }
    return -1;
}

void char_to_byte_array(char* input, uint8_t* output, int len) {
    //takes a char array and outputs a byte array. the chars are in hex so...
    for (int i = 0; i < len/2; i++){
        output[i] = (char_to_hex(input[2*i]) << 4) | (char_to_hex(input[2*i+1])); //b is now a full byte array from the hex input
    }

}

void bytes_to_char_array(uint8_t* input, char* output, int len) {
    //takes a char array and outputs a byte array. the chars are in hex so...
    //len is the length of the output
    for (int i = 0; i < len/2; i++){
        output[2*i] = hex_to_char((0xF0 & input[i])>>4);
        output[2*i+1] = hex_to_char((0x0F & input[i]));
    }

}

//checks to see the number of character frequency
int char_frequency(uint8_t* input, int len){
    int score = 0;
    int points = 0;
    for(int i = 0; i< len; i++){
        points = 0;
        switch (tolower((char)input[i])) {
            // from https://github.com/To1ne/cryptopals-c/blob/master/challenge_03/main.c
            case 'e': ++points;
            case 't': ++points;
            case 'a': ++points;
            case 'o': ++points;
            case 'i': ++points;
            case 'n': ++points; // missed the n
            case ' ': ++points;
            case 's': ++points;
            case 'h': ++points;
            case 'r': ++points;
            case 'd': ++points;
            case 'l': ++points;
            case 'u': ++points;
               score += points;
               break;
         }
    }
    return score;
}

// Functions to call to solve the challenges

void solve_s1c1(char* input, char* output, int length){
    //iterate through the input to create hex array
    if(length %2 == 1){
        printf("length of b64 encode is odd\n");
    }

    uint8_t* b = malloc(length*sizeof(uint8_t)/2); //I dont free it :(

    for (int i = 0; i < length/2; i++){
        b[i] = b[i] & 0; // remove any memory        
        b[i] = (char_to_hex(input[2*i]) << 4) | (char_to_hex(input[2*i+1])); //b is now a full byte array from the hex input
      //  printf("0x%x: ",b[i]);

    }
    //printf("%x, %x, %x\n",b[0],b[1],b[2]);
    
    int out_i = 0; // tracks the output index
    int counter = 0;
    while(counter < length/2){ // iterate over byte array
       // printf("out_i = %d, ",out_i);
        //printf("counter = %d\n",counter);

        switch(out_i%4){
            case 0: 
                output[out_i] = bas64_int(b[counter] >> 2);
                break;
            case 1: 
                output[out_i] = bas64_int(((b[counter]&0x03)<<4)|((b[counter+1]&0xF0)>>4)); 
                counter++;
                break;
            case 2: 
                output[out_i] = bas64_int(((b[counter]&0x0F)<<2)|((b[counter+1]&0xC0)>>6));
                counter++;
                break;
            case 3: 
                output[out_i] = bas64_int(b[counter]&0x3F);
                counter++;
                break;
            default:
                break;
        }
        //printf("out_i = %d, ",out_i);
        //printf("counter = %d\n",counter);

        //printf("num:%d = %c\n",out_i, output[out_i]);

        out_i++;
        
    }
    //
    free(b);
}


void solve_s1c2(char* buf1, char* buf2, char* ans, int len){
    //iterate through the buffers and write XOR to the answer
    //len is the length of the buffers
    if(len %2 == 1){
        printf("length of char string is odd\n");
    }

    uint8_t* buf1c = malloc(sizeof(uint8_t)*len/2);
    uint8_t* buf2c = malloc(sizeof(uint8_t)*len/2);
    uint8_t* ansint = malloc(sizeof(uint8_t)*len/2);

    char_to_byte_array(buf1,buf1c,len);
    char_to_byte_array(buf2,buf2c,len);


    for (int i = 0; i<len/2; i++){
        ansint[i] = buf1c[i] ^ buf2c[i];
    }

    bytes_to_char_array(ansint,ans,len);
    free(buf1c);
    free(buf2c);
    free(ansint);

}

void solve_s1c3(char* input, char* output, int len){
    // goal is to decode something which has been encoded with a single byte
    printf("I am working\n");
    uint8_t* bytes = malloc(sizeof(uint8_t)*len/2); //remember we're double packing it from chars
    uint8_t* cur = malloc(sizeof(uint8_t)*len/2);

    char_to_byte_array(input,bytes,len);


    int maximum = 0; // this is the metric we should keep
    int testscore = 0;

    for (int i = 0; i<256; i++) { // iterate over every hex value
        for (int j = 0; j< len/2; j++){
            cur[j] = bytes[j] ^ i;
        }
        testscore = 0;
        testscore = char_frequency(cur,len/2);
        if (testscore> maximum){
            maximum = testscore;
            for(int j = 0; j<len/2; j++){
                output[j] = cur[j];
            }
        }
    }
    printf("%dd\n", maximum);
    free(bytes);
    free(cur);
}