#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include "commands.h"
#include <fcntl.h>

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