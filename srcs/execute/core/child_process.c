/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 01:17:04 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_pipe_io(int i, int n, int prev[2], int next[2])
{
	// 첫 번째 명령어가 아니면 이전 파이프에서 입력 받기
	if (i > 0 && prev[0] != -1)
	{
		if (dup2(prev[0], STDIN_FILENO) < 0)
		{
			perror("dup2 stdin");
			exit(1);
		}
	}
	
	// 마지막 명령어가 아니면 다음 파이프로 출력
	if (i < n - 1 && next[1] != -1)
	{
		if (dup2(next[1], STDOUT_FILENO) < 0)
		{
			perror("dup2 stdout");
			exit(1);
		}
	}
	
	// 사용하지 않는 파이프 끝 닫기
	close_pipe_pair(prev);
	close_pipe_pair(next);
}

void	child_exec(t_com *cmd, t_shell *sh, int i, int n, int prev[2], int next[2])
{
	int	exit_code;

	// 시그널 설정
	set_child_signals();
	
	// 파이프 설정 (리다이렉션보다 먼저)
	setup_pipe_io(i, n, prev, next);
	
	// 리다이렉션 적용 (파이프 설정 후)
	if (apply_redirs(cmd, sh) < 0)
		exit(1);
	
	// 명령어 실행
	if (!cmd->args || !cmd->args[0])
		exit(0);
		
	if (is_builtin(cmd->args[0]))
	{
		exit_code = handle_builtin_child(cmd->args, &sh->envs, sh);
		exit(exit_code);
	}
	else
	{
		// 외부 명령어 실행
		run_external(cmd->args, sh->envs, sh);
		exit(127); // 명령어를 찾을 수 없음
	}
}
