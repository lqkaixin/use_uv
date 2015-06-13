#include <stdio.h>

struct A
{
    double x;
    char c;
};


int c_base_test()
{
    short a = -1;
    int b = 0;
    unsigned c = 0;
    unsigned short d = 0;

    //类型转换
    printf("(0 < short(-1)): %d\n", (0<a));
    printf("(int(0) < short(-1)): %d\n", (b<a));
    printf("(unsigned(0) < short(-1)): %d\n", (c<a));
    printf("(unsigned short(0) < short(-1)): %d\n", (d<a));
    printf("(unsigned(short(-1))): %u\n", (unsigned)a);

    //移位
    printf("123<<32: %d\n", (123<<32));
    printf("123>>33: %d\n", (123>>33));

    //float内存布局
    float f = 9.625;
    printf("%d\n", *(int*)&f);

    //对齐
    printf("%lu\n", sizeof(struct A));

    return 0;
}