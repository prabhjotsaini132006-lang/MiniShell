#include <stdio.h>
#include <sys/stat.h>

#include "mkdir.h"

void run_mkdir(const char *dirname)
{
    if (dirname == NULL)
    {
        fprintf(stderr, "mkdir: missing operand\n");
        return;
    }

    if (mkdir(dirname, 0755) == -1)
    {
        perror("mkdir");
    }
}