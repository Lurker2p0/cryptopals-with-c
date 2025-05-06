/*
This file will contain the helper functions that can solve the problems

 */

 #include <stdint.h>
 #include <string.h>

// takes an integer and returns a base64 char
char bas64_int(uint8_t cipher){
    //taken from https://github.com/To1ne/cryptopals-c
    switch ((cipher)){
        case 0 ... 25: return ('A' + cipher);
        case 26 ... 51: return ('a' + (cipher-26));
        case 52 ... 61: return ('1'+ (cipher-52));
        case 62: return '+';
        case 63: return '/';
        default:
            break;
    }
    return 0;
}