#include <stdio.h>
#include <ctype.h>

#include "wc.h"

void run_wc(const char *filename)
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
            perror("wc");
            return;
        }
    }

    int character;
    long lines = 0;
    long words = 0;
    long bytes = 0;
    int in_word = 0;

    while ((character = fgetc(file)) != EOF)
    {
        bytes++;

        if (character == '\n')
        {
            lines++;
        }

        if (isspace((unsigned char)character))
        {
            in_word = 0;
        }
        else if (!in_word)
        {
            words++;
            in_word = 1;
        }
    }

    if (ferror(file))
    {
        perror("wc");
    }
    else
    {
        printf("%ld %ld %ld", lines, words, bytes);

        if (filename != NULL)
        {
            printf(" %s", filename);
        }

        printf("\n");
    }

    if (file != stdin)
    {
        fclose(file);
    }
}