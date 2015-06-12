#include "utility.h"

#include <stdio.h>
#include <string.h>


int str2hex(char *buf, int len)
{
  int i = 0;
  for (i=len;i--;)
  {
    char c = buf[i]&0xf;
    if (c<10) buf[i*2+1] = '0'+c;
    else buf[i*2+1] = 'A'+c-10;

    c = (buf[i]>>4)&0xf;
    if (c<10) buf[i*2] = '0'+c;
    else buf[i*2] = 'A'+c-10;
  }                                                                
  buf[2*len] = '\0';
  return 2*len;
}

int hex2str(char *buf, int len)
{
  int i = 0;
  for (i=0;i<len;)
  {
    char c = 0;
    if (buf[i]>='0' && buf[i]<='9') c = (c<<4)+(buf[i]-'0');
    else if (buf[i]>='a' && buf[i]<='z') c = (c<<4)+(buf[i]-'a'+10);
    else if (buf[i]>='A' && buf[i]<='Z') c = (c<<4)+(buf[i]-'A'+10);
    i++;

    if (buf[i]>='0' && buf[i]<='9') c = (c<<4)+(buf[i]-'0');
    else if (buf[i]>='a' && buf[i]<='z') c = (c<<4)+(buf[i]-'a'+10);
    else if (buf[i]>='A' && buf[i]<='Z') c = (c<<4)+(buf[i]-'A'+10);
    i++;
    buf[i/2-1] = c;
    DEBUG("[hex2str] %c %c %d\n", buf[i-2], buf[i-1], c);
  }                                                                                                    
  buf[len/2] = '\0';
  return len/2;
}

char base64[65] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};

int base642str(char *buf, int len)
{
  int ret = len/4*3;
  if (buf[len-1]=='=')
    ret--;
  if (buf[len-2]=='=')
    ret--;

  int k = 0, c = 0;
  for(k = 0;k<len;k++)
  {
    c <<= 6;
    if (buf[k] >= 'A' && buf[k] <= 'Z')
      c += buf[k]-'A';
    if (buf[k] >= 'a' && buf[k] <= 'z')
      c += buf[k]-'a' + 26;                                                    
    if (buf[k] >= '0' && buf[k] <= '9')
      c += buf[k]-'0' + 52;
    if (buf[k]=='+')
      c += 62;
    if (buf[k]=='/')
      c +=63;
    if (buf[k]=='=')
      ;
    if (k%4==3)
    {
      buf[k/4*3] = c>>16;
      buf[k/4*3+1] = (c>>8)&0xff;

      buf[k/4*3+2] = c&0xff;
      c = 0;
    }
  }
  buf[ret] = '\0';

  return ret; 
}

int str2base64(char *buf, int len)
{
  int ret = (len+2)/3*4;

  int k = len, c = 0;
  while(k--)
  {
    c |= ((unsigned char)buf[k])<<(8*(3-k%3-1));
    if (k%3==0)
    {
      DEBUG("[str2base64] k: %d, c: %d\n", k, c);
      buf[k/3*4] = base64[c>>18];
      buf[k/3*4+1] = base64[(c>>12)&0x3f];
      buf[k/3*4+2] = base64[(c>>6)&0x3f];
      buf[k/3*4+3] = base64[(c)&0x3f];
      c = 0;
    }
  }
  if (len%3)
    buf[ret-1] = '=';
  if (len%3==1)
    buf[ret-2] = '=';
  buf[ret] = '\0';
  return ret; 
}

#ifdef DEBUG_UTILITY

void test_base64()
{
  char buff[1024];
 // while(1)
  {
    printf("[%s] input: ", __FUNCTION__);
    scanf("%s", buff);
    str2base64(buff, strlen(buff));
    printf("[%s] convert base64: %s\n", __FUNCTION__, buff);
    base642str(buff, strlen(buff));
    printf("[%s] convert back: %s\n\n", __FUNCTION__, buff);
  }
}

void test_str_hex()
{
  char buff[1024];
  //while(1)
  {
    printf("[%s] input: ", __FUNCTION__);
    scanf("%s", buff);
    str2hex(buff, strlen(buff));
    printf("[%s] convert hex: %s\n", __FUNCTION__, buff);
    hex2str(buff, strlen(buff));
    printf("[%s] convert back: %s\n\n", __FUNCTION__, buff);
  }
}

int main()
{
  test_base64();
  test_str_hex();

  return 0;
}

#endif

