#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "background.h"
#include "commands.h"

static void execute_background_command(char *args[], int argc)
{
    if (argc == 0)
    {
        return;
    }

    if (strcmp(args[0], "echo") == 0)
    {
        run_echo(args, argc);
    }
    else if (strcmp(args[0], "cat") == 0)
    {
        run_cat(argc > 1 ? args[1] : NULL);
    }
    else if (strcmp(args[0], "head") == 0)
    {
        run_head(argc > 1 ? args[1] : NULL);
    }
    else if (strcmp(args[0], "tail") == 0)
    {
        run_tail(argc > 1 ? args[1] : NULL);
    }
    else if (strcmp(args[0], "wc") == 0)
    {
        run_wc(argc > 1 ? args[1] : NULL);
    }
    else if (strcmp(args[0], "pwd") == 0)
    {
        run_pwd();
    }
    else
    {
        fprintf(
            stderr,
            "minishell: background command not implemented: %s\n",
            args[0]
        );
    }
}

int run_background(char *args[], int argc)
{
    if (argc == 0)
    {
        return 0;
    }

    if (strcmp(args[argc - 1], "&") != 0)
    {
        return 0;
    }

    args[argc - 1] = NULL;
    argc--;

    if (argc == 0)
    {
        fprintf(stderr, "minishell: invalid background command\n");
        return -1;
    }

    pid_t child = fork();

    if (child == -1)
    {
        perror("fork");
        return -1;
    }

    if (child == 0)
    {
        execute_background_command(args, argc);
        _exit(0);
    }

    printf("[background process %d]\n", child);

    return 1;
}