/* srcs/main.c */
#include "../includes/minishell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t g_signal = 0;

/* SIGINT 핸들러: 새 프롬프트만 출력 */
static void sigint_handler(int sig)
{
    g_signal = sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void setup_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}

int main(void)
{
    char    *line;
    char    **args;

    setup_signals();
    while (1)
    {
        line = readline("minishell> ");
        if (!line)
            break ;                   /* Ctrl+D */
        if (*line)
            add_history(line);
        if (strcmp(line, "exit") == 0)
        {
            free(line);
            break;
        }
        args = tokenize(line);      /* 사용자 구현 */
        execute(args);              /* 사용자 구현 */
        free_tokens(args);          /* 사용자 구현 */
        free(line);
    }
    rl_clear_history();
    return 0;
}
