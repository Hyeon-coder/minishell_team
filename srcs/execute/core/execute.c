/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/21 23:23:17 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_pipeline(const t_com *head)
{
	int	count;

	count = 0;
	while (head)
	{
		++count;
		head = head->next;
	}
	return (count);
}

static int	has_redirs(const t_com *cmd)
{
	if (!cmd)
		return (0);
	if (cmd->infile && cmd->infile[0])
		return (1);
	if (cmd->outfile && cmd->outfile[0])
		return (1);
	if (cmd->heredoc_delimiter && cmd->heredoc_delimiter[0])
		return (1);
	if (cmd->append)
		return (1);
	return (0);
}

static int	exec_single(t_shell *sh, t_com *cmd)
{
	int		status;
	char	*name;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		sh->last_exit = 0;
		return (0);
	}
	name = cmd->args[0];
	if (is_builtin_name(name))
	{
		if (has_redirs(cmd))
			status = run_builtin_parent_with_redirs(cmd, sh);
		else
			status = run_builtin_parent(cmd, sh);
		sh->last_exit = status;
		return (status);
	}
	return (exec_pipeline(sh, cmd, 1));
}

int	execute(t_shell *sh)
{
	int		ncmds;
	t_com	*first;

	if (!sh)
		return (1);
	first = sh->commands;
	ncmds = count_pipeline(first);
	if (ncmds <= 0)
	{
		sh->last_exit = 0;
		return (0);
	}
	if (ncmds == 1)
		return (exec_single(sh, first));
	return (exec_pipeline(sh, first, ncmds));
}
