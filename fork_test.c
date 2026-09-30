#include<stdio.h>
#include<unistd.h>

int main(void)
{
    pid_t pid = fork();

    if(pid == -1)
    {
        printf("Fork Failed\n");
    }
    else if(pid == 0)
    {
        printf("I am the Child Process\n");
    }
    else{
        printf("I am the Parent Process\n");
    }

    return 0;
}