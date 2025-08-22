/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 04:11:40 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	is_valid_number(char *str)
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

static long	ft_atol_safe(char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		if (result > (LONG_MAX - (str[i] - '0')) / 10)
		{
			if (sign == 1)
				return (LONG_MAX);
			else
				return (LONG_MIN);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static void	exit_with_error(char *arg, t_shell *sh)
{
	ft_putstr_fd("exit\n", 2);
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	env_clear(&sh->envs);
	exit(2);
}

static void	exit_too_many_args(void)
{
	ft_putstr_fd("exit\n", 2);
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
}

int	ft_exit_builtin(t_shell *sh, char **argv, int is_parent, int in_pipe)
{
	long	exit_code;
	int		final_exit_code;

	/* 테스터 모드에서는 "exit" 출력하지 않음 */
	if (is_parent && !in_pipe && isatty(fileno(stdin)))
		ft_putstr_fd("exit\n", 2);
	
	if (!argv[1])
	{
		final_exit_code = sh->last_exit;
		if (is_parent && !in_pipe)
		{
			env_clear(&sh->envs);
			exit(final_exit_code);
		}
		return (final_exit_code);
	}
	
	if (!is_valid_number(argv[1]))
	{
		if (is_parent && !in_pipe)
			exit_with_error(argv[1], sh);
		else
			return (2);
	}
	
	if (argv[2])
	{
		if (is_parent && !in_pipe)
			exit_too_many_args();
		return (1);
	}
	
	exit_code = ft_atol_safe(argv[1]);
	final_exit_code = (int)(exit_code % 256);
	if (final_exit_code < 0)
		final_exit_code = 256 + final_exit_code;
	
	if (is_parent && !in_pipe)
	{
		env_clear(&sh->envs);
		exit(final_exit_code);
	}
	return (final_exit_code);
}
