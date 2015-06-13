#include "test.h"
#include <stdio.h>


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
    // int a[] = {5,3,4,1,2};
    // auto c = 0;
    // std::sort(a,a+5,[&c](int a,int b){c++;return a>b;});
    // for (int i=0;i<5;i++)
    //     printf("%d \n", a[i]);
    // printf("%d\n", c);
}

#endif


int main()
{
    BEGIN_TEST

    TEST(c_base_test);
    TEST(renamefile_test);

#ifdef __cplusplus
    TEST(xujilei_test);
    TEST(lambda_test);
#endif
    TEST(crash_test);
    TEST(free_disk_test);
    TEST(rc4_test);
    TEST(test_hub);

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