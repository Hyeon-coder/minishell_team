/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 13:25:42 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_meta(char c)
{
	return (c == '&' || c == ';' || c == '(' || c == ')' 
		|| c == '{' || c == '}' || c == '[' || c == ']');
}

size_t	custom_len(char *line)
{
	size_t	i;

	if (!line)
		return (0);
	i = 0;
	while (line[i])
		i++;
	return (i);
}

char	*custom_dup(char *line)
{
	size_t	i;
	char	*dup;
	size_t	len;

	if (!line)
		return (NULL);
	len = custom_len(line);
	dup = (char *)malloc((len + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = line[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
