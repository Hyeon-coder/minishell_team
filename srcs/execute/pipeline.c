/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:22:58 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/15 16:22:59 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	open_next_pipe(int i, int n, int next[2])
{
	if (i < n - 1)
	{
		if (pipe(next) < 0)
		{
			perror("pipe");
			return (-1);
		}
	}
	else
	{
		next[0] = -1;
		next[1] = -1;
	}
	return (0);
}

int	exec_pipeline(t_shell *sh, t_com *head, int n)
{
	int		i;
	int		prev[2];
	int		next[2];
	pid_t	*pids;
	t_com	*cmd;

	pids = (pid_t *)malloc(sizeof(pid_t) * n);
	if (!pids)
		return (perror("malloc"), 1);
	prev[0] = -1;
	prev[1] = -1;
	cmd = head;
	i = 0;
	while (i < n && cmd)
	{
		if (open_next_pipe(i, n, next) < 0)
			return (free(pids), 1);
		pids[i] = fork();
		if (pids[i] < 0)
			return (perror("fork"), free(pids), 1);
		if (pids[i] == 0)
			child_exec(cmd, sh, i, n, prev, next);
		close_pipe_pair(prev);
		prev[0] = next[0];
		prev[1] = next[1];
		cmd = cmd->next;
		i++;
	}
	close_pipe_pair(prev);
	i = wait_all(pids, n, sh);
	free(pids);
	return (i);
}
