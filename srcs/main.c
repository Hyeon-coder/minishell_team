/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 17:21:20 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 04:49:11 by juhyeonl         ###   ########.fr       */
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

t_env	*init_env(void)
{
	t_env	*env;

	env = (t_env *)malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->name = ft_strdup("W");
	env->value = ft_strdup("minishell");
	env->next = NULL;
	if (!env->name || !env->value)
	{
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		free(env);
		return (NULL);
	}
	return (env);
}

void	cleanup_shell(t_shell *sh)
{
	if (!sh)
		return ;
	free_sh_tokens(&sh->tokens);
	free_coms(&sh->commands);
	free_env_list(&sh->envs);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_shell	sh;

	(void)argc;
	(void)argv;
	(void)envp;
	sh.commands = NULL;
	sh.tokens = NULL;
	sh.envs = init_env();
	if (!sh.envs)
		return (1);
	sh.last_exit = 0;
	setup_signals();
	while ((line = readline(GRN "minishell> " RESET)))
	{
		printf("[DEBUG] Input line: '%s'\n", line);
		if (*line)
		{
			add_history(line);
			sh.tokens = tokenize(line, &sh.tokens, &sh);
			printf("[DEBUG] Tokens created: %p\n", (void*)sh.tokens);
			if (sh.tokens && *line)
			{
				sh.commands = init_coms(&sh.tokens, &sh.commands, &sh);
				printf("[DEBUG] Commands created: %p\n", (void*)sh.commands);
			}
			print_comms(&sh.commands);
			if (sh.commands)
			{
				printf("[DEBUG] Executing commands...\n");
				execute_simple_stub(&sh);
				printf("[DEBUG] Execution complete\n");
			}
			free_sh_tokens(&sh.tokens);
			free_coms(&sh.commands);
		}
		free(line);
		printf("[DEBUG] Loop iteration complete\n");
	}
	rl_clear_history();
	cleanup_shell(&sh);
	return (0);
}

// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   main.c                                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
// /*   Updated: 2025/08/22 04:18:05 by juhyeonl         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../includes/minishell.h"

// volatile sig_atomic_t	g_signal = 0;

// static void	init_shell(t_shell *sh, char **envp)
// {
// 	sh->commands = NULL;
// 	sh->tokens = NULL;
// 	sh->envs = env_init(envp);
// 	sh->last_exit = 0;
// }

// static void	cleanup_iteration(t_shell *sh, char *line)
// {
// 	if (sh->tokens)
// 	{
// 		free_sh_tokens(&sh->tokens);
// 		sh->tokens = NULL;
// 	}
// 	if (sh->commands)
// 	{
// 		free_coms(&sh->commands);
// 		sh->commands = NULL;
// 	}
// 	if (line)
// 		free(line);
// }

// static void	process_line(t_shell *sh, char *line)
// {
// 	if (!line || !*line)
// 		return ;
	
// 	/* 대화형 모드에서만 히스토리 추가 */
// 	if (isatty(fileno(stdin)))
// 		add_history(line);
	
// 	sh->tokens = tokenize(line, &sh->tokens, sh);
// 	if (!sh->tokens)
// 		return ;
	
// 	sh->commands = init_coms(&sh->tokens, &sh->commands, sh);
// 	if (!sh->commands)
// 		return ;
	
// 	/* 디버깅: 명령어 실행 전 상태 확인 */
// 	if (sh->commands && sh->commands->args && sh->commands->args[0])
// 	{
// 		/* execute 호출 */
// 		execute(sh);
		
// 		/* 디버깅: exit 명령어면 강제 종료 */
// 		if (ft_strcmp(sh->commands->args[0], "exit") == 0)
// 		{
// 			env_clear(&sh->envs);
// 			exit(sh->last_exit);
// 		}
// 	}
// }

// static void	handle_signal_exit(t_shell *sh)
// {
// 	if (g_signal == SIGINT)
// 	{
// 		sh->last_exit = 130;
// 		g_signal = 0;
// 	}
// 	else if (g_signal == SIGQUIT)
// 	{
// 		sh->last_exit = 131;
// 		g_signal = 0;
// 	}
// }

// /* 간단하고 안전한 read_line 구현 */
// static char	*read_line_from_fd(int fd)
// {
// 	static char	buffer[4096];
// 	static int	buffer_pos = 0;
// 	static int	buffer_size = 0;
// 	char		*line;
// 	int			line_len;
// 	int			i;

// 	/* 버퍼가 비어있으면 읽기 */
// 	if (buffer_pos >= buffer_size)
// 	{
// 		buffer_size = read(fd, buffer, sizeof(buffer) - 1);
// 		if (buffer_size <= 0)
// 			return (NULL);
// 		buffer[buffer_size] = '\0';
// 		buffer_pos = 0;
// 	}

// 	/* 개행문자까지 찾기 */
// 	i = buffer_pos;
// 	while (i < buffer_size && buffer[i] != '\n')
// 		i++;
	
// 	line_len = i - buffer_pos;
// 	line = malloc(line_len + 1);
// 	if (!line)
// 		return (NULL);
	
// 	ft_strlcpy(line, buffer + buffer_pos, line_len + 1);
	
// 	/* 다음 줄로 이동 */
// 	buffer_pos = i + 1;
	
// 	return (line);
// }

// static char	*get_input_line(void)
// {
// 	char	*line;
	
// 	if (isatty(fileno(stdin)))
// 	{
// 		/* 대화형 모드 - readline 사용 */
// 		line = readline("minishell> ");
// 	}
// 	else
// 	{
// 		/* 테스터/파이프 모드 - 간단한 read 사용 */
// 		line = read_line_from_fd(fileno(stdin));
// 	}
// 	return (line);
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	char	*line;
// 	t_shell	sh;

// 	(void)argc;
// 	(void)argv;
	
// 	init_shell(&sh, envp);
// 	setup_signals();
	
// 	while (1)
// 	{
// 		line = get_input_line();
		
// 		if (!line)
// 		{
// 			/* 대화형 모드에서만 "exit" 출력 */
// 			if (isatty(fileno(stdin)))
// 				ft_putstr_fd("exit\n", 2);
// 			break ;
// 		}
		
// 		if (g_signal)
// 			handle_signal_exit(&sh);
		
// 		process_line(&sh, line);
// 		cleanup_iteration(&sh, line);
// 	}
	
// 	if (isatty(fileno(stdin)))
// 		rl_clear_history();
// 	env_clear(&sh.envs);
// 	return (sh.last_exit);
// }
