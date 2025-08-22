/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 21:11:44 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/07 21:11:47 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	setenv_type(t_token *t)
{
	if (ftstrncmp("$?", t->str, 2))
		t->type = LSIG;
	else
		t->type = EV;
}

int	is_whitespace(char c)
{
	return ((c == ' ' || c == 9));
}

size_t	handle_sq(char *line)
{
	size_t	i;

	i = 0;
	while (line[i] != 39 && line[i])
		i++;
	i++;
	return (i);
}

size_t	handle_dq(char *line)
{
	size_t	i;

	i = 0;
	while (line[i] != 34 && line[i])
		i++;
	i++;
	return (i);
}

bool	is_rd(char c)
{
	return ((c == '>' || c == '<'));
}
