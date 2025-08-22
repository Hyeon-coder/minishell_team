/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 21:11:44 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 20:26:30 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	setenv_type(t_token *t)
// {
// 	if (!ft_strncmp("$?", t->str, 2))
// 		t->type = LSIG;
// 	else
// 		t->type = EV;
// }

size_t	handle_sq(char *line)
{
	size_t	i;

	i = 1;
	while (line[i] != 39 && line[i])
		i++;
	i++;
	return (i);
}

size_t	handle_dq(char *line)
{
	size_t	i;

	i = 1;
	while (line[i] != 34 && line[i])
		i++;
	i++;
	return (i);
}

bool	is_rd(char c)
{
	return ((c == '>' || c == '<'));
}

size_t	handle_dollar(char *line)
{
	size_t	i;

	if (!line)
		return (0);
	i = 0;
	if (line[i] == '?')
		return (1);
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	return (i);
}
