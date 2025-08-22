/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   missing_functions_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 16:06:07 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 토큰 초기값 설정 */
void	init_token_vals(t_token *token)
{
	if (!token)
		return ;
	token->str = NULL;
	token->type = WORD;
	token->sq = false;
	token->dq = false;
	token->does_exist = false;
	token->next = NULL;
	token->prev = NULL;
}

/* 환경변수 존재 확인 */
bool	does_env_exist(t_token *token, t_env **envs)
{
	t_env	*env_node;

	if (!token || !token->str || !envs || !*envs)
		return (false);
	env_node = env_find(*envs, token->str);
	return (env_node != NULL);
}

/* 토큰 경로 설정 */
void	token_path_setter(char *str, t_token *token)
{
	if (!str || !token)
		return ;
	if (ft_strcmp(str, "cd") == 0)
		token->type = CD;
	else if (ft_strcmp(str, "echo") == 0)
		token->type = ECHO;
	else if (ft_strcmp(str, "pwd") == 0)
		token->type = PWD;
	else if (ft_strcmp(str, "export") == 0)
		token->type = EXPORT;
	else if (ft_strcmp(str, "unset") == 0)
		token->type = UNSET;
	else if (ft_strcmp(str, "env") == 0)
		token->type = ENV;
	else if (ft_strcmp(str, "exit") == 0)
		token->type = EXIT;
	else
		token->type = WORD;
}

/* 명령어 경로 설정 */
void	com_path_setter(char *str, t_com *com)
{
	if (!str || !com)
		return ;
	if (ft_strcmp(str, "cd") == 0)
		com->type = CD;
	else if (ft_strcmp(str, "echo") == 0)
		com->type = ECHO;
	else if (ft_strcmp(str, "pwd") == 0)
		com->type = PWD;
	else if (ft_strcmp(str, "export") == 0)
		com->type = EXPORT;
	else if (ft_strcmp(str, "unset") == 0)
		com->type = UNSET;
	else if (ft_strcmp(str, "env") == 0)
		com->type = ENV;
	else if (ft_strcmp(str, "exit") == 0)
		com->type = EXIT;
	else
		com->type = WORD;
}

/* 외부 명령어 실행 함수 */
int	execute_external_command(t_com *cmd, t_shell *sh)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		run_external(cmd->args, sh->envs, sh);
		exit(127);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
