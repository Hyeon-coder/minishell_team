/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 02:59:13 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	wait_all(pid_t *pids, int n, t_shell *sh)
{
	int	i;
	int	status;
	int	exit_code;

	exit_code = 0;
	i = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_code = 128 + WTERMSIG(status);
		i++;
	}
	free(pids);
	sh->last_exit = exit_code;
	return (exit_code);
}
