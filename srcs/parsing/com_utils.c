/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   com_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:36:15 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 03:21:10 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	count_args(t_token **tokens)
{
	size_t	i;
	t_token	*current;

	if (!tokens || !*tokens)
		return (0);
	current = *tokens;
	i = 0;
	while (current && current->type != PIPE)
	{
		if (current->type != RD_I && current->type != RD_O 
			&& current->type != RD_O_APPEND && current->type != HERE_DOC)
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
	while (current)
	{
		if (current->type == PIPE)
			res++;
		current = current->next;
	}
	return (res);
}

void	fill_o_dir(t_com *new, t_token *d)
{
	if (!new || !d || !d->next)
		return;
	new->outfile = ft_strdup(d->next->str);
	new->redir_type_out = true;
	if (d->type == RD_O_APPEND)
		new->append = true;
}

void	fill_in_dir(t_com *new, t_token *d)
{
	if (!new || !d || !d->next)
		return;
	new->infile = ft_strdup(d->next->str);
	new->redir_type_in = true;
}

void	setup_directors(t_com *new, t_token **tokens)
{
	t_token	*last_in_dir;
	t_token	*last_o_dir;
	t_token	*current;

	if (!tokens || !*tokens)
		return;
	current = *tokens;
	last_in_dir = NULL;
	last_o_dir = NULL;
	while (current && current->type != PIPE)
	{
		if (current->type == RD_I || current->type == HERE_DOC)
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
