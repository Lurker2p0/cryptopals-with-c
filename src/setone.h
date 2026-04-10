
#include <stdint.h>
#include <string.h>
#include <ctype.h>


//main functions

//takes a 6-bit hex value, returns a char base64 value, -1 if fails
char bas64_int(uint8_t cipher);

int hamming_distance(char* buf1, char* buf2, int len);

//Helper functions
void char_to_byte_array(char* input, char* output, int len);

//takes a char, returns a hex value, -1 if fails
uint8_t char_to_hex(char input);

//takes a char poiter input and a char output pointer, the length refers to the input
void solve_s1c1(char* input, char* output, int length);

void solve_s1c2(char* buf1, char* buf2, char* ans, int len);


uint8_t solve_s1c3(char* input, char* output, int len);


void solve_s1c4(FILE *file, char* ans);

void solve_s1c5(char* input, char* output, char* key, int keylen,  int len);

void solve_s1c6(FILE *file, char* ans);

void base64_to_bytes(char* input, uint8_t* output, int len_i, int len_o);

void solve_s1c7(uint8_t* ciphertext, char* key, uint8_t* plaintext, int c_len, int k_len, int p_len);
