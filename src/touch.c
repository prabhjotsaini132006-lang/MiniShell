#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "touch.h"

void run_touch(const char *filename)
{
    if (filename == NULL)
    {
        fprintf(stderr, "touch: missing operand\n");
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