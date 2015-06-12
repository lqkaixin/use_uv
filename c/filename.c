#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argv, char *argc)
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
    return(0);
}
