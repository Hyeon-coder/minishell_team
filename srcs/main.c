/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:52:51 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/23 06:51:21 by juhyeonl         ###   ########.fr       */
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
	
	(void)argc;
	(void)argv;
	sh.envs = env_init(envp);
	if (!sh.envs)
	{
		ft_putendl_fd("minishell: environment initialization failed", 2);
		return (1);
	}
	sh.commands = NULL;
	sh.tokens = NULL;
	sh.last_exit = 0;
	
	setup_signals();
	while ((line = readline(GRN "minishell> " RESET)))
	{
		if (*line)
			add_history(line);
		sh.tokens = tokenize(line, &sh.tokens, &sh);
		if (sh.tokens && *line)
			sh.commands = init_coms(&sh.tokens, &sh.commands, &sh);
		if (sh.commands)
			execute(&sh);
		free_sh_tokens(&sh.tokens);
		free_coms(&sh.commands);
		free(line);
	}
	env_clear(&sh.envs);
	rl_clear_history();
	return (0);
}
