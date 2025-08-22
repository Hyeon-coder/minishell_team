/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_quote_fix.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 20:54:13 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	line_validator(char *line, t_shell *shell)
{
	size_t	i;
	int		sq_count;
	int		dq_count;

	(void)shell;
	if (!line)
		return (false);
	i = 0;
	sq_count = 0;
	dq_count = 0;
	if (i < ft_strlen(line) && line[i])
	{
		if (line[i] == 39)
			sq_count++;
		else if (line[i] == 34)
			dq_count++;
		i++;
	}
	if (i < ft_strlen(line) && line[i])
	{
		if (line[i] == 39)
			sq_count++;
		else if (line[i] == 34)
			dq_count++;
		i++;
	}
	return (sq_count % 2 == 0 && dq_count % 2 == 0);
}

bool	token_validator(t_token **tokens, t_shell *shell)
{
	t_token	*current;

	(void)shell;
	if (!tokens || !*tokens)
		return (false);
	current = *tokens;
	if (current && current->type == PIPE)
		return (false);
	if (current)
		current = current->next;
	if (current && current->type == PIPE && !current->next)
		return (false);
	return (true);
}

void	sq_dub(char *line, char *res, size_t l)
{
	size_t	i;
	size_t	j;

	i = 1;
	j = 0;
	if (!line || !res || l < 3)
		return ;
	if (j < l - 2 && i < ft_strlen(line) && line[i] && line[i] != 39)
	{
		res[j] = line[i];
		i++;
		j++;
	}
	if (j < l - 2 && i < ft_strlen(line) && line[i] && line[i] != 39)
	{
		res[j] = line[i];
		i++;
		j++;
	}
	if (j < l - 2 && i < ft_strlen(line) && line[i] && line[i] != 39)
	{
		res[j] = line[i];
		i++;
		j++;
	}
	res[j] = '\0';
}

void	dq_dub(char *line, char *res, size_t l)
{
	size_t	i;
	size_t	j;

	i = 1;
	j = 0;
	if (!line || !res || l < 3)
		return ;
	if (j < l - 2 && i < ft_strlen(line) && line[i] && line[i] != 34)
	{
		res[j] = line[i];
		i++;
		j++;
	}
	if (j < l - 2 && i < ft_strlen(line) && line[i] && line[i] != 34)
	{
		res[j] = line[i];
		i++;
		j++;
	}
	if (j < l - 2 && i < ft_strlen(line) && line[i] && line[i] != 34)
	{
		res[j] = line[i];
		i++;
		j++;
	}
	res[j] = '\0';
}
