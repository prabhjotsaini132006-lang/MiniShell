#include <stdio.h>
#include <dirent.h>

#include "ls.h"

void run_ls(const char *path)
{
    const char *directory = path;

    if (directory == NULL)
    {
        directory = ".";
    }

    DIR *dir = opendir(directory);

    if (dir == NULL)
    {
        perror("ls");
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        printf("%s  ", entry->d_name);
    }

    printf("\n");

    closedir(dir);
}