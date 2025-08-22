/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:52:51 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/16 01:55:54 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	print_comms(t_com **coms)
{
	t_com *curr;
	size_t i;
	i = 0;
	if(!*coms)
		return ;
	curr = *coms;
	while(curr)
	{
		i = 0;
		while(curr->args[i])
		{
			printf("%s \n", curr->args[i]);
			i++;
		}
		curr = curr->next;
	}

}

int main(int argc, char **argv, char **envp)
{
	char	*line;
	t_shell	sh;
	t_env *env = (t_env *)malloc(1 * sizeof(t_env *));
	(void)argc;
	(void)argv;
	(void)envp;
	env->name = "World";
	env->value = "Hello";
	sh.commands = NULL;
	sh.tokens = NULL;
	sh.envs = env;
	sh.last_exit = 0;
	setup_signals();
	while ((line = readline(GRN "minishell> " RESET)))
	{
		if (*line)
			add_history(line);
		sh.tokens = tokenize(line, &sh.tokens, &sh);
		if (sh.tokens && *line)
			sh.commands = init_coms(&sh.tokens, &sh.commands, &sh);
		//print_comms(&sh.commands);
		if (sh.commands)
			execute(&sh);
		// printf("[DEBUG] : after execute\n");
		free_sh_tokens(&sh.tokens);
		// printf("[DEBUG] : after free_sh_tokens\n");
		free_coms(&sh.commands);
		// printf("[DEBUG] : after free_coms\n");
		free(line);
		// printf("[DEBUG] : after free\n");
	}
	rl_clear_history();
	return 0;
}
