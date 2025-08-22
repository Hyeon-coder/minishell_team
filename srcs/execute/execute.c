/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 04:20:16 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_commands(t_com *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

int	exec_pipeline(t_shell *sh, t_com *head, int n)
{
	pid_t	*pids;
	int		i;
	t_com	*cmd;
	int		prev[2];
	int		next[2];

	prev[0] = -1;
	prev[1] = -1;
	pids = malloc(sizeof(pid_t) * n);
	if (!pids)
		return (1);
	
	cmd = head;
	i = 0;
	while (cmd && i < n)
	{
		if (i < n - 1)
		{
			if (pipe(next) == -1)
			{
				free(pids);
				return (1);
			}
		}
		else
		{
			next[0] = -1;
			next[1] = -1;
		}
		
		pids[i] = fork();
		if (pids[i] == 0)
			child_exec(cmd, sh, i, n, prev, next);
		else if (pids[i] < 0)
		{
			free(pids);
			return (1);
		}
		
		close_pipe_pair(prev);
		prev[0] = next[0];
		prev[1] = next[1];
		
		cmd = cmd->next;
		i++;
	}
	
	close_pipe_pair(prev);
	return (wait_all(pids, n, sh));
}

int	execute(t_shell *sh)
{
	t_com	*cmd;
	int		cmd_count;

	if (!sh || !sh->commands)
		return (0);
	
	cmd = sh->commands;
	cmd_count = count_commands(cmd);
	
	if (cmd_count == 1)
	{
		if (is_builtin(sh->commands->args[0]))
			return (run_builtin_parent_with_redirs(sh->commands, sh));
		else
			return (exec_pipeline(sh, sh->commands, 1));
	}
	else
		return (exec_pipeline(sh, sh->commands, cmd_count));
}
