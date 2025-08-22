/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_fix.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 22:18:21 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static size_t	count_var_chars(char *line, size_t i)
{
	if (!line || !line[i])
		return (0);
	if (!ft_isalnum(line[i]) && line[i] != '_')
		return (0);
	return (1 + count_var_chars(line, i + 1));
}

size_t	handle_dollar(char *line)
{
	if (!line)
		return (0);
	if (line[0] == '?')
		return (1);
	return (count_var_chars(line, 0));
}

// size_t	increment_index(char *line)
// {
// 	size_t	i;

// 	i = 0;
// 	if (!line)
// 		return (0);
// 	if (line[i] == '|')
// 		return (1);
// 	if (is_rd(line[i]))
// 		return (handle_rd(line));
// 	if (line[i] && !is_whitespace(line[i]) && !is_separator(line[i]))
// 	{
// 		i++;
// 		if (line[i] && !is_whitespace(line[i]) && !is_separator(line[i]))
// 			i++;
// 		if (line[i] && !is_whitespace(line[i]) && !is_separator(line[i]))
// 			i++;
// 		if (line[i] && !is_whitespace(line[i]) && !is_separator(line[i]))
// 			i++;
// 		if (line[i] && !is_whitespace(line[i]) && !is_separator(line[i]))
// 			i++;
// 	}
// 	return (i);
// }

size_t	token_dub_loop(char *line, t_token *token)
{
	size_t	i;

	i = 0;
	if (!line || !token)
		return (0);
	if (line[i] == 39)
	{
		token->sq = true;
		return (handle_sq(line));
	}
	if (line[i] == 34)
	{
		token->dq = true;
		return (handle_dq(line));
	}
	if (i < ft_strlen(line) && line[i] && !is_separator(line[i]))
		i++;
	if (i < ft_strlen(line) && line[i] && !is_separator(line[i]))
		i++;
	if (i < ft_strlen(line) && line[i] && !is_separator(line[i]))
		i++;
	if (i < ft_strlen(line) && line[i] && !is_separator(line[i]))
		i++;
	if (i < ft_strlen(line) && line[i] && !is_separator(line[i]))
		i++;
	return (i);
}

static size_t	find_quote_end(char *line, size_t i, char quote)
{
	if (!line || !line[i])
		return (i);
	if (line[i] == quote)
		return (i + 1);
	return (find_quote_end(line, i + 1, quote));
}

size_t	handle_sq(char *line)
{
	if (!line || line[0] != 39)
		return (0);
	return (find_quote_end(line, 1, 39));
}

size_t	handle_dq(char *line)
{
	if (!line || line[0] != 34)
		return (0);
	return (find_quote_end(line, 1, 34));
}
