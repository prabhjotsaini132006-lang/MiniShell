#include <stdio.h>
#include <dirent.h>
#include "commands.h"

void run_ls(void)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");

    if (dir == NULL)
    {
        perror("ls");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}