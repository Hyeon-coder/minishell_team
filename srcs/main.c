/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 17:21:20 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 18:33:23 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal = 0;

static void	sigint_handler(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	cleanup_shell(t_shell *sh)
{
	if (!sh)
		return ;
	free_sh_tokens(&sh->tokens);
	free_coms(&sh->commands);
	free_env_list(&sh->envs);
}

/* 입력 처리 및 실행 */
static void	process_line(t_shell *sh, char *line)
{
	if (!line_validator(line, sh))
	{
		sh->last_exit = 2;
		return ;
	}
	sh->tokens = tokenize(line, &sh->tokens, sh);
	if (!sh->tokens)
		return ;
	if (!token_validator(&sh->tokens, sh))
	{
		free_sh_tokens(&sh->tokens);
		sh->last_exit = 2;
		return ;
	}
	sh->commands = init_coms(&sh->tokens, &sh->commands, sh);
	if (!sh->commands)
	{
		free_sh_tokens(&sh->tokens);
		return ;
	}
	
	/* 디버그 출력 (테스트용) */
	if (sh->commands && sh->commands->args)
	{
		int j = 0;
		printf("[DEBUG] Command: ");
		while (sh->commands->args[j])
		{
			printf("args[%d]='%s' ", j, sh->commands->args[j]);
			j++;
		}
		printf("\n");
	}
	
	/* 실제 실행 함수 호출 */
	sh->last_exit = execute_commands(sh);
	free_sh_tokens(&sh->tokens);
	free_coms(&sh->commands);
}

/* 프롬프트 생성 */
static char	*get_prompt(void)
{
	static char	prompt[256];
	
	ft_strlcpy(prompt, GRN "minishell> " RESET, sizeof(prompt));
	return (prompt);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_shell	sh;

	(void)argc;
	(void)argv;
	ft_memset(&sh, 0, sizeof(t_shell));
	sh.envs = env_init(envp);  /* env_utils.c의 함수 사용 */
	if (!sh.envs)
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		return (1);
	}
	sh.last_exit = 0;
	setup_signals();
	while (1)
	{
		line = readline(get_prompt());
		if (!line)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		if (*line)
		{
			add_history(line);
			process_line(&sh, line);
		}
		free(line);
	}
	rl_clear_history();
	cleanup_shell(&sh);
	return (sh.last_exit);
}
