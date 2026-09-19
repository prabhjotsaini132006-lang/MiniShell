#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *input = NULL;
    size_t size = 0;

    while(1){
        printf("minishell> ");
        fflush(stdout);

        if(getline(&input, &size, stdin) == -1)
        {
            printf("\n");
            break;
        }

        printf("You entered: %s", input);
    }
  
    free(input);

    return 0;
}