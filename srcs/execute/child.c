/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 03:28:25 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	child_exec(t_com *cmd, t_shell *sh, int i, int n, int prev[2], int next[2])
{
	set_child_signals();
	
	if (i > 0 && prev[0] != -1)
	{
		dup2(prev[0], STDIN_FILENO);
		close_pipe_pair(prev);
	}
	
	if (i < n - 1 && next[1] != -1)
	{
		dup2(next[1], STDOUT_FILENO);
		close_pipe_pair(next);
	}
	
	if (apply_redirs(cmd, sh) != 0)
		exit(sh->last_exit);
	
	if (is_builtin(cmd->args[0]))
	{
		sh->last_exit = handle_builtin_child(cmd->args, &sh->envs, sh);
		exit(sh->last_exit);
	}
	else
	{
		run_external(cmd->args, sh->envs, sh);
		exit(sh->last_exit);
	}
}
