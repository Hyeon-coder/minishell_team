/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_validator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:56:42 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 18:37:30 by juhyeonl         ###   ########.fr       */
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
	if (ft_strcmp(token->str, "|") == 0 || ft_strcmp(token->str, ">>") == 0)
		return (true);
	if (ft_strcmp(token->str, "<") == 0 || ft_strcmp(token->str, ">") == 0)
		return (true);
	return (false);
}

bool	is_token_valid(t_token *token)
{
	bool	not_present;

	not_present = true;
	if (is_pipe_or_rd(token) && is_pipe_or_rd(token->next))
		not_present = false;
	if (is_pipe_or_rd(token) && !token->next)
		not_present = false;
	if (token->type == HERE_DOC && !token->next)
		not_present = false;
	if (!token->sq && !token->dq)
	{	
		if (is_pipe_or_rd(token) && !token->prev && token->type != HERE_DOC)
			not_present = false;
	}
	if (does_contain_meta(token) && token->type == WORD)
		not_present = false;
	return (not_present);
}

bool	token_validator(t_token **tokens, t_shell *shell)
{
	t_token	*token;
	bool	not_present;

	if (!tokens || !*tokens)
		return (false);
	token = *tokens;
	not_present = true;
	while (token && not_present)
	{
		not_present = is_token_valid(token);
		if (!not_present)
			break ;
		token = token->next;
	}
	if (!not_present)
	{
		write_syntax_error("minishell: syntax error near unexpected token ",
			shell);
	}
	return (not_present);
}

size_t	increment_index(char *line)
{
	size_t	i;

	i = 0;
	if (!line)
		return (i);
	if (line[i] == '|')
		return (1);
	if (line[i] == 39)  /* 단일 따옴표 */
		return (handle_sq(line));
	if (line[i] == 34)  /* 이중 따옴표 */
		return (handle_dq(line));
	if (is_rd(line[i]))
		i += rd_loop(&line[i]);
	else
		i += defloop(&line[i]);
	return (i);
}
