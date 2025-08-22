/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_validator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:56:42 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 21:00:31 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	does_contain_meta(t_token *token)
{
	size_t	i;

	i = 0;
	if (!token)
		return (false);
	if (!token->str)
		return (false);
	if (token->sq || token->dq)
		return (false);
	while (token->str[i])
	{
		if (is_meta(token->str[i]) || token->str[i] == '|')
			return (true);
		if (token->str[i] == '<' || token->str[i] == '>')
			return (true);
		i++;
	}
	return (false);
}

bool	is_pipe_or_rd(t_token *token)
{
	if (!token)
		return (false);
	if (token->sq || token->dq)
		return (false);
	if (token->type == HERE_DOC)
		return (true);
	if (token->type == PIPE)
		return (true);
	if (token->type == RD_O || token->type == RD_O_APPEND)
		return (true);
	if (token->type == RD_I)
		return (true);
	return (false);
}

bool	is_token_valid(t_token *token)
{
	if (!token)
		return (false);
	if (is_pipe_or_rd(token) && is_pipe_or_rd(token->next))
		return (false);
	if (is_pipe_or_rd(token) && !token->next)
		return (false);
	if (token->type == PIPE && !token->prev)
		return (false);
	if (!token->sq && !token->dq)
	{	
		if (does_contain_meta(token) && token->type == WORD)
			return (false);
	}
	return (true);
}

// bool	token_validator(t_token **tokens, t_shell *shell)
// {
// 	t_token	*token;
// 	bool	valid;

// 	if (!tokens || !*tokens)
// 		return (false);
// 	token = *tokens;
// 	valid = true;
// 	while (token && valid)
// 	{
// 		valid = is_token_valid(token);
// 		if (!valid)
// 			break ;
// 		token = token->next;
// 	}
// 	if (!valid)
// 	{
// 		write_syntax_error("minishell: syntax error near unexpected token ",
// 			shell);
// 	}
// 	return (valid);
// }

// size_t	increment_index(char *line)
// {
// 	size_t	i;

// 	i = 0;
// 	if (!line)
// 		return (i);
// 	if (line[i] == '|')
// 		return (1);
// 	if (line[i] == 39)
// 		return (handle_sq(line));
// 	if (line[i] == 34)
// 		return (handle_dq(line));
// 	if (is_rd(line[i]))
// 		i += rd_loop(&line[i]);
// 	else
// 		i += defloop(&line[i]);
// 	return (i);
// }
