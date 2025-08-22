/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 12:38:18 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 파이프 개수 세기 */
static int	count_pipes(t_com *cmd)
{
	int		count;
	t_com	*current;

	count = 0;
	current = cmd;
	while (current && current->next)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/* 단일 명령어 실행 (파이프 없음) */
static int	execute_single(t_shell *sh, t_com *cmd)
{
	int	status;
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	
	/* 리다이렉션 처리 */
	if (setup_redirections(cmd) < 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}
	
	/* 빌트인 명령어 확인 및 실행 */
	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		status = handle_builtin_parent(cmd->args, &sh->envs, sh);
	}
	else
	{
		status = execute_external_command(cmd, sh);
	}
	
	/* 파일 디스크립터 복구 */
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	
	return (status);
}

/* 파이프라인 실행 */
static int	execute_pipeline(t_shell *sh, t_com *cmd)
{
	int		pipe_count;
	int		status;
	pid_t	*pids;
	int		**pipes;
	int		i;
	t_com	*current;

	pipe_count = count_pipes(cmd);
	pids = malloc(sizeof(pid_t) * (pipe_count + 1));
	pipes = malloc(sizeof(int *) * pipe_count);
	if (!pids || !pipes)
		return (free(pids), free(pipes), 1);
	
	/* 파이프 생성 */
	i = 0;
	while (i < pipe_count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) < 0)
		{
			while (--i >= 0)
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
				free(pipes[i]);
			}
			free(pids);
			free(pipes);
			return (1);
		}
		i++;
	}
	
	/* 각 명령어에 대해 자식 프로세스 생성 */
	current = cmd;
	i = 0;
	while (current)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("fork");
			status = 1;
			break ;
		}
		if (pids[i] == 0)
		{
			/* 자식 프로세스에서 파이프 설정 */
			if (i > 0)
				dup2(pipes[i - 1][0], STDIN_FILENO);
			if (i < pipe_count)
				dup2(pipes[i][1], STDOUT_FILENO);
			
			/* 모든 파이프 닫기 */
			int j = 0;
			while (j < pipe_count)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				j++;
			}
			
			/* 리다이렉션 처리 */
			if (setup_redirections(current) < 0)
				exit(1);
			
			/* 명령어 실행 */
			if (current->args && current->args[0] && is_builtin(current->args[0]))
			{
				status = handle_builtin_child(current->args, &sh->envs, sh);
				exit(status);
			}
			else
			{
				run_external(current->args, sh->envs, sh);
				exit(127);
			}
		}
		current = current->next;
		i++;
	}
	
	/* 부모: 모든 파이프 닫기 */
	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}
	free(pipes);
	
	/* 모든 자식 프로세스 대기 */
	i = 0;
	while (i <= pipe_count)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
	
	free(pids);
	
	/* 마지막 명령어의 종료 상태 반환 */
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

/* 메인 실행 함수 */
int	execute_commands(t_shell *sh)
{
	if (!sh || !sh->commands)
		return (0);
	
	/* 파이프가 있는지 확인 */
	if (sh->commands->next)
		return (execute_pipeline(sh, sh->commands));
	else
		return (execute_single(sh, sh->commands));
}
