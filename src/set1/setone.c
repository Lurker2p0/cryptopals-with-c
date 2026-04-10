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
    //printf("The key was %X\n", Key);
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
    uint8_t* line_arr = malloc(sizeof(char)*LINE_LEN/2); // assumes that each line is 60 characters
    char* output = malloc(sizeof(char)*LINE_LEN/2); // assumes that each line is 60 characters

    //char_to_byte_array(line,output,LINE_LEN);

    while(fgets(line, LEN, file)!= NULL){
        char_to_byte_array(line,line_arr,LINE_LEN);
        solve_s1c3((char*)line_arr, output, LINE_LEN/2); // might fail on line 93 
        test = char_frequency(output, LINE_LEN/2);
        if (test>max){
            max = test;
            for (int j = 0; j<LINE_LEN/2;j++){
                ans[j] = output[j];
            }
        }
        i++;
    }

    free(line_arr);
    free(output);
    free(line);
}


void solve_s1c5(char* input, char* output, char* key, int keylen, int len){
    
    uint8_t* output_ba = malloc(sizeof(uint8_t)*len);
    for(int i = 0; i < len; i++){ // iterate over bytes
        //printf("%X xor %X --> ", input[i], key[i % keylen]);
        output_ba[i] = input[i] ^ key[i % keylen]; // key is ok because it's meant to be a char :)
        //printf("%X\n", output_ba[i]);
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
    int array_len = (buf_len * 6) / 8 - 1; // lenght of th byte array...last bytes might be padding
    // POTENTIAL HACKY FIXTODO: fix the hacky thing did a - 1 for padding.

    //printf("Before: %s\n", strip_buffer);
        
    base64_to_bytes(strip_buffer, byte_array, buf_len, array_len);

    //for(i=0; i<array_len-1; i++)
        //printf("%x ",byte_array[i]);
    //printf("\n");

    //for(i=0; i<array_len-1; i++)
     //   printf("%c",byte_array[i] ^ key[i % 3]);
    //printf("\n");

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
        //printf("KEYSIZE = %d, score = %0.2f\n", KEYSIZE, cur_score);
        if (cur_score < min_score){
            min_score = cur_score;
            min_key = KEYSIZE;
        }
        cur_score = 0;
    }
    //printf("min_key = %d, with min_score = %0.2f\n", min_key, min_score); // Probably gonna be a keysize of 5
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
    free(byte_array);
    free(strip_buffer);
    free(sol_arr);
    free(temp_arr);

    fclose(file);

}

//~~~~~~~~~~AES HELPER FUNCTIONS AND LOOKUP TABLES BELOW~~~~~~~~~~

// maybe i can get away with including the inverse functionality inside the inital function

//an exercise that calculates and prints the forward SBOX, it'll be pasted in later as a LUT

uint8_t mulInv(int value) { // returns the multiplicatvie inverse in GF(2^8); 
    //good thing addition and multiplication already performed mod 2^8.

    if(value == 0x00)
        return 0x00;

    for (int x = 1; x < 256; x++)
        if (((value % 256) * (x % 256)) % 256 == 1)
            return (uint8_t)x;
    return -1;
}
void print_SBOX(){
    printf("STARTING PRINT OF THE SBOX:\n");
    int i,j; // itirators
    uint8_t SBox[16][16];

    for(i=0; i<16; i++) // initalize as psudo multiplication table
        for(j=0; j<16; j++)
            SBox[i][j] = ((i<<4) | j);
    
    for(i=0; i<16; i++) // initalize as psudo multiplication table
        for(j=0; j<16; j++)
            SBox[i][j] = mulInv((int)SBox[i][j]);


    //printing the SBOX
    printf("uint8_t SBOX[16][16] = {");
    for(i=0; i<16; i++){
        printf("{");
        for(j=0; j<16; j++){
            printf("0x%02X",SBox[i][j]);
            if((i==15) && (j==15))
                printf("}}\n");
            else if(j==15)
                printf("}\n\t\t\t");
            else
                printf(", ");
        }
    }
    printf("ENDING PRINT OF THE SBOX\n");

}

//an exercise that calculates and prints the forward SBOX, it'll be pasted in later as a LUT
void print_InvSBOX(){

}
uint8_t SBOX[16][16] = {
    { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 },
	{ 0xCa, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 },
	{ 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 },
	{ 0x04,	0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 },
	{ 0x09,	0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 },
	{ 0x53,	0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf },
	{ 0xd0,	0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 },
	{ 0x51,	0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 },
	{ 0xcd,	0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 },
	{ 0x60,	0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb },
	{ 0xe0,	0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 },
	{ 0xe7,	0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 },
	{ 0xba,	0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a },
	{ 0x70,	0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e },
	{ 0xe1,	0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e,	0x94, 0x9b,	0x1e, 0x87,	0xe9, 0xce,	0x55, 0x28,	0xdf },
	{ 0x8c, 0xa1, 0x89, 0x0d, 0xbf,	0xe6, 0x42,	0x68, 0x41,	0x99, 0x2d,	0x0f, 0xb0,	0x54, 0xbb,	0x16 }
};

uint8_t InvSBOX[16][16] = {
    { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb },
    { 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb },
    { 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e },
    { 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25 },
    { 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92 },
    { 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84 },
    { 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06 },
    { 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b },
    { 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73 },
    { 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e },
    { 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b },
    { 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4 },
    { 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f },
    { 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef },
    { 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61 },
    { 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d }
};

void SubBytes(uint8_t state[][4], int inv) {
    uint8_t temp;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++){
            temp = state[i][j];
            if (inv == 0)
                state[i][j] = SBOX[(temp & 0xF0) >> 4][temp & 0x0F];
            else
                state[i][j] = InvSBOX[(temp & 0xF0) >> 4][temp & 0x0F];
        }
            
}
void Swap_elements(uint8_t state[][4], int i1, int j1, int i2, int j2){ // Swaps two elements in an array
    uint8_t temp;
    temp = state[i1][j1];
    state[i1][j1] = state[i2][j2];
    state[i2][j2] = temp;
}
void ShiftRows(uint8_t state[][4], int inv) {
    
    if(inv == 0){
        //2nd row goal                       1 2 3 0
        Swap_elements(state, 1, 0, 1, 1); // 1 0 2 3
        Swap_elements(state, 1, 1, 1, 2); // 1 2 0 3
        Swap_elements(state, 1, 2, 1, 3); // 1 2 3 0

        //3rd row goal                       2 3 0 1
        Swap_elements(state, 2, 0, 2, 1); // 1 0 2 3
        Swap_elements(state, 2, 1, 2, 2); // 1 2 0 3
        Swap_elements(state, 2, 0, 2, 3); // 3 2 0 1
        Swap_elements(state, 2, 0, 2, 1); // 2 3 0 1

        //4th row goal                       3 0 1 2
        Swap_elements(state, 3, 3, 3, 2); // 0 1 3 2
        Swap_elements(state, 3, 2, 3, 1); // 0 3 1 2
        Swap_elements(state, 3, 1, 3, 0); // 3 0 1 2
    }

    else {
        //2th row goal                       3 0 1 2
        Swap_elements(state, 1, 3, 1, 2); // 0 1 3 2
        Swap_elements(state, 1, 2, 1, 1); // 0 3 1 2
        Swap_elements(state, 1, 1, 1, 0); // 3 0 1 2

        //3rd row goal                       2 3 0 1
        Swap_elements(state, 2, 0, 2, 1); // 1 0 2 3
        Swap_elements(state, 2, 1, 2, 2); // 1 2 0 3
        Swap_elements(state, 2, 0, 2, 3); // 3 2 0 1
        Swap_elements(state, 2, 0, 2, 1); // 2 3 0 1

        //2nd row goal                       1 2 3 0
        Swap_elements(state, 3, 0, 3, 1); // 1 0 2 3
        Swap_elements(state, 3, 1, 3, 2); // 1 2 0 3
        Swap_elements(state, 3, 2, 3, 3); // 1 2 3 0
    }
    

}

uint8_t galiousMul(uint8_t a, uint8_t b) { // performs a multipicaiton in galious field
    //copied verbatium from: https://blog.0x7d0.dev/education/how-aes-is-implemented/
    uint8_t result = 0; 

    while (b != 0) 
    {
        if (b & 1) 
            result ^= a;

        if (a & 0x80)
            a = (a << 1) ^ 0x11b; 
        else
            a <<= 1; 

        b >>= 1;
    }

    return result;
    
}

void MixColumns(uint8_t state[][4], int inv) {
    uint8_t temp_state[4][4];

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            temp_state[i][j] = state[i][j];
        

    if(inv == 0)
        for(int j = 0; j < 4; j++){ // iterates through columns
            state[0][j] = ((galiousMul(0x02, temp_state[0][j])) ^ (galiousMul(0x03, temp_state[1][j]))) ^ ((galiousMul(0x01, temp_state[2][j])) ^ (galiousMul(0x01, temp_state[3][j])));
            state[1][j] = ((galiousMul(0x01, temp_state[0][j])) ^ (galiousMul(0x02, temp_state[1][j]))) ^ ((galiousMul(0x03, temp_state[2][j])) ^ (galiousMul(0x01, temp_state[3][j])));
            state[2][j] = ((galiousMul(0x01, temp_state[0][j])) ^ (galiousMul(0x01, temp_state[1][j]))) ^ ((galiousMul(0x02, temp_state[2][j])) ^ (galiousMul(0x03, temp_state[3][j])));
            state[3][j] = ((galiousMul(0x03, temp_state[0][j])) ^ (galiousMul(0x01, temp_state[1][j]))) ^ ((galiousMul(0x01, temp_state[2][j])) ^ (galiousMul(0x02, temp_state[3][j])));
        }
    else {
        for(int j = 0; j < 4; j++){ // iterates through columns
            state[0][j] = ((galiousMul(0x0E, temp_state[0][j])) ^ (galiousMul(0x0B, temp_state[1][j]))) ^ ((galiousMul(0x0D, temp_state[2][j])) ^ (galiousMul(0x09, temp_state[3][j])));
            state[1][j] = ((galiousMul(0x09, temp_state[0][j])) ^ (galiousMul(0x0E, temp_state[1][j]))) ^ ((galiousMul(0x0B, temp_state[2][j])) ^ (galiousMul(0x0D, temp_state[3][j])));
            state[2][j] = ((galiousMul(0x0D, temp_state[0][j])) ^ (galiousMul(0x09, temp_state[1][j]))) ^ ((galiousMul(0x0E, temp_state[2][j])) ^ (galiousMul(0x0B, temp_state[3][j])));
            state[3][j] = ((galiousMul(0x0B, temp_state[0][j])) ^ (galiousMul(0x0D, temp_state[1][j]))) ^ ((galiousMul(0x09, temp_state[2][j])) ^ (galiousMul(0x0E, temp_state[3][j])));
        }
    }
}

void set_initial(uint8_t state[], uint8_t RC, int inv){
    // 1 byte left circular rotation --> move the bytes like so 0 1 2 3 >> 1 2 3 0
    // COULD BE INVERTED FOR DECRYPT ??? so we would need 0 1 2 3 --> 3 0 1 2
    uint8_t temp;
    if(1){
        temp = state[3]; state[3] = state[0]; state[0] = temp; // 3 1 2 0
        temp = state[1]; state[1] = state[0]; state[0] = temp; // 1 3 2 0
        temp = state[2]; state[2] = state[1]; state[1] = temp; // 1 2 3 0
    }
    else { //?????
        temp = state[3]; state[3] = state[0]; state[0] = temp; // 3 1 2 0
        temp = state[3]; state[3] = state[2]; state[2] = temp; // 3 1 0 2
        temp = state[2]; state[2] = state[1]; state[1] = temp; // 3 0 1 2
    }
    

    //perform a substitution
    for(int i = 0; i < 4; i++){
        temp = state[i];
        if(inv == 0)
            state[i] = SBOX[(temp & 0xF0) >> 4][temp & 0x0F];
        else
            state[i] = InvSBOX[(temp & 0xF0) >> 4][temp & 0x0F];
    }
    
    state[0] ^= RC; // XOR the first byte with the RC
    state[1] ^= 0x00;
    state[2] ^= 0x00;
    state[3] ^= 0x00;

}

void Key_Expansion(uint8_t key[][4][4], int inv) { // with 11 rounds lol and it should be already initalized with the first key
    //be careful for key having been initalized with junk data
    //printf("\nSTART DEBUG KEY EXPANSION\n");

    int r, i, j;
    int NUM_ROUNDS = 11;

    uint8_t RC = 0x01; //remember lower 4 bytes are 0 lol
    uint8_t RC_iter = 0x02;
    uint8_t temp[4]; // refers to w_i+4 and is used in calculating g(x)
    for(i = 0; i < 4; i++)
        temp[i] = key[0][i][3]; // make it the 4th column

    for (r = 1; r < NUM_ROUNDS; r++){
        
        if (r > 1){
            RC = galiousMul(RC_iter, RC); // find current round constant --> CHECKED
        }
        set_initial(temp, RC, inv); // setting the first column in temp
    
        for(i = 0; i < 4; i++)
            key[r][i][0] = key[r-1][i][0] ^ temp[i]; // first col XOR inital temp
        
        for (j = 1; j < 4; j++){ // sum over rows, iterate over columns 2-4
            for( i = 0; i < 4; i++)
                key[r][i][j] = key[r-1][i][j] ^ key[r][i][j-1];
        }
    
        for(i = 0; i < 4; i++) // setup inital temp for next round
            temp[i] = key[r][i][3]; // make it the 4th column
    }
}

// another way to do it is to precompute the key
void AddRoundKey(uint8_t state[][4], uint8_t key[][4][4], int round) {
    
    // apply the key XOR
    for(int j = 0; j < 4; j++)
        for(int i = 0; i < 4; i++)
            state[i][j] ^= key[round][i][j];

}

//~~~~~~~~~~AES HELPER FUNCTIONS AND LOOKUP TABLES ABOVE~~~~~~~~~~


//aka decrypt AES 128 bit in ECB mode
//the input is all byte arrays. functions in here should never take characters. 
//cipher text will return a string with the null character for printing in the main function
void solve_s1c7(uint8_t* ciphertext, char* key, uint8_t* plaintext, int c_len, int k_len, int p_len){
    //decryption steps:
    // 1. Inverse Shift Rows, 2. Inverse Sub Bytes, 3. Add round key, 4. Inverse mix columns
    int i; // iterator
    int j; // iterator
    int inv = 1;

    printf("\nSTART AES DECRYPT\n");

    if (c_len % 16)
        printf("You need padding\n");
    
    //we are just gonna get the first 16 bytes.
    uint8_t state_array[4][4] = {0x00};
    uint8_t key_array[11][4][4] = {0x00}; // key[round][row][col], 11 rounds for AES 128
    
    int k = 0; // k for kounter
    //char* test_key = "hello00000000000";
    for(j=0; j < 4; j++) // grab first 4 bytes of cipher text
        for(i=0; i < 4; i++) {
            key_array[0][i][j] = key[k]; // fill up the first round of the key
            k++;
        }

    Key_Expansion(key_array, 0); // idk why inverse didn't work but this is fine :)
    
    //decrypts 1 block... is
    int r;
    int p_i = 0;
    for(int block = 0; block < c_len / 16 - 1; block++){ // had to do it one less
        for(j = 0; j < 4; j++) // assign current block
            for(i = 0; i < 4; i++){
                state_array[i][j] = ciphertext[k];
                k++;
            }
        
        //decrypt block...key should be the same given the key expansion

        AddRoundKey(state_array, key_array, 10); // last byte 
        for(r = 9; r > 0; r--){
            ShiftRows(state_array, inv);
            SubBytes(state_array, inv);
            AddRoundKey(state_array,key_array,r);
            MixColumns(state_array, inv);
        }
        ShiftRows(state_array, inv);
        SubBytes(state_array, inv);
        AddRoundKey(state_array,key_array,0);

        //assign plaintext to 
        for(j = 0; j < 4; j++)
            for(i = 0; i < 4; i++){
                //printf("p_i = %d\n", p_i);
                plaintext[p_i] = (char)state_array[i][j];
                p_i++;
            }
               
    }
   

    /*
    AddRoundKey(state_array,key_array,10);

    for(int r = 9; r > 0; r--){
        ShiftRows(state_array, inv);
        SubBytes(state_array, inv);
        AddRoundKey(state_array,key_array,r);
        MixColumns(state_array, inv);
    }
    ShiftRows(state_array, inv);
    SubBytes(state_array, inv);
    AddRoundKey(state_array,key_array,0);
    */
   //MixColumns(state_array, inv);


    //print_SBOX();
    //print_InvSBOX();
    printf("\nENDING AES DECRYPT\n");

}

int solve_s1c8(uint8_t line[][160], int num_lines){
    int line_num, i, j, k; // i = 16byte hold pointer, j is the iterator pointer, k is the byte ptr
    int line_len = 160;
    uint8_t temp [16]; // 16 byte array

    for(line_num = 0; line_num < num_lines; line_num++) // for all lines
        for(i = 0; i < line_len; i+=16){ // select a 16 byte chunk
            for(k = 0; k < 16; k++) // allocate temp with 16 byte chunk
                temp[k] = line[line_num][i + k];
            for(j = i + 16; j < line_len; j += 16) { // check rest of lines
                for(k = 0; k < 16; k++) // check match
                    if(temp[k] != line[line_num][j + k]) // not equal
                        break; // go next
                    else if (k == 15)
                    {
                        printf("Line %d is likely encoded with AES ECB\n", line_num);
                        return line_num;
                    }
                    
                
        }
    }
    printf("Failed to find a line encoded with AES ECB\n");
    return -1; 
}