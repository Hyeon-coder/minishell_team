/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dub_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 22:43:54 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/20 22:43:58 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	count_check(char *line, size_t	i)
{
	if (is_rd(line[i]) && !is_whitespace(line[i - 1]))
		return (true);
	if ((!is_rd(line[i]) && is_rd(line[i - 1])) || line[i] == '|')
		return (true);
	return (false);
}

size_t	token_dub_loop(char *line, t_token *token)
{
	size_t	i;

	i = 0;
	while (!is_whitespace(line[i]) && line[i])
	{
		if (line[i] == 39)
		{
			i += handle_sq(&line[i + 1]);
			token->sq = true;
		}
		if (line[i] == 34)
		{
			i += handle_dq(&line[i + 1]);
			token->dq = true;
		}
		if (line[i] == '$')
			return (i += handle_dollar(&line[i + 1]));
		if (is_rd(line[i]))
			break ;
		i++;
	}
	return (i);
}

void	sq_dub(char *line, char *res, size_t l)
{
	size_t	i;
	size_t	j;

	if (!line)
		return ;
	i = 0;
	j = 0;
	while (i < l)
	{
		if (line[i] == 39)
			i++;
		else
		{
			res[j] = line[i];
			i++;
			j++;
		}
	}
	res[j] = '\0';
}

void	dq_dub(char *line, char *res, size_t l)
{
	size_t	i;
	size_t	j;

	if (!line)
		return ;
	i = 0;
	j = 0;
	while (i < l)
	{
		if (line[i] == 34)
			i++;
		else
		{
			res[j] = line[i];
			i++;
			j++;
		}
	}
	res[j] = '\0';
}
