/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:25:28 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/15 16:25:29 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	wait_all(pid_t *pids, int n, t_shell *sh)
{
	int	i;
	int	status;
	int	last;

	i = 0;
	last = 0;
	while (i < n)
	{
		if (waitpid(pids[i], &status, 0) > 0)
		{
			if (WIFEXITED(status))
				last = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last = 128 + WTERMSIG(status);
		}
		i++;
	}
	sh->last_exit = last;
	return (last);
}
