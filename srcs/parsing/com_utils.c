/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   com_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:36:15 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/07 19:36:19 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	count_args(t_token **tokens)
{
	size_t	i;
	t_token	*current;

	current = *tokens;
	i = 1;
	while (current->next && current->next->type != PIPE)
	{
		i++;
		current = current->next;
	}
	return (i);
}

size_t	count_coms(t_token **tokens)
{
	size_t	res;
	t_token	*current;

	res = 1;
	current = *tokens;
	while (current->next)
	{
		if (current->type == PIPE)
			res++;
		current = current->next;
	}
	return (res);
}

void	fill_o_dir(t_com *new, t_token *d)
{
	new->outfile = d->next->str;
	new->redir_type_out = true;
}

void	fill_in_dir(t_com *new, t_token *d)
{
	new->infile = d->prev->str;
	new->redir_type_in = true;
}

void	setup_directors(t_com *new, t_token **tokens)
{
	t_token	*last_in_dir;
	t_token	*last_o_dir;
	t_token	*current;

	current = *tokens;
	last_in_dir = NULL;
	last_o_dir = NULL;
	while (current->next)
	{
		if (current->type == RD_I)
			last_in_dir = current;
		if (current->type == RD_O || current->type == RD_O_APPEND)
			last_o_dir = current;
		current = current->next;
	}
	if (last_in_dir)
		fill_in_dir(new, last_in_dir);
	if (last_o_dir)
		fill_o_dir(new, last_o_dir);
}
