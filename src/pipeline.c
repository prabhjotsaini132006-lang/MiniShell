#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "pipeline.h"
#include "redirection.h"
#include "cat.h"
#include "echo.h"
#include "head.h"
#include "tail.h"
#include "wc.h"

#define MAX_COMMANDS 50

static void execute_pipeline_command(char *command[])
{
    int argc = 0;

    while (command[argc] != NULL)
    {
        argc++;
    }

    if (argc == 0)
    {
        return;
    }

    if (handle_redirection(command, &argc) == -1)
    {
        return;
    }

    if (argc == 0)
    {
        return;
    }

    if (strcmp(command[0], "echo") == 0)
    {
        run_echo(command, argc);
    }
    else if (strcmp(command[0], "cat") == 0)
    {
        run_cat(argc > 1 ? command[1] : NULL);
    }
    else if (strcmp(command[0], "head") == 0)
    {
        run_head(argc > 1 ? command[1] : NULL);
    }
    else if (strcmp(command[0], "wc") == 0)
    {
        run_wc(argc > 1 ? command[1] : NULL);
    }

    else if (strcmp(command[0], "tail") == 0)
    {
        run_tail(argc > 1 ? command[1] : NULL);
    }
    else
    {
        fprintf(
            stderr,
            "minishell: pipeline command not implemented: %s\n",
            command[0]
        );
    }
}

int run_pipeline(char *args[], int argc)
{
    int command_count = 1;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(args[i], "|") == 0)
        {
            command_count++;
        }
    }

    if (command_count == 1)
    {
        return 0;
    }

    if (command_count > MAX_COMMANDS)
    {
        fprintf(stderr, "minishell: too many pipeline commands\n");
        return -1;
    }

    char **commands[MAX_COMMANDS];
    int command_index = 0;

    commands[command_index++] = args;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(args[i], "|") == 0)
        {
            if (i == 0 || i == argc - 1 || args[i + 1] == NULL)
            {
                fprintf(stderr, "minishell: invalid pipe\n");
                return -1;
            }

            args[i] = NULL;
            commands[command_index++] = &args[i + 1];
        }
    }

    int pipe_fds[MAX_COMMANDS - 1][2];
    pid_t children[MAX_COMMANDS];

    for (int i = 0; i < command_count - 1; i++)
    {
        if (pipe(pipe_fds[i]) == -1)
        {
            perror("pipe");
            return -1;
        }
    }

    for (int i = 0; i < command_count; i++)
    {
        children[i] = fork();

        if (children[i] == -1)
        {
            perror("fork");

            for (int j = 0; j < command_count - 1; j++)
            {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }

            return -1;
        }

        if (children[i] == 0)
        {
            if (i > 0)
            {
                if (dup2(pipe_fds[i - 1][0], STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            if (i < command_count - 1)
            {
                if (dup2(pipe_fds[i][1], STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            for (int j = 0; j < command_count - 1; j++)
            {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }

            execute_pipeline_command(commands[i]);

            exit(EXIT_SUCCESS);
        }
    }


    for (int i = 0; i < command_count - 1; i++)
    {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }

    for (int i = 0; i < command_count; i++)
    {
        waitpid(children[i], NULL, 0);
    }

    return 1;
}