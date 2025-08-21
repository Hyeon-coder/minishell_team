/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:39:34 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/21 23:24:02 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_option(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '-')
			return (1);
		i++;
	}
	return (0);
}

int	ft_env(char **argv, t_env *env_list)
{
	t_env	*tmp;

	if (argv[1])
	{
		if (check_option(argv))
			ft_putstr_fd("env: options are not supported\n", 2);
		else
		{
			ft_putstr_fd("env: '", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putstr_fd("': no arguments expected\n", 2);
		}
		return (1);
	}
	tmp = env_list;
	while (tmp)
	{
		printf("%s=%s\n", tmp->name, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
