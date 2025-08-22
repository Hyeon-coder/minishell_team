/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_setter_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:02:06 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/14 19:02:09 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	com_path_setter(char *str, t_com *token)
{
	if (!str || !token)
		return ;
	if (ftstrncmp("./", str, 2))
		token->type = REL_PATHF;
	if (ftstrncmp("/", str, 1))
		token->type = PATH;
}

void	token_path_setter(char *str, t_token *token)
{
	if (!str || !token)
		return ;
	if (ftstrncmp("./", str, 2))
		token->type = REL_PATHF;
	if (ftstrncmp("/", str, 1))
		token->type = PATH;
}

void	init_token_vals(t_token *token)
{
	token->sq = false;
	token->dq = false;
	token->next = NULL;
	token->prev = NULL;
}

bool	is_separator(char c)
{
	if (is_rd(c))
		return (true);
	if (is_whitespace(c))
		return (true);
	if (c == 32)
		return (true);
	return (false);
}
