/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dub_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 22:43:54 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 22:19:29 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static size_t	count_word_chars_recursive(char *line, size_t i)
// {
// 	if (!line || !line[i])
// 		return (0);
// 	if (is_whitespace(line[i]) || is_separator(line[i]))
// 		return (0);
// 	return (1 + count_word_chars_recursive(line, i + 1));
// }

// static size_t	count_word_chars(char *line, size_t start)
// {
// 	return (count_word_chars_recursive(line, start));
// }

// size_t	token_dub_loop(char *line, t_token *token)
// {
// 	if (!line || !token)
// 		return (0);
// 	if (line[0] == 39)
// 	{
// 		token->sq = true;
// 		return (handle_sq(line));
// 	}
// 	if (line[0] == 34)
// 	{
// 		token->dq = true;
// 		return (handle_dq(line));
// 	}
// 	if (line[0] == '$')
// 	{
// 		return (1 + handle_dollar(&line[1]));
// 	}
// 	if (is_rd(line[0]) || line[0] == '|')
// 	{
// 		if (line[0] == '<' && line[1] == '<')
// 			return (2);
// 		if (line[0] == '>' && line[1] == '>')
// 			return (2);
// 		return (1);
// 	}
// 	return (count_word_chars(line, 0));
// }

// static void	copy_until_quote_recursive(char *line, char *res, size_t i, 
// 	size_t j, char quote)
// {
// 	if (!line || !res || !line[i] || line[i] == quote)
// 	{
// 		res[j] = '\0';
// 		return ;
// 	}
// 	res[j] = line[i];
// 	copy_until_quote_recursive(line, res, i + 1, j + 1, quote);
// }

// void	sq_dub(char *line, char *res, size_t l)
// {
// 	if (!line || !res || l < 3)
// 	{
// 		if (res)
// 			res[0] = '\0';
// 		return ;
// 	}
// 	copy_until_quote_recursive(line, res, 1, 0, 39);
// }

// void	dq_dub(char *line, char *res, size_t l)
// {
// 	if (!line || !res || l < 3)
// 	{
// 		if (res)
// 			res[0] = '\0';
// 		return ;
// 	}
// 	copy_until_quote_recursive(line, res, 1, 0, 34);
// }
