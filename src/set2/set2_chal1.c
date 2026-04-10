
 #include <stdint.h>
 #include <string.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <ctype.h>


// implements padding PKCS#7 where you padd up to a number of bytes
// and the bytes added are the number of paddings needed. i suppose you can't
// padd more than 32 bytes 0xff.
void add_padding(uint8_t* input, uint8_t* output, int in_len, int out_len) {
    int i;
    int bytes_needed = out_len - in_len;
    if (bytes_needed < 0){
        printf("output length smaller than input length\n");
        return;
    }
    else if (bytes_needed == 0) {
        printf("no padding needed\n");
        return;
    }

    bytes_needed = (uint8_t)bytes_needed; // cast to what i need

    for (i = 0; i < out_len; i++){
        if (i < in_len)
            output[i] = input[i];
        else
            output[i] = bytes_needed;
    }
}