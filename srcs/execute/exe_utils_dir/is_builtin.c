/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 01:13:12 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/16 01:13:19 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/* Return 1 if s is a builtin name, else 0. */
int	is_builtin_name(const char *s)
{
	if (!s)
		return (0);
	if (ft_strcmp(s, "echo") == 0)
		return (1);
	if (ft_strcmp(s, "cd") == 0)
		return (1);
	if (ft_strcmp(s, "pwd") == 0)
		return (1);
	if (ft_strcmp(s, "export") == 0)
		return (1);
	if (ft_strcmp(s, "unset") == 0)
		return (1);
	if (ft_strcmp(s, "env") == 0)
		return (1);
	if (ft_strcmp(s, "exit") == 0)
		return (1);
	return (0);
}

/* Boolean wrapper kept for historical call sites. */
bool	is_builtin(const char *cmd)
{
	return (is_builtin_name(cmd) != 0);
}
