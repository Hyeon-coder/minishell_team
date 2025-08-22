/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_setter_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:02:06 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 20:15:07 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	does_env_exist(t_token *token, t_env **envs)
{
	t_env	*current;

	if (!*envs)
		return (false);
	if (token->dq || token->sq)
		return (true);
	current = *envs;
	if (ft_strcmp(token->str, "$"))
		return (true);
	while (current)
	{
		if (ft_strcmp(&token->str[1], current->name) == 0)
			return (true);
		current = current->next;
	}
	return (false);
}

void	com_path_setter(char *str, t_com *token)
{
	if (!str || !token)
		return ;
	if (token->type != WORD)
		return ;
	if (ft_strncmp("./", str, 2))
		token->type = PATH;
	if (ft_strncmp("/", str, 1))
		token->type = PATH;
	if (ft_strcmp("cd", str))
		token->type = CD;
}

void	token_path_setter(char *str, t_token *token)
{
	if (!str || !token)
		return ;
	if (token->type != WORD)
		return ;
	if (ft_strncmp("./", str, 2))
		token->type = PATH;
	if (ft_strncmp("/", str, 1))
		token->type = PATH;
	if (ft_strcmp("cd", str))
		token->type = CD;
}

void	init_token_vals(t_token *token)
{
	token->sq = false;
	token->dq = false;
	token->does_exist = false;
	token->next = NULL;
	token->prev = NULL;
}

bool	is_separator(char c)
{
	if (is_rd(c) || is_meta(c))
		return (true);
	if (is_whitespace(c) || c == '|')
		return (true);
	if (c == 32 || c == '$' || c == 39)
		return (true);
	return (false);
}
