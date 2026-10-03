#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

void run_ls(void)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");

    if (dir == NULL)
    {
        perror("ls");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

void run_pwd(void)
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("pwd");
        return;
    }

    printf("%s\n", cwd);
}

void run_cd(char *path)
{
    if (path == NULL)
    {
        fprintf(stderr, "cd: missing argument\n");
        return;
    }

    if (chdir(path) == -1)
    {
        perror("cd");
    }
}

void run_cat(char *filename)
{
    if (filename == NULL)
    {
        fprintf(stderr, "cat: missing file operand\n");
        return;
    }

    int fd = open(filename, O_RDONLY);

    if (fd == -1)
    {
        perror("cat");
        return;
    }

    char buffer[1024];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
    {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    if (bytes_read == -1)
    {
        perror("cat");
    }

    close(fd);
}

void run_touch(char *filename)
{
    if (filename == NULL)
    {
        fprintf(stderr, "touch: missing file operand\n");
        return;
    }

    int fd = open(filename, O_WRONLY | O_CREAT, 0644);

    if (fd == -1)
    {
        perror("touch");
        return;
    }

    close(fd);
}

void run_mkdir(char *dirname)
{
    if (dirname == NULL)
    {
        fprintf(stderr, "mkdir: missing operand\n");
        return;
    }

    if (mkdir(dirname, 0755) == -1)
    {
        perror("mkdir");
        return;
    }
}

void run_rm(char *filename)
{
    if (filename == NULL)
    {
        fprintf(stderr, "rm: missing operand\n");
        return;
    }

    if (unlink(filename) == -1)
    {
        perror("rm");
        return;
    }
}

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

void run_clear(void)
{
    printf("\033[H\033[2J");
    fflush(stdout);
}

int run_exit(void)
{
    return 1;
}

void run_head(char *filename)
{
    if (filename == NULL)
    {
        fprintf(stderr, "head: missing file operand\n");
        return;
    }

    int fd = open(filename, O_RDONLY);

    if (fd == -1)
    {
        perror("head");
        return;
    }

    char buffer[1024];
    ssize_t bytes_read;
    int line_count = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
    {
        for (int i = 0; i < bytes_read; i++)
        {
            printf("%c", buffer[i]);

            if (buffer[i] == '\n')
            {
                line_count++;

                if (line_count == 10)
                {
                    close(fd);
                    return;
                }
            }
        }
    }

    if (bytes_read == -1)
    {
        perror("head");
    }

    close(fd);
}

void run_tail(char *filename)
{
    if (filename == NULL)
    {
        fprintf(stderr, "tail: missing file operand\n");
        return;
    }

    int fd = open(filename, O_RDONLY);

    if (fd == -1)
    {
        perror("tail");
        return;
    }

    char buffer[4096];
    ssize_t bytes_read;
    size_t total_size = 0;
    char *data = NULL;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
    {
        char *new_data = realloc(data, total_size + bytes_read + 1);

        if (new_data == NULL)
        {
            fprintf(stderr, "tail: memory allocation failed\n");
            free(data);
            close(fd);
            return;
        }

        data = new_data;

        memcpy(data + total_size, buffer, bytes_read);

        total_size += bytes_read;
        data[total_size] = '\0';
    }

    if (bytes_read == -1)
    {
        perror("tail");
        free(data);
        close(fd);
        return;
    }

    close(fd);

    if (total_size == 0)
    {
        free(data);
        return;
    }

    int newline_count = 0;
    size_t start = total_size;

    for (size_t i = total_size; i > 0; i--)
    {
        if (data[i - 1] == '\n')
        {
            newline_count++;

            if (newline_count == 11)
            {
                start = i;
                break;
            }
        }
    }

    if (newline_count <= 10)
    {
        start = 0;
    }

    printf("%s", data + start);

    free(data);
}

void run_wc(char *filename)
{
    if (filename == NULL)
    {
        fprintf(stderr, "wc: missing file operand\n");
        return;
    }

    int fd = open(filename, O_RDONLY);

    if (fd == -1)
    {
        perror("wc");
        return;
    }

    char buffer[1024];
    ssize_t bytes_read;

    long lines = 0;
    long words = 0;
    long bytes = 0;

    int in_word = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
    {
        bytes += bytes_read;

        for (ssize_t i = 0; i < bytes_read; i++)
        {
            if (buffer[i] == '\n')
            {
                lines++;
            }

            if (buffer[i] == ' ' ||
                buffer[i] == '\n' ||
                buffer[i] == '\t')
            {
                in_word = 0;
            }
            else if (in_word == 0)
            {
                words++;
                in_word = 1;
            }
        }
    }

    if (bytes_read == -1)
    {
        perror("wc");
        close(fd);
        return;
    }

    close(fd);

    printf("%ld %ld %ld %s\n", lines, words, bytes, filename);
}

void run_cp(char *source, char *destination)
{
    if (source == NULL || destination == NULL)
    {
        fprintf(stderr, "cp: missing file operand\n");
        return;
    }

    int source_fd = open(source, O_RDONLY);

    if (source_fd == -1)
    {
        perror("cp");
        return;
    }

    int destination_fd = open(
        destination,
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
    );

    if (destination_fd == -1)
    {
        perror("cp");
        close(source_fd);
        return;
    }

    char buffer[4096];
    ssize_t bytes_read;

    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0)
    {
        ssize_t total_written = 0;

        while (total_written < bytes_read)
        {
            ssize_t bytes_written = write(
                destination_fd,
                buffer + total_written,
                bytes_read - total_written
            );

            if (bytes_written == -1)
            {
                perror("cp");
                close(source_fd);
                close(destination_fd);
                return;
            }

            total_written += bytes_written;
        }
    }

    if (bytes_read == -1)
    {
        perror("cp");
    }

    close(source_fd);
    close(destination_fd);
}

void run_mv(char *source, char *destination)
{
    if (source == NULL || destination == NULL)
    {
        fprintf(stderr, "mv: missing file operand\n");
        return;
    }

    if (rename(source, destination) == -1)
    {
        perror("mv");
        return;
    }
}