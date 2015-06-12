#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/vfs.h> /* 或者 <sys/statfs.h> */



#define BEGIN_TEST                      \
int idx = 0, tmp = 0;                   \
while(1)   \
{                                       \
    tmp = 0;                            \
    printf("\033[1;33mPlease Input Test Idx(0查看, -1退出): \033[0m");  \
    if(scanf("%d", &idx) != 1 || idx == -1) break;

#define TEST(A) if (idx == ++tmp) A(); if(idx == 0) printf("%d: "#A"\n", tmp);

#define END_TEST }

#define LOG_DEBUG(fmt, ...) printf(fmt "\n", __VA_ARGS__)

#ifndef N_A
#include "common/base64.h"
#include "common/md5.h"
// #include <utils/CallStack.h>
int de_base64_test()
{
    char s[10] = {0},d[10] = {0};
    int len ;


    {
        _u8 calc_keya[16];
        _u8 keya[33] = {0};
        ctx_md5 md5;
        md5_initialize(&md5);
        md5_update(&md5, (const unsigned char*)"xl_kk785236", sd_strlen("xl_kk785236"));
        md5_update(&md5, (const unsigned char*)"1e4f2efa86cda5d762eaeacb02f162e56f80103c", sd_strlen("1e4f2efa86cda5d762eaeacb02f162e56f80103c"));
        md5_update(&md5, (const unsigned char*)"01310711396417996715", sd_strlen("01310711396417996715"));
        md5_update(&md5, (const unsigned char*)"RXV02SDY-ZLNR-V6EI-8BNM-T6TD2KIBFNRW", sd_strlen("RXV02SDY-ZLNR-V6EI-8BNM-T6TD2KIBFNRW"));
        md5_finish(&md5, calc_keya);
        str2hex((const char *)calc_keya, 16, keya, 33);
        printf("%s\n", keya);
    }
    {
        _u8 calc_keya[16];
        _u8 keya[33] = {0};
        ctx_md5 md5;
        md5_initialize(&md5);
        md5_update(&md5, (const unsigned char*)"xl_kk7852361e4f2efa86cda5d762eaeacb02f162e56f80103c01310711396417996715RXV02SDY-ZLNR-V6EI-8BNM-T6TD2KIBFNRW", sd_strlen("xl_kk7852361e4f2efa86cda5d762eaeacb02f162e56f80103c01310711396417996715RXV02SDY-ZLNR-V6EI-8BNM-T6TD2KIBFNRW"));
        md5_finish(&md5, calc_keya);
        str2hex((const char *)calc_keya, 16, keya, 33);
        printf("%s\n", keya);
    }

    while (scanf("%s", s)!=EOF)
    {
        sd_base64_encode((_u8*)s, strlen(s), d);

        sd_base64_decode(d, (_u8*)s, &len);

        printf("encode result: %s-decode result: %s-%d\n", d,s,len);

    }
    return 0;
}

void de_crc16_test()
{
    int a;
    while(scanf("%d\n", &a) == 1)
    {
    printf("crc16 of 0x%x: %d", a, sd_get_crc16((void *)&a, 1));
    }

}

#endif

int free_disk_test()
{
    char path[1024] = {0};
    while (scanf("%s", path) == 1)
    {
    struct statfs fs;
    statfs(path, &fs);
    printf("%u, %u\n", (unsigned)fs.f_bavail, (unsigned)fs.f_bsize);
    }

    return 0;
}

#ifdef __cplusplus

#include <algorithm>

class A
{
public:
    A(){printf("no data!\n");}
    A(int a){ printf("%d\n", a);}
    virtual void fun(){}
};
class B:virtual public A
{
public:
    B():A(1){}
    virtual void gun(){}
};
class C:virtual public A
{
public:
    C():A(2){}
    virtual void hun(){}
};
class D:public B, C
{
public:
    D():B(),C(){}
    D(int a):A(a),B(),C(){}
    virtual void iun(){}
};
class B1:public A
{
public:
    B1():A(1){}
    virtual void gun(){}
};
class C1:public A
{
public:
    C1():A(2){}
    virtual void hun(){}
};
class D1:public B1, C1
{
public:
    D1():B1(),C1(){}
    D1(int a):B1(),C1(){}
    virtual void iun(){}
};

int xujilei_test()
{
    D d;
    D dd(3);
    printf("%d\n", (int)sizeof(D));
    printf("%d\n", (int)sizeof(D1));
    //printf(&unix["\021%six\012\0"], (unix)["have"] + "fun" - 0x60);
    return 0;
}


int lambda_test()
{
    int a[] = {5,3,4,1,2};
    auto c = 0;
    std::sort(a,a+5,[&c](int a,int b){c++;return a>b;});
    for (int i=0;i<5;i++)
        printf("%d \n", a[i]);
    printf("%d\n", c);
}

#endif


int renamefile_test()
{
    char newName[256]={'\0'};
    char oldName[256]={'\0'};
    
    printf("Input the rename file:\n");
    scanf("%s", oldName);
    printf("Input the renamed filename:\n");
    scanf("%s", newName);

    int f = open(oldName, O_RDWR|O_APPEND);
    if(f <= 0)
    {
        printf("open() error\n");
    }
    int j=rename(oldName, newName);
    if(j != 0)
    {
        printf("rename() error\n");
    }
    printf("Program End\n");
    write(f,"abc\n", 4);
    getchar();
    return 0;
}

int sign_num_cmp_mov_test()
{
    short a = -1;
    int b = 0;
    unsigned c = 0;
    unsigned short d = 0;
    printf("(0 < short(-1)): %d\n", (0<a));
    printf("(int(0) < short(-1)): %d\n", (b<a));
    printf("(unsigned(0) < short(-1)): %d\n", (c<a));
    printf("(unsigned short(0) < short(-1)): %d\n", (d<a));

    printf("123<<32: %d\n", (123<<32));
    printf("123>>33: %d\n", (123>>33));

    printf("(unsigned(short(-1))): %u\n", (unsigned)a);
    return 0;
}

int ff(int a, int b)
{
    //getCallStack();
    *(int*)0 = 4;
    return a+b;
}

int gg(int a, int b)
{
    return ff(0x12000000,0x34000000);
}

int hh(int a, int b)
{
    return gg(0x56000000, 0x78000000);
}

int crash_test()
{
    hh(0xab000000,0xcd000000);
    // CallStack stack;
    return 0;
}




//程序开始
#include<stdio.h>
#include<string.h>
 
typedef unsigned long ULONG;
 
/*初始化函数*/
void rc4_init(unsigned char*s,unsigned char*key,unsigned long Len)
{
    int i=0,j=0;
    char k[256]={0};
    unsigned char tmp=0;
    for(i=0;i<256;i++)
    {
        s[i]=i;
        k[i]=key[i%Len];
    }
    for(i=0;i<256;i++)
    {
        j=(j+s[i]+k[i])%256;
        tmp=s[i];
        s[i]=s[j];//交换s[i]和s[j]
        s[j]=tmp;
    }
}
 
/*加解密*/
void rc4_crypt(unsigned char*s,unsigned char*Data,unsigned long Len)
{
    int i=0,j=0,t=0;
    unsigned long k=0;
    unsigned char tmp;
    for(k=0;k<Len;k++)
    {
        i=(i+1)%256;
        j=(j+s[i])%256;
        tmp=s[i];
        s[i]=s[j];//交换s[x]和s[y]
        s[j]=tmp;
        t=(s[i]+s[j])%256;
        Data[k]^=s[t];
    }
}
 
int rc4_test()
{
    unsigned char s[256]={0},s2[256]={0};//S-box
    char key[256]={"justfortest"};
    char pData[512]="这是一个用来加密的数据Data";
    ULONG len=strlen(pData);
    int i;
     
    printf("pData=%s\n",pData);
    printf("key=%s,length=%d\n\n",key,(int)strlen(key));
    rc4_init(s,(unsigned char*)key,strlen(key));//已经完成了初始化
    printf("完成对S[i]的初始化，如下：\n\n");
    for(i=0;i<256;i++)
    {
        printf("%02X",s[i]);
        if(i&&(i+1)%16==0)putchar('\n');
    }
    printf("\n\n");
    for(i=0;i<256;i++)//用s2[i]暂时保留经过初始化的s[i]，很重要的！！！
    {
        s2[i]=s[i];
    }
    printf("已经初始化，现在加密:\n\n");
    rc4_crypt(s,(unsigned char*)pData,len);//加密
    printf("pData=%s\n\n",pData);
    printf("已经加密，现在解密:\n\n");
    rc4_init(s2,(unsigned char*)key,strlen(key));//初始化密钥
    rc4_crypt(s2,(unsigned char*)pData,len);//解密
    printf("pData=%s\n\n",pData);
    return 0;
}
 
//程序完




int main()
{
    BEGIN_TEST

    TEST(sign_num_cmp_mov_test);
    TEST(renamefile_test);
#ifndef N_A
    TEST(de_base64_test);
    TEST(de_crc16_test);
#endif

#ifdef __cplusplus
    TEST(xujilei_test);
    TEST(lambda_test);
#endif
    TEST(crash_test);
    TEST(free_disk_test);
    TEST(rc4_test);

    END_TEST
    return 0;
}



// #define MAX_DEPTH 31
// #define MAX_BACKTRACE_LINE_LENGTH 800
// #define PATH "/system/lib/libcorkscrew.so"

// typedef ssize_t (*unwindFn)(backtrace_frame_t*, size_t, size_t);
// typedef void (*unwindSymbFn)(const backtrace_frame_t*, size_t, backtrace_symbol_t*);
// typedef void (*unwindSymbFreeFn)(backtrace_symbol_t*, size_t);

// static void *gHandle = NULL;

// static int getCallStack(void){
//     ssize_t i = 0;
//     ssize_t result = 0;
//     ssize_t count;
//     backtrace_frame_t mStack[MAX_DEPTH];
//     backtrace_symbol_t symbols[MAX_DEPTH];

//     unwindFn unwind_backtrace = NULL;
//     unwindSymbFn get_backtrace_symbols = NULL;
//     unwindSymbFreeFn free_backtrace_symbols = NULL;

//     // open the so.
//     if(gHandle == NULL) gHandle = dlopen(PATH, RTLD_NOW);

//     // get the interface for unwind and symbol analyse
//     if(gHandle != NULL) unwind_backtrace = (unwindFn)dlsym(gHandle, "unwind_backtrace");
//     if(gHandle != NULL) get_backtrace_symbols = (unwindSymbFn)dlsym(gHandle, "get_backtrace_symbols");
//     if(gHandle != NULL) free_backtrace_symbols = (unwindSymbFreeFn)dlsym(gHandle, "free_backtrace_symbols");

//     if(!gHandle ||!unwind_backtrace ||!get_backtrace_symbols || !free_backtrace_symbols ){
//         ALOGE("Error! cannot get unwind info: handle:%p %p %p %p",
//         gHandle, unwind_backtrace, get_backtrace_symbols, free_backtrace_symbols );
//         return result;
//     }

//     count= unwind_backtrace(mStack, 1, MAX_DEPTH);
//     get_backtrace_symbols(mStack, count, symbols);

//     for (i = 0; i < count; i++) {
//         char line[MAX_BACKTRACE_LINE_LENGTH];

//         const char* mapName = symbols[i].map_name ? symbols[i].map_name : "<unknown>";
//         const char* symbolName =symbols[i].demangled_name ? symbols[i].demangled_name : symbols[i].symbol_name;
//         size_t fieldWidth = (MAX_BACKTRACE_LINE_LENGTH - 80) / 2;

//         if (symbolName) {
//             uint32_t pc_offset = symbols[i].relative_pc - symbols[i].relative_symbol_addr;
//             if (pc_offset) {
//                 snprintf(line, MAX_BACKTRACE_LINE_LENGTH, "#%02d pc %08x %.*s (%.*s+%u)",
//                 i, symbols[i].relative_pc, fieldWidth, mapName,
//                 fieldWidth, symbolName, pc_offset);
//             } else {
//             snprintf(line, MAX_BACKTRACE_LINE_LENGTH, "#%02d pc %08x %.*s (%.*s)",
//                 i, symbols[i].relative_pc, fieldWidth, mapName,
//                 fieldWidth, symbolName);
//             }
//         } else {
//             snprintf(line, MAX_BACKTRACE_LINE_LENGTH, "#%02d pc %08x %.*s",
//                 i, symbols[i].relative_pc, fieldWidth, mapName);
//         }

//         ALOGD("%s", line);
//     }

//     free_backtrace_symbols(symbols, count);

//     return result;
// }



//#include <corkscrew/backtrace.h> 

// void dumpStackTrace(const char* logtag, int32_t ignoreDepth, int32_t maxDepth)
// {
//     static const int MAX_DEPTH = 31;
//     static const int MAX_BACKTRACE_LINE_LENGTH = 800;

//     if (maxDepth > MAX_DEPTH) {
//         maxDepth = MAX_DEPTH;
//     }
//     backtrace_frame_t mStack[MAX_DEPTH];
//     ssize_t count = unwind_backtrace(mStack, ignoreDepth + 1, maxDepth);
//     if (count <= 0) {
//         LOGE("Can not get stack trace");
//         return;
//     }

//     backtrace_symbol_t symbols[count];

//     get_backtrace_symbols(mStack, count, symbols);
//     int i;
//     for (i = 0; i < count; i++) {
//         char line[MAX_BACKTRACE_LINE_LENGTH];
//         format_backtrace_line(i, &mStack[i], &symbols[i], line, MAX_BACKTRACE_LINE_LENGTH);
//         LOG_DEBUG("%s%s", "", line);
//     }
//     free_backtrace_symbols(symbols, count);
// }

// void foobar() {
//     dumpStackTrace("Trace", 1, 30);
// }