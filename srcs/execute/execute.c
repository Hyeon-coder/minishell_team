/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:22:28 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/23 05:56:44 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_cmds(t_com *cur)
{
	int	n;

	n = 0;
	while (cur)
	{
		n++;
		cur = cur->next;
	}
	return (n);
}

static int	has_redirs(t_com *cmd)
{
	if (!cmd)
		return (0);
	if (cmd->infile != NULL)
		return (1);
	if (cmd->outfile != NULL)
		return (1);
	/* If you have heredoc flags/fields, check them here as well. */
	return (0);
}

static int	needs_parent(const char *name)
{
	if (!name)
		return (0);
	if (!ft_strcmp(name, "cd"))
		return (1);
	if (!ft_strcmp(name, "export"))
		return (1);
	if (!ft_strcmp(name, "unset"))
		return (1);
	if (!ft_strcmp(name, "exit"))
		return (1);
	return (0);
}

/* 빈 명령어인지 확인하는 함수 */
static int	is_empty_command(t_com *cmd)
{
	if (!cmd)
		return (1);
	if (!cmd->args)
		return (1);
	if (!cmd->args[0])
		return (1);
	if (cmd->args[0][0] == '\0')
		return (1);
	return (0);
}

/* 빈 명령어 처리 - 리다이렉션만 있으면 처리, 아니면 성공 */
static int	handle_empty_command(t_com *cmd, t_shell *sh)
{
	int	saved_in;
	int	saved_out;

	/* 리다이렉션이 있으면 처리 */
	if (has_redirs(cmd))
	{
		saved_in = dup(STDIN_FILENO);
		saved_out = dup(STDOUT_FILENO);
		if (saved_in < 0 || saved_out < 0)
			return (1);
		
		if (apply_redirs(cmd, sh) < 0)
		{
			if (saved_in >= 0)
				close(saved_in);
			if (saved_out >= 0)
				close(saved_out);
			return (1);
		}
		
		/* 표준 입출력 복원 */
		if (saved_in >= 0)
		{
			dup2(saved_in, STDIN_FILENO);
			close(saved_in);
		}
		if (saved_out >= 0)
		{
			dup2(saved_out, STDOUT_FILENO);
			close(saved_out);
		}
	}
	
	/* 빈 명령어는 성공으로 처리 (bash 동작과 동일) */
	return (0);
}

int	execute(t_shell *sh)
{
	t_com	*cmd;
	int		n;

	cmd = sh->commands;
	if (!cmd)
		return (0);
	
	/* 빈 명령어 처리 */
	if (is_empty_command(cmd))
	{
		sh->last_exit = handle_empty_command(cmd, sh);
		return (sh->last_exit);
	}
	
	n = count_cmds(cmd);
	if (n == 1 && is_builtin(cmd->args[0]))
	{
		if (needs_parent(cmd->args[0]))
		{
			if (has_redirs(cmd))
			{
				sh->last_exit = run_builtin_parent_with_redirs(sh, cmd);
				return (sh->last_exit);
			}
			sh->last_exit = run_builtin_parent(cmd, sh);
			return (sh->last_exit);
		}
		if (!has_redirs(cmd))
		{
			sh->last_exit = run_builtin_parent(cmd, sh);
			return (sh->last_exit);
		}
	}
	return (exec_pipeline(sh, cmd, n));
}
