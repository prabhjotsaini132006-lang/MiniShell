#include <stdio.h>
#include <unistd.h>

#include "pwd.h"

void run_pwd(void)
{
    char cwd[4096];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("pwd");
        return;
    }

    printf("%s\n", cwd);
}