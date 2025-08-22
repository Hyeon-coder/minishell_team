/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:23:15 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/15 16:56:42 by juhyeonl         ###   ########.fr       */
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

void	child_exec(t_com *cmd, t_shell *sh, int i, int n, int prev[2], int next[2])
{
	set_child_signals();
	connect_pipes(i, n, prev, next);
	if (apply_redirs(cmd, sh) < 0)
		exit(1);
	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
		exit(handle_builtin(cmd->args, &sh->envs, sh));
	run_external(cmd->args, sh->envs, sh);
	exit(127);
}
