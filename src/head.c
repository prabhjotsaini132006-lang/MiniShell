#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "head.h"

#define HEAD_LINES 10

void run_head(const char *filename)
{
    FILE *file;

    if (filename == NULL)
    {
        file = stdin;
    }
    else
    {
        file = fopen(filename, "r");

        if (file == NULL)
        {
            perror("head");
            return;
        }
    }

    char *line = NULL;
    size_t size = 0;

    for (int i = 0; i < HEAD_LINES; i++)
    {
        if (getline(&line, &size, file) == -1)
        {
            break;
        }

        printf("%s", line);
    }

    free(line);

    if (file != stdin)
    {
        fclose(file);
    }
}