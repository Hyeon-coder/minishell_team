/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 01:45:11 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_pipe_safely(int *fd)
{
	if (fd && *fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}

static void	setup_pipe_io(int i, int n, int prev[2], int next[2])
{
	// 첫 번째 명령어가 아니면 이전 파이프에서 입력 받기
	if (i > 0 && prev[0] >= 0)
	{
		if (dup2(prev[0], STDIN_FILENO) < 0)
		{
			perror("dup2 stdin");
			exit(1);
		}
	}
	
	// 마지막 명령어가 아니면 다음 파이프로 출력
	if (i < n - 1 && next[1] >= 0)
	{
		if (dup2(next[1], STDOUT_FILENO) < 0)
		{
			perror("dup2 stdout");
			exit(1);
		}
	}
	
	// 사용하지 않는 파이프 끝 닫기
	close_pipe_safely(&prev[0]);
	close_pipe_safely(&prev[1]);
	close_pipe_safely(&next[0]);
	close_pipe_safely(&next[1]);
}

static void	restore_original_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin >= 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout >= 0)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

static int	apply_redirections_with_backup(t_com *cmd, t_shell *sh, 
										int *saved_stdin, int *saved_stdout)
{
	// 리다이렉션이 있는 경우에만 원본 fd 백업
	if (cmd->redir_type_in || cmd->redir_type_out)
	{
		if (cmd->redir_type_in)
		{
			*saved_stdin = dup(STDIN_FILENO);
			if (*saved_stdin < 0)
			{
				perror("dup stdin");
				return (-1);
			}
		}
		
		if (cmd->redir_type_out)
		{
			*saved_stdout = dup(STDOUT_FILENO);
			if (*saved_stdout < 0)
			{
				perror("dup stdout");
				if (*saved_stdin >= 0)
					close(*saved_stdin);
				return (-1);
			}
		}
		
		// 리다이렉션 적용
		if (apply_redirs(cmd, sh) < 0)
		{
			restore_original_fds(*saved_stdin, *saved_stdout);
			return (-1);
		}
	}
	
	return (0);
}

void	child_exec(t_com *cmd, t_shell *sh, int i, int n, int prev[2], int next[2])
{
	int	exit_code;
	int	saved_stdin = -1;
	int	saved_stdout = -1;

	// 시그널 설정
	set_child_signals();
	
	// 파이프 설정 (리다이렉션보다 먼저)
	setup_pipe_io(i, n, prev, next);
	
	// 리다이렉션 적용 (백업과 함께)
	if (apply_redirections_with_backup(cmd, sh, &saved_stdin, &saved_stdout) < 0)
	{
		exit(1);
	}
	
	// 명령어 실행
	if (!cmd->args || !cmd->args[0])
	{
		restore_original_fds(saved_stdin, saved_stdout);
		exit(0);
	}
		
	if (is_builtin(cmd->args[0]))
	{
		exit_code = handle_builtin_child(cmd->args, &sh->envs, sh);
		restore_original_fds(saved_stdin, saved_stdout);
		exit(exit_code);
	}
	else
	{
		// 외부 명령어 실행
		run_external(cmd->args, sh->envs, sh);
		restore_original_fds(saved_stdin, saved_stdout);
		exit(sh->last_exit); // run_external이 sh->last_exit을 설정함
	}
}
