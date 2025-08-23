/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:23:15 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/23 05:57:16 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	connect_pipes(int i, int n, int prev[2], int next[2])
{
	if (i > 0)
	{
		if (dup2(prev[0], STDIN_FILENO) < 0)
			perror("dup2");
	}
	if (i < n - 1)
	{
		if (dup2(next[1], STDOUT_FILENO) < 0)
			perror("dup2");
	}
	close_pipe_pair(prev);
	close_pipe_pair(next);
}

/* 빈 명령어인지 확인하는 함수 */
static int	is_empty_cmd(t_com *cmd)
{
	if (!cmd)
		return (1);
	if (!cmd->args)
		return (1);
	if (!cmd->args[0])
		return (1);
	if (cmd->args[0][0] == '\0')
		return (1);
	return (0);
}

void	child_exec(t_com *cmd, t_shell *sh, int i, int n, int prev[2], int next[2])
{
	set_child_signals();
	connect_pipes(i, n, prev, next);
	
	/* 리다이렉션 적용 */
	if (apply_redirs(cmd, sh) < 0)
		exit(1);
	
	/* 빈 명령어 처리 */
	if (is_empty_cmd(cmd))
	{
		/* 빈 명령어는 성공으로 처리 (bash 동작과 동일) */
		exit(0);
	}
	
	/* 빌트인 명령어 처리 */
	if (is_builtin(cmd->args[0]))
		exit(handle_builtin(cmd->args, &sh->envs, sh));
	
	/* 외부 명령어 실행 */
	run_external(cmd->args, sh->envs, sh);
	exit(127);
}
