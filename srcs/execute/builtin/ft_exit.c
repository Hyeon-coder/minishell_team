/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 01:15:34 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	argv_count(char **argv)
{
	int	count;

	count = 0;
	while (argv[count])
		count++;
	return (count);
}

static int	is_numeric_string(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	
	if (!str[i])
		return (0);
	
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	
	return (1);
}

static long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
	{
		i++;
	}
	
	while (str[i] && ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	
	return (result * sign);
}

static int	parse_numeric(const char *str, long long *result)
{
	if (!is_numeric_string(str))
		return (0);
	
	*result = ft_atoll(str);
	return (1);
}

static void	print_numeric_error(const char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

static void	print_too_many_args(void)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
}

/*
 * ft_exit_builtin
 * - is_parent: 1 when running in the parent (single command context)
 * - in_pipe  : 1 when running in a pipeline (child process context)
 *
 * Behavior:
 * - No arg           : exit with last status.
 * - One numeric arg  : exit with (unsigned char)arg.
 * - One non-numeric  : print error and exit 2 (bash behavior).
 * - >=2 args numeric : print error, do not exit, set $?=1.
 * Prints "exit" only when actually terminating the parent shell.
 */
int	ft_exit_builtin(t_shell *sh, char **argv, int is_parent, int in_pipe)
{
	int			ac;
	long long	ll;
	int			status;

	ac = argv_count(argv);
	if (ac == 1)
	{
		if (is_parent && !in_pipe)
			ft_putendl_fd("exit", 1);
		exit(sh->last_exit);
	}
	
	if (!parse_numeric(argv[1], &ll))
	{
		if (is_parent && !in_pipe)
			ft_putendl_fd("exit", 1);
		print_numeric_error(argv[1]);
		exit(2);  // bash에서는 2로 종료
	}
	
	if (ac > 2)
	{
		print_too_many_args();
		sh->last_exit = 1;
		return (1);
	}
	
	status = (unsigned char)ll;
	if (is_parent && !in_pipe)
		ft_putendl_fd("exit", 1);
	exit(status);
}
