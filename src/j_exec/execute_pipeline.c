/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/27 23:40:02 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * 파이프 자식 프로세스 실행
 * ge_mini_shell의 execute_pipe_child 로직 적용
 */
static void	execute_pipe_child(t_ms *ms, t_ast *node, int *pipe_fd, int direction)
{
	// direction: 0 = 왼쪽(출력을 pipe로), 1 = 오른쪽(입력을 pipe에서)
	
	if (direction == 0)
	{
		// 왼쪽 명령어: stdout을 pipe의 write end로
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	else
	{
		// 오른쪽 명령어: stdin을 pipe의 read end에서
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}

	// 재귀적으로 AST 실행 (자식에서)
	exit(_executor(ms, node, 1));
}

/**
 * ge_mini_shell의 execute_pipeline 로직을 minishell_team 구조에 적용
 * 재귀적 AST 구조를 활용한 파이프라인 처리
 */
int	execute_pipeline(t_ms *ms, t_ast *node)
{
	int		pipe_fd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status_left;
	int		status_right;

	if (!node || !node->left || !node->right)
		return (1);

	// 파이프 생성
	if (pipe(pipe_fd) == -1)
	{
		ft_putstr_fd("minishell: pipe: ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (1);
	}

	// 왼쪽 명령어 실행 (자식 프로세스)
	pid_left = fork();
	if (pid_left == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		ft_putstr_fd("minishell: fork: ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (1);
	}
	else if (pid_left == 0)
	{
		execute_pipe_child(ms, node->left, pipe_fd, 0);
	}

	// 오른쪽 명령어 실행 (자식 프로세스)
	pid_right = fork();
	if (pid_right == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		ft_putstr_fd("minishell: fork: ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (1);
	}
	else if (pid_right == 0)
	{
		execute_pipe_child(ms, node->right, pipe_fd, 1);
	}

	// 부모에서 파이프 닫기
	close(pipe_fd[0]);
	close(pipe_fd[1]);

	// 부모에서 신호 무시
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	// 자식들 대기
	waitpid(pid_left, &status_left, 0);
	waitpid(pid_right, &status_right, 0);

	// 신호 처리 복원
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);

	// 오른쪽 명령어의 exit status 반환 (ge_mini_shell 방식)
	if (WIFEXITED(status_right))
		return (WEXITSTATUS(status_right));
	if (WIFSIGNALED(status_right))
	{
		if (WTERMSIG(status_right) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		return (128 + WTERMSIG(status_right));
	}
	return (1);
}

/**
 * 재귀적 executor 헬퍼 함수
 * ge_mini_shell의 _executor 개념을 minishell_team 구조에 적용
 */
int	_executor(t_ms *ms, t_ast *node, int is_child)
{
	if (!node)
		return (0);

	if (node->type == NODE_PIPE)
		return (execute_pipeline(ms, node));
	else if (node->type == NODE_COMMAND)
		return (execute_simple_command(ms, node->cmd, is_child));
	else if (node->type == NODE_MISSCMD)
	{
		// 명령어 없이 리디렉션만 있는 경우
		if (node->cmd)
			return (apply_redirections_for_empty(ms, node->cmd));
		return (0);
	}

	return (0);
}
