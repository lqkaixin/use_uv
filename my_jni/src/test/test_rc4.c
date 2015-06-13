#include <stdio.h>
#include <string.h>

#include "rc4.h"

int rc4_test()
{
    rc4_ctx ctx;
    char key[256]={"justfortest"};
    char pData[512]="这是一个用来加密的数据Data";
    unsigned long len=strlen(pData);
    int i;
     
    printf("pData=%s\n",pData);
    printf("key=%s,length=%d\n\n",key,(int)strlen(key));
    rc4_init(&ctx, key,strlen(key));//已经完成了初始化
    printf("完成对S[i]的初始化，如下：\n\n");
    for(i=0;i<256;i++)
    {
        printf("%02X",ctx.S[i]);
        if(i&&(i+1)%16==0)putchar('\n');
    }
    printf("\n\n");
    printf("已经初始化，现在加密:\n\n");
    rc4_crypt(&ctx, pData, pData, len);//加密
    printf("pData=%s\n\n",pData);
    printf("已经加密，现在解密:\n\n");
    rc4_init(&ctx, key, strlen(key));//初始化密钥
    rc4_crypt(&ctx, pData, pData, len);//解密
    printf("pData=%s\n\n",pData);
    return 0;
}