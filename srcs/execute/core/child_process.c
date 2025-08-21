/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:29:52 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/21 23:29:55 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	connect_input_pipe(int i, int prev[2])
{
	if (i > 0 && prev[0] != -1)
	{
		if (dup2(prev[0], STDIN_FILENO) < 0)
		{
			perror("dup2 stdin");
			exit(1);
		}
	}
}

static void	connect_output_pipe(int i, int n, int next[2])
{
	if (i < n - 1 && next[1] != -1)
	{
		if (dup2(next[1], STDOUT_FILENO) < 0)
		{
			perror("dup2 stdout");
			exit(1);
		}
	}
}

static void	connect_pipes(int i, int n, int prev[2], int next[2])
{
	connect_input_pipe(i, prev);
	connect_output_pipe(i, n, next);
	close_pipe_pair(prev);
	close_pipe_pair(next);
}

static int	execute_command(t_com *cmd, t_shell *sh)
{
	int	exit_code;

	if (!cmd->args || !cmd->args[0])
		return (0);
	if (is_builtin_name(cmd->args[0]))
	{
		exit_code = handle_builtin_child(cmd->args, &sh->envs, sh);
		return (exit_code);
	}
	run_external(cmd->args, sh->envs, sh);
	return (sh->last_exit);
}

void	child_exec(t_com *cmd, t_shell *sh, int i, int n,
			int prev[2], int next[2])
{
	int	exit_code;

	set_child_signals();
	connect_pipes(i, n, prev, next);
	if (apply_redirs(cmd, sh) < 0)
		exit(1);
	exit_code = execute_command(cmd, sh);
	exit(exit_code);
}
