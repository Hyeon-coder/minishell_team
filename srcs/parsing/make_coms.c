/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_coms.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:30:08 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 18:49:32 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_comms(t_com **coms)
{
	t_com	*curr;
	int		i;

	if (!coms || !*coms)
		return ;
	curr = *coms;
	while (curr)
	{
		i = 0;
		while (curr->args && curr->args[i])
		{
			i++;
		}
		curr = curr->next;
	}
}

t_com	*make_com(t_token **tokens, t_shell *shell)
{
	t_com	*new;

	if (!*tokens)
		exit(1);
	new = (t_com *)malloc(1 * sizeof(t_com));
	if (!new)
	{
		print_mem_error("minishell: memory allocation error", shell);
		return (NULL);
	}
	new->next = NULL;
	new->prev = NULL;
	new->infile = NULL;
	new->outfile = NULL;
	new->heredoc_delimiter = NULL;
	new->path = NULL;
	new->redirections = NULL;
	new->type = WORD;
	new->is_piped = false;
	new->redir_type_in = false;
	new->redir_type_out = false;
	new->append = false;
	new->args = make_args(tokens, shell);
	setup_directors(new, tokens);
	return (new);
}

void	add_com(t_com *new, t_com **coms)
{
	t_com	*current;

	if (!new->args)
	{
		free(new);
		return ;
	}
	set_com_type(new->args[0], new);
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

bool	path_checker(t_com **coms, t_shell *shell)
{
	(void)coms;
	(void)shell;
	return (true);
}

t_com	*init_coms(t_token **tokens, t_com **coms, t_shell *shell)
{
	if (!tokens)
		return (NULL);
	*coms = make_coms(tokens, coms, shell);
	if (!path_checker(coms, shell))
	{
		free_coms(coms);
		return (NULL);
	}
	return (*coms);
}
