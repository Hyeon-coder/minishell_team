/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_parent.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/21 23:21:20 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	save_std(int *in, int *out)
{
	*in = dup(STDIN_FILENO);
	*out = dup(STDOUT_FILENO);
	if (*in < 0 || *out < 0)
	{
		if (*in >= 0)
			close(*in);
		if (*out >= 0)
			close(*out);
		perror("dup");
		return (-1);
	}
	return (0);
}

static void	restore_std(int in, int out)
{
	if (in >= 0)
		dup2(in, STDIN_FILENO);
	if (out >= 0)
		dup2(out, STDOUT_FILENO);
	if (in >= 0)
		close(in);
	if (out >= 0)
		close(out);
}

int	run_builtin_parent(t_com *cmd, t_shell *sh)
{
	int	saved_in;
	int	saved_out;
	int	ret;

	if (save_std(&saved_in, &saved_out) < 0)
		return (1);
	if (apply_redirs(cmd, sh) < 0)
	{
		restore_std(saved_in, saved_out);
		return (1);
	}
	ret = handle_builtin_parent(cmd->args, &sh->envs, sh);
	restore_std(saved_in, saved_out);
	return (ret);
}

int	run_builtin_parent_with_redirs(t_com *cmd, t_shell *sh)
{
	int	saved_in;
	int	saved_out;
	int	rc;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in < 0 || saved_out < 0)
		return (sh->last_exit = 1, 1);
	if (apply_redirs(cmd, sh) != 0)
	{
		dup2(saved_in, STDIN_FILENO);
		dup2(saved_out, STDOUT_FILENO);
		close(saved_in);
		close(saved_out);
		return (sh->last_exit = 1, 1);
	}
	rc = run_builtin_parent(cmd, sh);
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (rc);
}
