/*
This file will contain the helper functions that can solve the problems

 */

 #include <stdint.h>
 #include <string.h>
 #include <stdio.h>
 #include <stdlib.h>

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

void solve_s1c1(char* input, char* output, int length){
    //iterate through the input to create hex array
    if(length %2 == 1){
        printf("length of b64 encode is odd\n");
    }

    uint8_t* b = malloc(length*sizeof(uint8_t)/2);

    for (int i = 0; i < length/2; i++){
        b[i] = b[i] & 0; // remove any memory        
        b[i] = (char_to_hex(input[2*i]) << 4) | (char_to_hex(input[2*i+1])); //b is now a full byte array from the hex input
        printf("0x%x: ",b[i]);

    }
    printf("%x, %x, %x\n",b[0],b[1],b[2]);
    
    int out_i = 0; // tracks the output index
    int counter = 0;
    while(counter < length/2){ // iterate over byte array
        printf("out_i = %d, ",out_i);
        printf("counter = %d\n",counter);

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
        printf("out_i = %d, ",out_i);
        printf("counter = %d\n",counter);

        printf("num:%d = %c\n",out_i, output[out_i]);

        out_i++;
        
    }
    //
}