/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljh3900 <ljh3900@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 08:21:02 by ljh3900           #+#    #+#             */
/*   Updated: 2025/06/11 20:42:58 by ljh3900          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	is_n_option(const char *s)
{
	int	i = 1;

	if (s[0] != '-' || s[1] != 'n')
		return (0);
	while (s[i] == 'n')
		i++;
	return (s[i] == '\0');
}

static void	print_dollar(const char *arg, t_env *env_list, int last_exit)
{
	t_env *node;

	if (arg[1] == '?' && arg[2] == '\0')
	{
		ft_putnbr_fd(last_exit, 1);
		return;
	}
	node = env_find(env_list, arg + 1);
	if (node && node->value)
		ft_putstr_fd(node->value, 1);
}

int ft_echo(char **argv, t_env *env_list, int last_exit)
{
	int	i;
	int	print_nl;

	i = 1;
	print_nl = 1;
	while (argv[i] && is_n_option(argv[i]))
	{
		print_nl = 0;
		i++;
	}
	while (argv[i])
	{
		if (argv[i][0] == '$')
			print_dollar(argv[i], env_list, last_exit);
		else
			ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (print_nl)
		ft_putchar_fd('\n', 1);
	return (0);
}
