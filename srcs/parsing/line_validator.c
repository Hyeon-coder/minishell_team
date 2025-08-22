/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_validator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 21:08:38 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/07 21:08:41 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	check_sq(char *line)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == 39)
			return (true);
		i++;
	}
	return (false);
}

bool	check_dq(char *line)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == 34)
			return (true);
		i++;
	}
	return (false);
}

bool	q_check_handler(char *line, char q)
{
	bool	ret;

	if (q == 34)
		ret = check_dq(line);
	else
		ret = check_sq(line);
	return (ret);
}

size_t	q_count_handler(char *line, char q)
{
	size_t	ret;

	if (q == 39)
		ret = handle_sq(line);
	else
		ret = handle_dq(line);
	return (ret);
}

bool	line_validator(char *line)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == 39 || line[i] == 34)
		{
			if (!q_check_handler(&line[i + 1], line[i]))
				return (false);
			i += q_count_handler(&line[i + 1], line[i]);
		}
		i++;
	}
	return (true);
}
