#include <stdio.h>

#include "mv.h"

void run_mv(const char *source, const char *destination)
{
    if (source == NULL || destination == NULL)
    {
        fprintf(stderr, "mv: missing operand\n");
        return;
    }

    if (rename(source, destination) == -1)
    {
        perror("mv");
    }
}