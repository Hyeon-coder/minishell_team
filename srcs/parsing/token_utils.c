/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:42:36 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/07 20:42:39 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_type(t_token *token)
{
	if (ftstrcmp("echo", token->str))
		token->type = ECHO;
	if (ftstrcmp("pwd", token->str))
		token->type = PWD;
	if (ftstrcmp("exit", token->str))
		token->type = EXIT;
	if (ftstrcmp("|", token->str) && !token->sq && !token->dq)
		token->type = PIPE;
	if (ftstrcmp("unset", token->str))
		token->type = UNSET;
	if (ftstrcmp("export", token->str))
		token->type = EXPORT;
	if (ftstrcmp("-n", token->str))
		token->type = N;
	if (token->str[0] == '$' && !token->sq)
		setenv_type(token);
	if (ftstrcmp(">", token->str) && !token->sq && !token->dq)
		token->type = RD_O;
	if (ftstrcmp(">>", token->str) && !token->sq && !token->dq)
		token->type = RD_O_APPEND;
	if (ftstrcmp("<", token->str) && !token->sq && !token->dq)
		token->type = RD_I;
	if (ftstrcmp("<<", token->str) && !token->sq && !token->dq)
		token->type = HERE_DOC;
	token_path_setter(token->str, token);
}

size_t	handle_rd(char *line)
{
	size_t	i;

	i = 0;
	while (is_rd(line[i]))
		i++;
	return (i);
}

size_t	rd_loop(char *line)
{
	size_t	i;

	i = 0;
	while (is_rd(line[i]))
		i++;
	if (i > 0)
		i--;
	return (i);
}

size_t	defloop(char *line)
{
	size_t	i;

	i = 0;
	while (!is_whitespace(line[i]) && line[i])
	{
		if (line[i] == 39)
			i += handle_sq(&line[i + 1]);
		if (line[i] == 34)
			i += handle_dq(&line[i + 1]);
		if (is_rd(line[i]))
			break ;
		if (line[i] == '$')
		{
			i += handle_dollar(&line[i + 1]);
			break ;
		}
		i++;
	}
	if (i > 0)
		i--;
	return (i);
}
