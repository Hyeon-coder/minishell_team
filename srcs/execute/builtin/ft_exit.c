/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/16 00:34:48 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

/* Count argv items (NULL-terminated). */
static int	argv_count(char **argv)
{
	int	n;

	n = 0;
	while (argv && argv[n])
		n++;
	return (n);
}

/* Parse signed integer into long long with basic overflow guard. */
static int	parse_numeric(const char *s, long long *out)
{
	long long	acc;
	int			sign;
	size_t		i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	sign = 1;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	if (s[i] < '0' || s[i] > '9')
		return (0);
	acc = 0;
	while (s[i] >= '0' && s[i] <= '9')
	{
		if (acc > LLONG_MAX / 10
			|| (acc == LLONG_MAX / 10 && (s[i] - '0') > (LLONG_MAX % 10)))
			return (0);
		acc = acc * 10 + (s[i] - '0');
		i++;
	}
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] != '\0')
		return (0);
	*out = acc * sign;
	return (1);
}

/* Print "numeric argument required" to stderr. */
static void	print_numeric_error(const char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

/* Print "too many arguments" to stderr. */
static void	print_too_many_args(void)
{
	ft_putendl_fd("minishell: exit: too many arguments", 2);
}

/*
 * ft_exit_builtin
 * - is_parent: 1 when running in the parent (single command context)
 * - in_pipe  : 1 when running in a pipeline (child process context)
 *
 * Behavior:
 * - No arg           : exit with last status.
 * - One numeric arg  : exit with (unsigned char)arg.
 * - One non-numeric  : print error and exit 255.
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
		exit(255);
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
