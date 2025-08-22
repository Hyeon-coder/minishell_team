/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_coms.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:30:08 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/23 02:35:11 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	init_com(t_com *com)
{
	if (!com)
		return ;
	
	com->pipe_fd[0] = -1;
	com->pipe_fd[1] = -1;
	com->args = NULL;
	com->path = NULL;
	com->infile = NULL;
	com->outfile = NULL;
	com->type = WORD;
	com->is_piped = false;
	com->redir_type_in = false;
	com->redir_type_out = false;
	com->append = false;
	com->next = NULL;
	com->prev = NULL;
}

t_com	*make_com(t_token **tokens, t_shell *shell)
{
	t_token	*current;
	t_com	*new;

	if (!*tokens)
		return (NULL);
	new = (t_com *)malloc(sizeof(t_com));
	if (!new)
	{
		print_mem_error("memory allocation error", shell);
		return (NULL);
	}
	
	init_com(new);
	current = *tokens;
	new->type = current->type;
	new->args = make_args(tokens, shell);
	if (!new->args)
	{
		free(new);
		return (NULL);
	}
	setup_directors(new, tokens);
	return (new);
}

void	add_com(t_com *new, t_com **coms)
{
	t_com	*current;

	if (!new || !coms)
		return ;
		
	current = *coms;
	if (!*coms)
	{
		*coms = new;
		return ;
	}
	while (current->next)
		current = current->next;
	current->next = new;
	new->prev = current;
}

t_com	*make_coms(t_token **tokens, t_com **coms, t_shell *shell)
{
	t_token	*current;
	t_com	*new;

	if (!tokens || !*tokens)
		return (NULL);
		
	current = *tokens;
	new = make_com(tokens, shell);
	if (!new)
		return (NULL);
	add_com(new, coms);
	
	while (current->next)
	{
		if (current->type == PIPE)
		{
			new = make_com(&current->next, shell);
			if (!new)
			{
				free_coms(coms);
				return (NULL);
			}
			add_com(new, coms);
		}
		current = current->next;
	}
	return (*coms);
}

void	expand_env_com_types(t_com **coms)
{
	t_com	*current;

	if (!coms || !*coms)
		return ;
	current = *coms;
	while (current)
	{
		if (current->type == EV)
			set_com_type(current->args[0], current);
		current = current->next;
	}
}

t_com	*init_coms(t_token **tokens, t_com **coms, t_shell *shell)
{
	if (!tokens || !*tokens || !coms)
		return (NULL);
		
	*coms = make_coms(tokens, coms, shell);
	if (*coms)
		expand_env_com_types(coms);
	return (*coms);
}
