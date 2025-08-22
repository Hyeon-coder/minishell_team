/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin_parent_with_redirs.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/16 00:35:04 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <unistd.h>

/*
 * Run a builtin in the parent while applying redirections only
 * for this command. Restores STDIN/STDOUT afterwards.
 * If the builtin is "exit" and it terminates, control never returns.
 */
int	run_builtin_parent_with_redirs(t_shell *sh, t_com *cmd)
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
