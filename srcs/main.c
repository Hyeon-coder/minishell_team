/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 17:21:20 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 11:52:26 by juhyeonl         ###   ########.fr       */
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

/* 실제 환경 변수를 복사하여 링크드 리스트로 만듦 */
t_env	*env_init(char **envp)
{
	t_env	*head;
	t_env	*current;
	t_env	*new;
	int		i;
	char	*equal_pos;

	head = NULL;
	current = NULL;
	i = 0;
	while (envp && envp[i])
	{
		new = (t_env *)malloc(sizeof(t_env));
		if (!new)
			return (free_env_list(&head), NULL);
		equal_pos = ft_strchr(envp[i], '=');
		if (equal_pos)
		{
			new->name = ft_substr(envp[i], 0, equal_pos - envp[i]);
			new->value = ft_strdup(equal_pos + 1);
		}
		else
		{
			new->name = ft_strdup(envp[i]);
			new->value = ft_strdup("");
		}
		new->next = NULL;
		if (!new->name || !new->value)
		{
			free(new->name);
			free(new->value);
			free(new);
			return (free_env_list(&head), NULL);
		}
		if (!head)
			head = new;
		else
			current->next = new;
		current = new;
		i++;
	}
	return (head);
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
	sh.envs = env_init(envp);
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
