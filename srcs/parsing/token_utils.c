/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:42:36 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 20:22:06 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_type(t_token *token)
{
	printf("[DEBUG] set_type: checking token '%s'\n", token->str);
	
	if (!ft_strcmp("echo", token->str))
		token->type = ECHO;
	else if (!ft_strcmp("pwd", token->str))
		token->type = PWD;
	else if (!ft_strcmp("exit", token->str))
		token->type = EXIT;
	else if (!ft_strcmp("|", token->str) && !token->sq && !token->dq)
	{
		printf("[DEBUG] set_type: PIPE detected! Setting type to PIPE\n");
		token->type = PIPE;
	}
	else if (!ft_strcmp("unset", token->str))
		token->type = UNSET;
	else if (!ft_strcmp("export", token->str))
		token->type = EXPORT;
	else if (!ft_strcmp("-n", token->str))
		token->type = N;
	else if (token->str[0] == '$' && !token->sq)
		setenv_type(token);
	else if (!ft_strcmp(">", token->str) && !token->sq && !token->dq)
		token->type = RD_O;
	else if (!ft_strcmp(">>", token->str) && !token->sq && !token->dq)
		token->type = RD_O_APPEND;
	else if (!ft_strcmp("<", token->str) && !token->sq && !token->dq)
		token->type = RD_I;
	else if (!ft_strcmp("<<", token->str) && !token->sq && !token->dq)
		token->type = HERE_DOC;
	else
		token_path_setter(token->str, token);
	
	printf("[DEBUG] set_type: final type = %d\n", token->type);
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
	return (i);
}

size_t	defloop(char *line)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\t')
			break ;
		else if (line[i] == '|')
			break ;
		else if (line[i] == 39)
			break ;
		else if (line[i] == 34)
			break ;
		else if (is_rd(line[i]))
			break ;
		else
			i++;
	}
	return (i);
}
