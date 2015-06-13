#include <stdio.h>
#include <sys/vfs.h>

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