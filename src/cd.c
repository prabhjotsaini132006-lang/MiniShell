#include <stdio.h>
#include <unistd.h>

#include "cd.h"

void run_cd(const char *path)
{
    if (path == NULL)
    {
        fprintf(stderr, "cd: missing operand\n");
        return;
    }

    if (chdir(path) == -1)
    {
        perror("cd");
    }
}