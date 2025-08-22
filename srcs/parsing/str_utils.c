/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 21:02:35 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/23 02:35:57 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ftstrcmp(char *s1, char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (false);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (false);
		i++;
	}
	return (true);
}

int	ftstrncmp(char *s1, char *s2, size_t n)
{
	size_t	i;

	if (!s1 || !s2 || n == 0)
		return (0);
		
	i = 0;
	while (i < n && s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	
	if (i < n)
		return (s1[i] - s2[i]);
		
	return (0);
}

bool	is_meta(char c)
{
	if (c == 92)
		return (true);
	if (c == ';')
		return (true);
	if (c == '&')
		return (true);
	return (false);
}

size_t	custom_len(char *line)
{
	size_t	i;

	if (!line)
		return (0);
	i = 0;
	while (line[i] && !is_whitespace(line[i]))
	{
		if (line[i] == 39)
			i += handle_sq(&line[i + 1]);
		if (line[i] == 34)
			i += handle_dq(&line[i + 1]);
		i++;
	}
	return (i);
}

char	*custom_dup(char *line)
{
	size_t	i;
	char	*res;
	size_t	l;

	if (!line)
		return (ft_strdup(""));
	
	l = custom_len(line);
	if (l == 0)
		return (ft_strdup(""));
	
	res = (char *)malloc((l + 1) * sizeof(char));
	if (!res)
		return (NULL);
	
	i = 0;
	while (i < l && line[i])
	{
		res[i] = line[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
