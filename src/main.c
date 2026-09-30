#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"

int main(void)
{
    char *input = NULL;
    size_t size = 0;

    while (1)
    {
        printf("minishell> ");
        fflush(stdout);

        if (getline(&input, &size, stdin) == -1)
        {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
        {
            continue;
        }

        char *args[100];
        int argc = 0;

        char *token = strtok(input, " ");

        while (token != NULL && argc < 99)
        {
            args[argc] = token;
            argc++;

            token = strtok(NULL, " ");
        }

        args[argc] = NULL;

        if (strcmp(args[0], "ls") == 0)
        {
            run_ls();
            continue;
        }

        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                fprintf(stderr, "cd: missing argument\n");
            }
            else if (chdir(args[1]) == -1)
            {
                perror("cd");
            }

            continue;
        }

        if (strcmp(args[0], "pwd") == 0)
        {
            char cwd[1024];

            if (getcwd(cwd, sizeof(cwd)) == NULL)
            {
                perror("pwd");
            }
            else
            {
                printf("%s\n", cwd);
            }

            continue;
        }

        if (strcmp(args[0], "exit") == 0)
        {
            break;
        }

        printf("minishell: command not implemented: %s\n", args[0]);
    }

    free(input);

    return 0;
}