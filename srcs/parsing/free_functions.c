/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 21:01:14 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 04:34:07 by juhyeonl         ###   ########.fr       */
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

void	free_path(t_com *com)
{
	if (!com)
		return ;
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
		temp = current->next;
		free_args(current->args);
		free_path(current);
		current->prev = NULL;
		current->next = NULL;
		free(current);
		current = temp;
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
		temp = current->next;
		if (current->str)
		{
			free(current->str);
			current->str = NULL;
		}
		current->prev = NULL;
		current->next = NULL;
		free(current);
		current = temp;
	}
	*tokens = NULL;
}

void	free_env_list(t_env **envs)
{
	t_env	*current;
	t_env	*temp;

	if (!envs || !*envs)
		return ;
	current = *envs;
	while (current)
	{
		temp = current->next;
		if (current->name)
		{
			free(current->name);
			current->name = NULL;
		}
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		free(current);
		current = temp;
	}
	*envs = NULL;
}

void	free_both(t_shell *shell)
{
	if (!shell)
		return ;
	free_sh_tokens(&shell->tokens);
	free_coms(&shell->commands);
}
