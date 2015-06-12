#include "base64.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
static char rebase[256];

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
/* */ 
char *base64_encode(const char* in, int in_len, char*out, int out_len)
{ 
    int ret_len = (in_len+2) / 3 * 4;
    int temp = in_len % 3; 
    int i = 0, j = 0;

    if ( out == NULL || out_len < ret_len)
    { 
        printf("Not enough memory.\n"); 
        return NULL; 
    } 
    while (i < in_len) 
    {
        unsigned merged = 0;
        merged |= (in[i++]&0xFF)<<16;
        if (i<in_len) merged |= (in[i++]&0xFF)<<8;
        if (i<in_len) merged |= (in[i++]&0xFF)<<0;

        out[j++] = base[(merged>>18)&0x3f];
        out[j++] = base[(merged>>12)&0x3f];
        out[j++] = base[(merged>> 6)&0x3f];
        out[j++] = base[(merged>> 0)&0x3f];
    } 

    printf("%d %d.\n", i, j); 

    if (temp++ == 1) out[--j] = base[64];
    if (temp++ == 2) out[--j] = base[64];
      
    return out; 
} 

char *base64_decode(const char* in, int in_len, char*out, int out_len)
{
    int ret_len = in_len/4*3;
    int i,j;

    if (in_len%4 || out==NULL || out_len < ret_len) return NULL;


    if (rebase['a'] == 0)
    {
        for (i=0;i<65;i++)
            rebase[base[i]] = i;
    }

    i = j = 0;
    while (i < in_len)
    {
        unsigned merged = 0;
        merged |= (rebase[in[i++]]&0x3F)<<18;
        merged |= (rebase[in[i++]]&0x3F)<<12;
        merged |= (rebase[in[i++]]&0x3F)<<6;
        merged |= (rebase[in[i++]]&0x3F)<<0;

        out[j++] = (merged>>16)&0xff;
        out[j++] = (merged>> 8)&0xff;
        out[j++] = (merged>> 0)&0xff;
    }

    return out; 
}