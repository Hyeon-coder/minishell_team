/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 02:55:02 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ftstrcmp(char *s1, char *s2)
{
	size_t	i;

	if (!s1 || !s2)
		return (false);
	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (false);
		i++;
	}
	return (s1[i] == s2[i]);
}

int	ftstrncmp(char *s1, char *s2, size_t n)
{
	size_t	i;

	if (!s1 || !s2)
		return (-1);
	i = 0;
	while (i < n && s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	if (i == n)
		return (1);
	return (s1[i] == s2[i]);
}

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
