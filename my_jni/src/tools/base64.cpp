#include "base64.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
static char rebase[256];

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

int str2hex(const char *in, int in_len, char *out, int out_len)
{
    if (out_len < in_len * 2) return -1;
    int i, t;
    for(i = 0; i < in_len; i++)
    {
        t = (in[i] >> 4)&0xf;
        if (t<10) out[(i<<1)] = '0' + t;
        else out[(i<<1)] = 'A' + t - 10;

        t = in[i]&0xf;
        if (t<10) out[(i<<1)+1] = '0' + t;
        else out[(i<<1)+1] = 'A' + t - 10;
    }
    return 0;
}
int hex2str(const char *in, int in_len, char *out, int out_len)
{
    if (in_len&1 || out_len < (in_len>>1)) return -1;
    int i;
    for (i = 0; i < in_len; ++i)
    {
        if (in[i] >= '0' && in[i] <= '9')
            out[i>>1] = (out[i>>1]<<4) + in[i] - '0';
        else if (in[i] >= 'a' && in[i] <= 'z')
            out[i>>1] = (out[i>>1]<<4) + in[i] - 'a' + 10;
        else if (in[i] >= 'A' && in[i] <= 'Z')
            out[i>>1] = (out[i>>1]<<4) + in[i] - 'A' + 10;
        else return -1;
    }
    return 0;
}