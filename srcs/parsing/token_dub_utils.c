/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dub_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 22:43:54 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 03:40:50 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	count_check(char *line, size_t i)
{
	if (i == 0)
		return (false);
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
		if (line[i] == 39)  /* 단일 따옴표 */
		{
			token->sq = true;
			i++;
			i += handle_sq(&line[i]);
		}
		else if (line[i] == 34)  /* 이중 따옴표 */
		{
			token->dq = true;
			i++;
			i += handle_dq(&line[i]);
		}
		else if (line[i] == '$')
		{
			i++;
			return (i + handle_dollar(&line[i]));
		}
		else if (is_rd(line[i]) || line[i] == '|')
			break;
		else
			i++;
	}
	return (i);
}

void	sq_dub(char *line, char *res, size_t l)
{
	size_t	i;
	size_t	j;

	if (!line || !res)
		return ;
	i = 0;
	j = 0;
	while (i < l && line[i])
	{
		if (line[i] == 39)  /* 단일 따옴표 건너뛰기 */
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

	if (!line || !res)
		return ;
	i = 0;
	j = 0;
	while (i < l && line[i])
	{
		if (line[i] == 34)  /* 이중 따옴표 건너뛰기 */
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
