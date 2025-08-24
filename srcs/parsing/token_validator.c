/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* token_validator.c                                  :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/07 20:56:42 by mhurtamo          #+#    #+#             */
/* Updated: 2025/08/23 06:37:25 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	check_if_exists(t_token *token, t_shell *shell)
{
	bool	ret;

	if (token->type != PATH && token->type != REL_PATHF)
		return (true);
	if (token->type == PATH)
		ret = is_valid_dir(token->str, shell);
	else
		ret = is_valid_file(token, shell);
	return (ret);
}

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
	if (ftstrcmp(token->str, "|") || ftstrcmp(token->str, ">>"))
		return (true);
	if (ftstrcmp(token->str, "<") || ftstrcmp(token->str, ">"))
		return (true);
	return (false);
}

bool	token_validator(t_token **tokens, t_shell *shell)
{
	t_token	*token;
	bool	not_present;

	if (!*tokens)
		return (false);
	token = *tokens;
	not_present = true;
	while (token && not_present)
	{
		if (token->type == PIPE)
		{
			// 파이프가 명령의 시작이나 끝에 오면 오류
			if (!token->prev || !token->next)
				not_present = false;
			// 파이프 뒤에 다른 파이프가 오면 오류
			else if (token->next->type == PIPE)
				not_present = false;
		}
		else if (token->type == RD_I || token->type == RD_O ||
		         token->type == RD_O_APPEND || token->type == HERE_DOC)
		{
			// 리다이렉션 뒤에 아무것도 없거나 다른 리다이렉션이 오면 오류
			if (!token->next || is_pipe_or_rd(token->next))
				not_present = false;
		}
		
		token = token->next;
	}
	if (!not_present)
		write_syntax_error("syntax error", shell);
	return (not_present);
}

size_t	increment_index(char *line)
{
	size_t	i;

	i = 0;
	if (!line)
		return (i);
	if (is_rd(line[i]))
		i += rd_loop(&line[i]);
	else
		i += defloop(&line[i]);
	return (i);
}
