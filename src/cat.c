#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "cat.h"

void run_cat(const char *filename)
{
    char buffer[4096];
    ssize_t bytes_read;

    if (filename == NULL)
    {
        while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
        {
            ssize_t total_written = 0;

            while (total_written < bytes_read)
            {
                ssize_t bytes_written = write(
                    STDOUT_FILENO,
                    buffer + total_written,
                    bytes_read - total_written
                );

                if (bytes_written == -1)
                {
                    perror("cat");
                    return;
                }

                total_written += bytes_written;
            }
        }

        if (bytes_read == -1)
        {
            perror("cat");
        }

        return;
    }

    int fd = open(filename, O_RDONLY);

    if (fd == -1)
    {
        perror("cat");
        return;
    }

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
    {
        ssize_t total_written = 0;

        while (total_written < bytes_read)
        {
            ssize_t bytes_written = write(
                STDOUT_FILENO,
                buffer + total_written,
                bytes_read - total_written
            );

            if (bytes_written == -1)
            {
                perror("cat");
                close(fd);
                return;
            }

            total_written += bytes_written;
        }
    }

    if (bytes_read == -1)
    {
        perror("cat");
    }

    close(fd);
}