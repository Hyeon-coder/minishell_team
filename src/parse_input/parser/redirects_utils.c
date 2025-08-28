/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:07:38 by mpierce           #+#    #+#             */
/*   Updated: 2025/08/28 13:58:04 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	start_heredoc(t_ms *ms, char *lim, t_infile *infile, int quo)
{
	ms->heredoc_name = heredoc_name(ms, ms->heredoc_no);
	signal(SIGINT, handle_signal);
	rl_event_hook = heredoc_sigint;
	if (handle_heredoc(ms, lim, ms->heredoc_name, quo) == -1)
	{
		unlink(ms->heredoc_name);
		free(ms->heredoc_name);
		ms->heredoc_name = NULL;
		ms->heredoc_no--;
		return (1);
	}
	rl_event_hook = NULL;
	if (infile->name)
	{
		free(infile->name);
		infile->name = NULL;
	}
	infile->name = ms->heredoc_name;
	ms->heredoc_name = NULL;
	return (0);
}

void	close_pipes(t_ms *ms)
{
	if (ms->ms_fd[0] != -1)
	{
		close(ms->ms_fd[0]);
		ms->ms_fd[0] = -1;
	}
	if (ms->ms_fd[1] != -1)
	{
		close(ms->ms_fd[1]);
		ms->ms_fd[1] = -1;
	}
	if (ms->prev_fd != -1)
	{
		close(ms->prev_fd);
		ms->prev_fd = -1;
	}
}

/*
** Prepare heredoc information for the command.
*/
bool	setup_heredoc(t_ms *ms, t_token *token, t_infile **infile)
{
	int	quoted;

	quoted = token->next->quoted;
	ms->heredoc_no++;
	*infile = new_infile("", 1);
	if (start_heredoc(ms, token->next->value, *infile, quoted))
	{
		free((*infile)->name);
		free(*infile);
		return (false);
	}
	return (true);
}

/*
** Check if the heredoc limit is reached. Max 16 heredocs allowed.
*/
bool	check_heredoc_limit(t_ms *ms)
{
	t_token	*current;
	int		heredoc_count;

	current = ms->token;
	heredoc_count = 0;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
			heredoc_count++;
		if (heredoc_count > 16)
		{
			ft_putstr_fd(HEREDOC_LIM, STDERR_FILENO);
			return (false);
		}
		current = current->next;
	}
	return (true);
}
