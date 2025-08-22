/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 21:01:14 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/23 02:35:44 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_args(char **args)
{
	size_t	i;
	
	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
}

void	free_args_partial(char **args, size_t count)
{
	size_t	i;
	
	if (!args)
		return ;
	i = 0;
	while (i < count)
	{
		if (args[i])
		{
			free(args[i]);
			args[i] = NULL;
		}
		i++;
	}
	free(args);
}

void	free_path(t_com *com)
{
	if (!com)
		return ;
	if (com->path)
	{
		free(com->path);
		com->path = NULL;
	}
	if (com->infile)
	{
		free(com->infile);
		com->infile = NULL;
	}
	if (com->outfile)
	{
		free(com->outfile);
		com->outfile = NULL;
	}
}

void	free_coms(t_com **coms)
{
	t_com	*current;
	t_com	*temp;

	if (!coms || !*coms)
		return ;
		
	current = *coms;
	while (current)
	{
		temp = current;
		current = current->next;
		
		if (temp->args)
			free_args(temp->args);
		free_path(temp);
		
		temp->prev = NULL;
		temp->next = NULL;
		free(temp);
	}
	*coms = NULL;
}

void	free_sh_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*temp;

	if (!tokens || !*tokens)
		return ;
		
	current = *tokens;
	while (current)
	{
		temp = current;
		current = current->next;
		
		if (temp->str)
		{
			free(temp->str);
			temp->str = NULL;
		}
		temp->prev = NULL;
		temp->next = NULL;
		free(temp);
	}
	*tokens = NULL;
}

void	free_both(t_shell *shell)
{
	if (!shell)
		return ;
		
	free_sh_tokens(&shell->tokens);
	free_coms(&shell->commands);
}
