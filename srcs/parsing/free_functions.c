/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 21:01:14 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/15 18:07:56 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_args(char **args)
{
	size_t	i;
	if (!args)
		return ;
	if (!*args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_path(t_com *com)
{
	if (!com)
		return ;
	if (com->infile)
		free(com->infile);
	if (com->outfile)
		free(com->outfile);
}

void	free_coms(t_com **coms)
{
	t_com	*current;
	t_com	*temp;

	if (!coms)
		return ;
	if (!*coms)
		return ;
	current = *coms;
	while (current)
	{
		temp = current;
		current = current->next;
		free_args(temp->args);
		free_path(temp);
		temp->prev = NULL;
		if (temp)
			free(temp);
	}
	*coms = NULL;
}

void	free_sh_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*temp;

	if (!tokens)
		return ;
	if (!*tokens)
		return ;
	current = *tokens;
	while (current)
	{
		temp = current;
		current = current->next;
		temp->prev = NULL;
		free(temp);
	}
	*tokens = NULL;
}

void	free_both(t_shell *shell)
{
	free_sh_tokens(&shell->tokens);
	free_coms(&shell->commands);
}
