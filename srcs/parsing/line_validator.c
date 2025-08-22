/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_validator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 02:55:18 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	check_sq(char *line)
{
	size_t	i;
	bool	open;

	i = 0;
	open = false;
	while (line[i])
	{
		if (line[i] == 39 && !open)
			open = true;
		else if (line[i] == 39 && open)
			open = false;
		i++;
	}
	return (!open);
}

bool	check_dq(char *line)
{
	size_t	i;
	bool	open;

	i = 0;
	open = false;
	while (line[i])
	{
		if (line[i] == 34 && !open)
			open = true;
		else if (line[i] == 34 && open)
			open = false;
		i++;
	}
	return (!open);
}

bool	q_check_handler(char *line, char q)
{
	if (q == 39)
		return (check_sq(line));
	else if (q == 34)
		return (check_dq(line));
	return (true);
}

size_t	q_count_handler(char *line, char q)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == q)
			count++;
		i++;
	}
	return (count);
}

bool	line_validator(char *line, t_shell *shell)
{
	if (!line || !*line)
		return (false);
	if (!check_sq(line))
	{
		write_syntax_error("minishell: unclosed single quote", shell);
		return (false);
	}
	if (!check_dq(line))
	{
		write_syntax_error("minishell: unclosed double quote", shell);
		return (false);
	}
	return (true);
}
