#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "redirection.h"
#include "pipeline.h"
#include "background.h"
#include "signals.h"
#include "history.h"
#include "ls.h"
#include "cd.h"
#include "pwd.h"
#include "cat.h"
#include "touch.h"
#include "foreground.h"
#include "mkdir.h"
#include "rm.h"
#include "echo.h"
#include "clear.h"
#include "head.h"
#include "tail.h"
#include "wc.h"
#include "cp.h"
#include "mv.h"
#include "exit.h"

int main(void)
{
    setup_signal_handlers();

    char *input = NULL;
    size_t size = 0;

    while (1)
    {
        printf("minishell> ");
        fflush(stdout);

        if (getline(&input, &size, stdin) == -1)
        {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
        {
            continue;
        }

        add_history_command(input);

        char *args[100];
        int argc = parse_input(input, args);
        int background_result = run_background(args, argc);

        if (background_result != 0)
        {
            continue;
        }
        int pipeline_result = run_pipeline(args, argc);

        if (pipeline_result != 0)
        {
            continue;
        }

        int foreground_result = run_foreground(args, argc);

        if (foreground_result != 0)
        {
            continue;
        }

        int original_stdin = dup(STDIN_FILENO);
        int original_stdout = dup(STDOUT_FILENO);

        if (original_stdin == -1 || original_stdout == -1)
        {
            perror("dup");
            break;
        }

        if (handle_redirection(args, &argc) == -1)
        {
            dup2(original_stdin, STDIN_FILENO);
            dup2(original_stdout, STDOUT_FILENO);

            close(original_stdin);
            close(original_stdout);

            continue;
        }

        if (argc == 0)
        {
            dup2(original_stdin, STDIN_FILENO);
            dup2(original_stdout, STDOUT_FILENO);

            close(original_stdin);
            close(original_stdout);

            continue;
        }

        if (strcmp(args[0], "ls") == 0)
        {
            run_ls(argc > 1 ? args[1] : NULL);
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            run_cd(argc > 1 ? args[1] : NULL);
        }
        else if (strcmp(args[0], "pwd") == 0)
        {
            run_pwd();
        }
        else if (strcmp(args[0], "exit") == 0)
        {
            if (run_exit())
            {
                dup2(original_stdin, STDIN_FILENO);
                dup2(original_stdout, STDOUT_FILENO);

                close(original_stdin);
                close(original_stdout);

                break;
            }
        }
        else if (strcmp(args[0], "cat") == 0)
        {
            run_cat(argc > 1 ? args[1] : NULL);
        }
        else if (strcmp(args[0], "touch") == 0)
        {
            run_touch(argc > 1 ? args[1] : NULL);
        }
        else if (strcmp(args[0], "mkdir") == 0)
        {
            run_mkdir(argc > 1 ? args[1] : NULL);
        }
        else if (strcmp(args[0], "rm") == 0)
        {
            run_rm(argc > 1 ? args[1] : NULL);
        }
        else if (strcmp(args[0], "echo") == 0)
        {
            run_echo(args, argc);
        }
        else if (strcmp(args[0], "clear") == 0)
        {
            run_clear();
        }
       else if (strcmp(args[0], "head") == 0)
        {
            run_head(argc > 1 ? args[1] : NULL);
        }
        else if (strcmp(args[0], "tail") == 0)
        {
            run_tail(argc > 1 ? args[1] : NULL);
        }
        else if (strcmp(args[0], "wc") == 0)
        {
            run_wc(argc > 1 ? args[1] : NULL);
        }
        else if (strcmp(args[0], "cp") == 0)
        {
            run_cp(
                argc > 1 ? args[1] : NULL,
                argc > 2 ? args[2] : NULL
            );
        }
       else if (strcmp(args[0], "mv") == 0)
        {
            run_mv(
                argc > 1 ? args[1] : NULL,
                argc > 2 ? args[2] : NULL
            );
        }
        else if (strcmp(args[0], "history") == 0)
        {
            show_history();
        }
        else
        {
            printf(
                "minishell: command not implemented: %s\n",
                args[0]
            );
        }

        dup2(original_stdin, STDIN_FILENO);
        dup2(original_stdout, STDOUT_FILENO);

        close(original_stdin);
        close(original_stdout);
    }

    free(input);

    return 0;
}