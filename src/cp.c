#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "cp.h"

void run_cp(const char *source, const char *destination)
{
    if (source == NULL || destination == NULL)
    {
        fprintf(stderr, "cp: missing operand\n");
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