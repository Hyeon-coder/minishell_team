/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   missing_functions_complete.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 21:07:01 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setenv_type(t_token *token)
{
	if (!token || !token->str)
		return ;
	if (token->str[0] == '$' && !token->sq)
		token->type = EV;
}

void	init_token_vals(t_token *token)
{
	if (!token)
		return ;
	token->str = NULL;
	token->type = WORD;
	token->sq = false;
	token->dq = false;
	token->does_exist = false;
	token->next = NULL;
	token->prev = NULL;
}

bool	does_env_exist(t_token *token, t_env **envs)
{
	t_env	*env_node;
	char	*name;

	if (!token || !token->str || !envs || !*envs)
		return (false);
	if (token->sq || token->dq)
		return (true);
	if (token->str[0] != '$')
		return (false);
	name = make_name(token->str);
	if (!name)
		return (false);
	env_node = find_env(name, envs);
	free(name);
	return (env_node != NULL);
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

void	token_path_setter(char *str, t_token *token)
{
	if (!str || !token)
		return ;
	if (token->type != WORD)
		return ;
	if (!ft_strncmp("./", str, 2))
		token->type = PATH;
	else if (!ft_strncmp("/", str, 1))
		token->type = PATH;
	else if (!ft_strcmp("cd", str))
		token->type = CD;
	else if (!ft_strcmp("env", str))
		token->type = ENV;
}

// void	com_path_setter(char *str, t_com *com)
// {
// 	if (!str || !com)
// 		return ;
// 	if (com->type != WORD)
// 		return ;
// 	if (!ft_strncmp("./", str, 2))
// 		com->type = PATH;
// 	else if (!ft_strncmp("/", str, 1))
// 		com->type = PATH;
// 	else if (!ft_strcmp("cd", str))
// 		com->type = CD;
// 	else if (!ft_strcmp("env", str))
// 		com->type = ENV;
// }
