#include <stdio.h>
#include <string.h>

int main(void)
{
    char input[] = "ls -l /home";
    
    char *token = strtok(input, " ");

    while(token != NULL)
    {
        printf("Token: %s\n", token);

        token = strtok(NULL, " ");
    }

    return 0;
}