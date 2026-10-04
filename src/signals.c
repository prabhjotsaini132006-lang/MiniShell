#include <stdio.h>
#include <signal.h>

#include "signals.h"

static void handle_sigint(int signal_number)
{
    (void)signal_number;

    printf("\nminishell> ");
    fflush(stdout);
}

static void handle_sigtstp(int signal_number)
{
    (void)signal_number;

    printf("\nminishell> ");
    fflush(stdout);
}

void setup_signal_handlers(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_sigtstp);
}