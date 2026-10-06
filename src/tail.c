#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tail.h"

#define TAIL_LINES 10

void run_tail(const char *filename)
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
            perror("tail");
            return;
        }
    }

    char **lines = malloc(TAIL_LINES * sizeof(char *));

    if (lines == NULL)
    {
        perror("tail");
        
        if (file != stdin)
        {
            fclose(file);
        }

        return;
    }

    for (int i = 0; i < TAIL_LINES; i++)
    {
        lines[i] = NULL;
    }

    int count = 0;
    char *line = NULL;
    size_t size = 0;

    while (getline(&line, &size, file) != -1)
    {
        free(lines[count]);

        lines[count] = malloc(strlen(line) + 1);

        if (lines[count] == NULL)
        {
            perror("tail");

            free(line);

            for (int i = 0; i < TAIL_LINES; i++)
            {
                free(lines[i]);
            }

            free(lines);

            if (file != stdin)
            {
                fclose(file);
            }

            return;
        }

        strcpy(lines[count], line);

        count = (count + 1) % TAIL_LINES;
    }

    free(line);

    int start;

    if (count == 0)
    {
        start = 0;
    }
    else if (count < TAIL_LINES)
    {
        start = 0;
    }
    else
    {
        start = count;
    }

    int total_lines;

    if (count < TAIL_LINES)
    {
        total_lines = count;
    }
    else
    {
        total_lines = TAIL_LINES;
    }

    for (int i = 0; i < total_lines; i++)
    {
        int index = (start + i) % TAIL_LINES;

        if (lines[index] != NULL)
        {
            printf("%s", lines[index]);
        }
    }

    for (int i = 0; i < TAIL_LINES; i++)
    {
        free(lines[i]);
    }

    free(lines);

    if (file != stdin)
    {
        fclose(file);
    }
}