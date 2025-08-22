/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_parent.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:25:06 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/15 16:55:17 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	ret = handle_builtin(cmd->args, &sh->envs, sh);
	restore_std(saved_in, saved_out);
	return (ret);
}
