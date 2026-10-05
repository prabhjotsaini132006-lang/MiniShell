#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history.h"

#define MAX_HISTORY 100

static char *history[MAX_HISTORY];
static int history_count = 0;

void add_history_command(const char *command)
{
    if (command == NULL || strlen(command) == 0)
    {
        return;
    }

    if (history_count == MAX_HISTORY)
    {
        free(history[0]);

        for (int i = 1; i < MAX_HISTORY; i++)
        {
            history[i - 1] = history[i];
        }

        history_count--;
    }

    history[history_count] = malloc(strlen(command) + 1);

    if (history[history_count] == NULL)
    {
        perror("history");
        return;
    }

    strcpy(history[history_count], command);
    history_count++;
}

void show_history(void)
{
    for (int i = 0; i < history_count; i++)
    {
        printf("%d  %s\n", i + 1, history[i]);
    }
}

void free_history(void)
{
    for (int i = 0; i < history_count; i++)
    {
        free(history[i]);
        history[i] = NULL;
    }

    history_count = 0;
}