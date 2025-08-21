/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 00:46:19 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static pid_t	create_child_process(t_com *cmd, t_shell *sh, int i, int n,
				int prev[2], int next[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		child_exec(cmd, sh, i, n, prev, next);
	return (pid);
}

static void	close_and_update_pipes(int prev[2], int next[2])
{
	close_pipe_pair(prev);
	prev[0] = next[0];
	prev[1] = next[1];
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
		pids[i] = create_child_process(cmd, sh, i, n, prev, next);
		if (pids[i] < 0)
			return (free(pids), 1);
		close_and_update_pipes(prev, next);
		cmd = cmd->next;
		i++;
	}
	close_pipe_pair(prev);
	i = wait_all(pids, n, sh);
	free(pids);
	return (i);
}
