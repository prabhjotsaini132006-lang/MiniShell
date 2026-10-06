#include <stdio.h>

#include "clear.h"

void run_clear(void)
{
    printf("\033[2J\033[H");
    fflush(stdout);
}