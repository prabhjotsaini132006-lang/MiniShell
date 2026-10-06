#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "signals.h"

static void handle_sigint(int signal_number)
{
    (void)signal_number;

    write(STDOUT_FILENO, "\n", 1);
}

static void handle_sigtstp(int signal_number)
{
    (void)signal_number;

    write(STDOUT_FILENO, "\n", 1);
}

static void handle_sigchld(int signal_number)
{
    (void)signal_number;

    while (waitpid(-1, NULL, WNOHANG) > 0)
    {
    }
}

void setup_signal_handlers(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_sigtstp);
    signal(SIGCHLD, handle_sigchld);
}