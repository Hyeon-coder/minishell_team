/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/21 23:23:57 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_n_option(const char *s)
{
	int	i;

	i = 1;
	if (s[0] != '-' || s[1] != 'n')
		return (0);
	while (s[i] == 'n')
		i++;
	return (s[i] == '\0');
}

static int	check_n_options(char **argv, int *i)
{
	int	print_nl;

	*i = 1;
	print_nl = 1;
	while (argv[*i] && is_n_option(argv[*i]))
	{
		print_nl = 0;
		(*i)++;
	}
	return (print_nl);
}

static void	print_arguments(char **argv, int start_index)
{
	int	i;

	i = start_index;
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
}

int	ft_echo(char **argv)
{
	int	i;
	int	print_nl;

	print_nl = check_n_options(argv, &i);
	print_arguments(argv, i);
	if (print_nl)
		ft_putchar_fd('\n', 1);
	return (0);
}
