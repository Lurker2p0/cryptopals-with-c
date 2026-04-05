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

uint8_t base64_to_hex(char cipher){
    //takes a b64 char and outputs the hex of that in a six bit format
    switch (cipher){
        case 'A'...'Z': return (cipher -'A');
        case 'a' ... 'z': return (cipher-'a')+26;
        case '0'...'9': return (cipher-'0')+52;
        case '+': return 62;
        case '/': return 63;
        default:
            break;
    }
    printf("UH OH BROTHER fail in base64 to hex:\'%d\'\n",(int)cipher);
    exit(EXIT_FAILURE);
    return '?';
}


//01001001 00100111 01101101

void base64_to_bytes(char* input, uint8_t* output, int len_i, int len_o){
    //takes something that's been encoded in b64 and returns an array of bytes
    //we assume that len_i is exactly the number of b64 characters without \n
    //iterate through each char from the input
    // which is N*6 bytes, which is N*6/8 points 
    char* temp = malloc(sizeof(char) * (len_i));
    int offset = 0;
    int i;
    for(i = 0; i < len_i; i++){ // for every character...
        if (input[i] == '\n' || input[i]=='='){ // check for things that dont have bytes in em
            offset++;
            continue;
        }
        temp[i-offset] = base64_to_hex(input[i]);
    }
    
    //make it into actual byte values

    int i_sb = 0;

    uint8_t temp1;
    uint8_t temp2;

    for (i = 0; i < len_o; i++){
        temp1 = temp[i_sb];
        temp2 = temp[i_sb+1];
        switch (i%3)
        {
        case 0:
            output[i] = ((temp1<<2) & 0xFC) | ((temp2>>4) & 0x03); // bruh
            i_sb++;
            break;
        case 1:
            output[i] = (((temp1 & 0x0F) << 4)) | ((temp2 & 0x3C)>>2); // jank af
            i_sb++;
            break;
        case 2:
            /* code */
            output[i] = ((temp1 & 0x03) << 6 ) | (temp2);
            i_sb++;
            i_sb++;
            break;
        default:
            break;
        }

    }
    free(temp);
    
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
    //used to make challenge inputs into actual hex numbers
    char temp1;
    char temp2;
    for (int i = 0; i < len/2; i++){
        temp1 = input[2*i];
        temp2 = input[2*i+1];
        output[i] = (char_to_hex(temp1) << 4) | (char_to_hex(temp2)); //b is now a full byte array from the hex input
    }

}

void bytes_to_char_array(uint8_t* input, char* output, int len) {
    //takes a char array and outputs a byte array. the chars are in hex so...
    //len is the length of the output
    for (int i = 0; i < len/2; i++){
        output[2*i] = hex_to_char((0xF0 & input[i])>>4);
        output[2*i+1] = hex_to_char((0x0F & input[i]));
    }
   //printf("%s\n",output);
    
}

//checks to see the number of character frequency
int char_frequency(char* input, int len){
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


int hamming_distance(unsigned char* buf1, unsigned char* buf2, int len){
    //length is the len of a buffer
    int dist = 0;
    uint8_t cur = 0;
    for(int i = 0; i<len; i++){
        cur = buf1[i] ^ buf2[i];
        for (int j = 0; j< 7; j++){
            dist+= cur%2;
            cur = cur >>1;
        }
    }
    return dist;

}

// Functions to call to solve the challenges
void solve_s1c1(char* input, char* output, int length){
    //Since the input is a string that represents a hexidecimal value we can't just take every character
    //as itself. We have to make the correct byte. so the two byte length string '49' refers to the single
    //byte 0x49

    //iterate through the input to create hex array
    if(length %2 == 1){
        printf("length of b64 encode is odd and accounts for \\0 character \n");
    }

    uint8_t* b = malloc(sizeof(uint8_t)*(length/2));

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

    uint8_t* buf1c = malloc(sizeof(uint8_t) * len/2);
    uint8_t* buf2c = malloc(sizeof(uint8_t) * len/2);
    uint8_t* ansint = malloc(sizeof(uint8_t) * len/2);

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

uint8_t solve_s1c3(char* input, char* output, int len){
    // make sure it's already a byte array and not a char input string. 
    // goal is to decode something which has been encoded with a single byte
    char* cur = malloc(sizeof(char)*len);

    int maximum = 0; // this is the metric we should keep
    int testscore = 0;
    int Key = 0;
    int i;
    char tempi;
    for (i = 0; i<256; i++) { //iterate over every potential byte value
        //printf("Key: %x: ", i);
        for (int j = 0; j < len; j++){
            tempi = input[j];
            cur[j] = tempi ^ i;
            //printf("%x", cur[j]);
        }
        testscore = 0;
        testscore = char_frequency(cur,len);
        if (testscore>0)
            //printf(" Score = %d\n", testscore);
        if (testscore > maximum){
            maximum = testscore;
            Key = i;
            for(int j = 0; j<len; j++){
                output[j] = cur[j];
            }
        }
    }
    printf("The key was %X\n", Key);
    free(cur);
    return Key;
}

void solve_s1c4(FILE *file, char* ans){
    // need to have a cap on the end of the file :) security related
    //len is the max len of the file
    int LEN = 256;
    if(file==NULL){ // how to read file was from ai overview
        perror("error opening file");
    }
    char* line = malloc(sizeof(char)*LEN); // put this on the heap and it worked :)
    int LINE_LEN = 60;
    int i = 0;
    int max = 0; // track max score
    int test = 0; // track max score
    char* output = malloc(sizeof(char)*LINE_LEN/2); // assumes that each line is 60 characters
    while(fgets(line, LEN,file)!= NULL){
        solve_s1c3(line, output, LINE_LEN); // might fail on line 93 
        test = char_frequency(output, LINE_LEN/2);
        if (test>max){
            max = test;
            for (int j = 0; j<LINE_LEN/2;j++){
                ans[j] = output[j];
            }
        }
        i++;
    }

    free(line);
    free(output);
}


void solve_s1c5(char* input, char* output, char* key, int keylen, int len){
    
    uint8_t* output_ba = malloc(sizeof(uint8_t)*len);
    for(int i = 0; i < len; i++){ // iterate over bytes
        printf("%X xor %X --> ", input[i], key[i % keylen]);
        output_ba[i] = input[i] ^ key[i % keylen]; // key is ok because it's meant to be a char :)
        printf("%X\n", output_ba[i]);
    }

    bytes_to_char_array(output_ba,output, len*2);
    free(output_ba);

}

void solve_s1c6(FILE *file, char* ans){
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

    printf("Before: %s\n", strip_buffer);
        
    char * key = "ICE";
    base64_to_bytes(strip_buffer, byte_array, buf_len, array_len);

    for(i=0; i<array_len-1; i++)
        printf("%x ",byte_array[i]);
    printf("\n");

    for(i=0; i<array_len-1; i++)
        printf("%c",byte_array[i] ^ key[i % 3]);
    printf("\n");

    int KEYSIZE;

    int min_key;
    double min_score = 1000000;
    double cur_score = 1000000;

    for(KEYSIZE = 2; KEYSIZE < 41; KEYSIZE++){
        cur_score = (double)(hamming_distance(byte_array, &byte_array[KEYSIZE], KEYSIZE))/KEYSIZE;
        cur_score += (double)(hamming_distance(&byte_array[KEYSIZE], &byte_array[2*KEYSIZE], KEYSIZE))/KEYSIZE;
        cur_score += (double)(hamming_distance(&byte_array[2*KEYSIZE], &byte_array[3*KEYSIZE], KEYSIZE))/KEYSIZE;
        cur_score += (double)(hamming_distance(&byte_array[3*KEYSIZE], &byte_array[4*KEYSIZE], KEYSIZE))/KEYSIZE;
        cur_score += (double)(hamming_distance(&byte_array[4*KEYSIZE], &byte_array[5*KEYSIZE], KEYSIZE))/KEYSIZE;
        cur_score = cur_score / 5;
        printf("KEYSIZE = %d, score = %0.2f\n", KEYSIZE, cur_score);
        if (cur_score < min_score){
            min_score = cur_score;
            min_key = KEYSIZE;
            if(cur_score < 3.0)
                printf("HEREHREHRHEH");
        }
        cur_score = 0;
    }
    printf("min_key = %d, with min_score = %0.2f\n", min_key, min_score); // Probably gonna be a keysize of 5
    KEYSIZE = 29;

    char * temp_arr = (char*)malloc(sizeof(char) * (array_len/KEYSIZE)); // make it just bigger lol
    char * sol_arr = (char*)malloc(sizeof(char) * (array_len/KEYSIZE)); // make it

    int j; // inner iterator
    int counter = 0;
    
    uint8_t thekey[KEYSIZE];

    for(i = 0; i < KEYSIZE; i++){ // for each keysize value
        //printf("Temp array %d: ", i);
        for(j = i; j < array_len-1; j += KEYSIZE) { // fill up the array
            temp_arr[counter] = byte_array[j];
            //printf("%x ",temp_arr[counter]);
            counter++;
        }
        thekey[i] = solve_s1c3(temp_arr, sol_arr, counter);
        counter = 0;
        sol_arr[array_len] = '\0';
        //printf("%s\n",sol_arr);
    }


    for(i = 0; i < array_len; i++)
        byte_array[i] = byte_array[i] ^ thekey[i%KEYSIZE];

    printf("\nActual: ");
    byte_array[array_len] = '\0';
    printf("%s\n", byte_array);


    free(buffer);
    free(strip_buffer);
    fclose(file);

}


void solve_s1c7(){

}

void solve_s1c8(){

}