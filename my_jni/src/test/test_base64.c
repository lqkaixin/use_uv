#include <stdio.h>
#include <string.h>

#include "base64.h"

int test_base64()
{ 
    const char *t = "那个abcd你好吗，哈哈，ANMOL"; 
    printf("\noriginal: %s\n", t); 
    int j = strlen(t); 
    char p[512] = {0};
    char *enc = base64_encode(t, j, p, 512); 
    printf("\nencoded : %s\n", enc); 
    int len = strlen(enc); 
    char *dec = base64_decode(enc, len, p, 512); 
    printf("\ndecoded : %s\n", dec); 
    return 0; 
}