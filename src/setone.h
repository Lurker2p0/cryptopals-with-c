
#include <stdint.h>
#include <string.h>

//main functions

//takes a 6-bit hex value, returns a char base64 value, -1 if fails
char bas64_int(uint8_t cipher);


//Helper functions

//takes a char, returns a hex value, -1 if fails
uint8_t char_to_hex(char input);

//takes a char poiter input and a char output pointer, the length refers to the input
void solve_s1c1(char* input, char* output, int length);

void solve_s1c2(char* buf1, char* buf2, char* ans, int len);
