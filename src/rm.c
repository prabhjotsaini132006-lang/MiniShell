#include <stdio.h>
#include <unistd.h>

#include "rm.h"

void run_rm(const char *filename)
{
    if (filename == NULL)
    {
        fprintf(stderr, "rm: missing operand\n");
        return;
    }

    if (unlink(filename) == -1)
    {
        perror("rm");
    }
}