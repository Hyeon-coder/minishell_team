/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/28 14:12:01 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * 자식 프로세스에서 외부 명령어 실행
 * ge_mini_shell의 child_process 로직 적용
 */
static void	child_process(t_ms *ms, t_cmd *cmd)
{
	char	*path;
	char	**envp;

	// 신호 처리 설정 (자식)
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	// 빌트인인 경우 직접 실행 후 exit
	if (is_builtin(cmd->full_cmd[0]))
		exit(execute_builtin_cmd(ms, cmd));

	// 명령어 경로 찾기
	path = find_command_path(cmd->full_cmd[0], ms);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->full_cmd[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		exit(127);
	}

	// 환경변수 배열 준비 (minishell_team은 이미 envp가 배열)
	envp = ms->envp;

	// execve 실행
	execve(path, cmd->full_cmd, envp);
	
	// execve 실패시
	ft_putstr_fd("minishell: execve: ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	free(path);
	exit(126);
}

/**
 * 자식 프로세스 대기 및 상태 처리
 * ge_mini_shell의 wait_for_child 로직 적용
 */
static int	wait_for_child(pid_t pid)
{
	int	status;

	// 부모 프로세스에서 신호 무시
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	
	waitpid(pid, &status, 0);
	
	// 신호 처리 복원
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

/**
 * ge_mini_shell의 execute_simple_command 로직을 minishell_team 구조에 적용
 * t_cmd 구조체 사용, 적절한 리디렉션 처리
 */
int	execute_simple_command(t_ms *ms, t_cmd *cmd, int is_child)
{
	pid_t	pid;
	int		original_fds[2];
	int		status;

	(void)is_child;
	status = 0;
	original_fds[0] = dup(STDIN_FILENO);
	original_fds[1] = dup(STDOUT_FILENO);
	if (original_fds[0] == -1 || original_fds[1] == -1)
	{
		ft_putendl_fd("minishell: dup failed", STDERR_FILENO);
		return (1);
	}
	if (apply_redirections(ms, cmd) == -1)
		status = 1;
	else
	{
		if (!cmd->full_cmd || !cmd->full_cmd[0])
			status = 0;
		else if (is_builtin(cmd->full_cmd[0]))
			status = execute_builtin_cmd(ms, cmd);
		else
		{
			pid = fork();
			if (pid == -1)
			{
				ft_putstr_fd("minishell: fork: ", STDERR_FILENO);
				ft_putendl_fd(strerror(errno), STDERR_FILENO);
				status = 1;
			}
			else if (pid == 0)
				child_process(ms, cmd);
			else
				status = wait_for_child(pid);
		}
	}
	dup2(original_fds[0], STDIN_FILENO);
	dup2(original_fds[1], STDOUT_FILENO);
	close(original_fds[0]);
	close(original_fds[1]);
	ms->exit_status = status;
	return (status);
}
