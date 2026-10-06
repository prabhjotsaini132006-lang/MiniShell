#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "foreground.h"
#include "cat.h"

static void execute_foreground_command(char *args[], int argc)
{
    if (argc == 0)
    {
        return;
    }

    if (strcmp(args[0], "cat") == 0)
    {
        run_cat(argc > 1 ? args[1] : NULL);
    }
    else
    {
        fprintf(stderr,
                "minishell: foreground command not implemented: %s\n",
                args[0]);
    }
}

int run_foreground(char *args[], int argc)
{
    if (argc == 0)
    {
        return 0;
    }

    if (strcmp(args[0], "cat") != 0)
    {
        return 0;
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        execute_foreground_command(args, argc);

        _exit(0);
    }

    waitpid(pid, NULL, 0);

    return 1;
}