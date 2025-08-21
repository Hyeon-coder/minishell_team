/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:52:51 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/21 23:45:21 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t g_signal = 0;

/* 디버깅용 함수 - 임시로 main.c에 추가해서 사용  */
void debug_print_tokens(t_token *tokens)
{
	t_token *current = tokens;
	int i = 0;
	
	printf("[DEBUG] Tokens:\n");
	while (current)
	{
		printf("[%d] type=%d, str='%s', sq=%d, dq=%d\n", 
			i, current->type, current->str ? current->str : "NULL", 
			current->sq, current->dq);
		current = current->next;
		i++;
	}
	printf("[DEBUG] End tokens\n");
}

void debug_print_commands(t_com *commands)
{
	t_com *current = commands;
	int i = 0;
	
	printf("[DEBUG] Commands:\n");
	while (current)
	{
		printf("[%d] type=%d\n", i, current->type);
		
		// 모든 args 출력 - 이 부분이 핵심!
		if (current->args)
		{
			int j = 0;
			while (current->args[j])
			{
				printf("    args[%d]='%s'\n", j, current->args[j]);
				j++;
			}
		}
		
		if (current->heredoc_delimiter)
			printf("    heredoc_delimiter='%s'\n", current->heredoc_delimiter);
		current = current->next;
		i++;
	}
	printf("[DEBUG] End commands\n");
}

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
			// printf("print args:%s \n", curr->args[i]);
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
	
	// 기존 하드코딩 대신 실제 envp로 초기화
	sh.commands = NULL;
	sh.tokens = NULL;
	sh.envs = env_init(envp);  // env_init 함수 사용
	sh.last_exit = 0;
	
	// env_init이 실패한 경우 처리
	if (!sh.envs)
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		return (1);
	}
	
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
		// printf("[DEBUG] : after execute\n");
		free_sh_tokens(&sh.tokens);
		// printf("[DEBUG] : after free_sh_tokens\n");
		free_coms(&sh.commands);
		// printf("[DEBUG] : after free_coms\n");
		free(line);
		// printf("[DEBUG] : after free\n");
	}
	
	// 프로그램 종료 시 환경변수 메모리 해제
	env_clear(&sh.envs);
	rl_clear_history();
	return (0);
}

/*
int main(int argc, char **argv, char **envp)
{
	char	*line;
	t_shell	sh;
	t_env *env = (t_env *)malloc(1 * sizeof(t_env));
	(void)argc;
	(void)argv;
	(void)envp;
	env->name = "W";
	env->value = "minishell";
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
		print_comms(&sh.commands);
		if (sh.commands)
			execute(&sh);
		free_sh_tokens(&sh.tokens);
		free_coms(&sh.commands);
		free(line);
	}
	rl_clear_history();
	return 0;
}
*/