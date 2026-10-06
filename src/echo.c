#include <stdio.h>

#include "echo.h"

void run_echo(char *args[], int argc)
{
    for (int i = 1; i < argc; i++)
    {
        printf("%s", args[i]);

        if (i < argc - 1)
        {
            printf(" ");
        }
    }

    printf("\n");
}