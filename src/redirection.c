#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "redirection.h"

int handle_redirection(char *args[], int *argc)
{
    int i;

    for (i = 0; i < *argc; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            if (i + 1 >= *argc)
            {
                fprintf(stderr, "redirection: missing file name\n");
                return -1;
            }

            int fd = open(
                args[i + 1],
                O_WRONLY | O_CREAT | O_TRUNC,
                0644
            );

            if (fd == -1)
            {
                perror("redirection");
                return -1;
            }

            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return -1;
            }

            close(fd);

            args[i] = NULL;
            *argc = i;

            return 0;
        }

        if (strcmp(args[i], ">>") == 0)
        {
            if (i + 1 >= *argc)
            {
                fprintf(stderr, "redirection: missing file name\n");
                return -1;
            }

            int fd = open(
                args[i + 1],
                O_WRONLY | O_CREAT | O_APPEND,
                0644
            );

            if (fd == -1)
            {
                perror("redirection");
                return -1;
            }

            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return -1;
            }

            close(fd);

            args[i] = NULL;
            *argc = i;

            return 0;
        }

        if (strcmp(args[i], "<") == 0)
        {
            if (i + 1 >= *argc)
            {
                fprintf(stderr, "redirection: missing file name\n");
                return -1;
            }

            int fd = open(args[i + 1], O_RDONLY);

            if (fd == -1)
            {
                perror("redirection");
                return -1;
            }

            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return -1;
            }

            close(fd);

            args[i] = NULL;
            *argc = i;

            return 0;
        }
    }

    return 0;
}