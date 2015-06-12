#include <stdio.h>

int main()
{
    float a = 9.625;
    printf("%d\n", *(int*)&a);
    return 0;
}
