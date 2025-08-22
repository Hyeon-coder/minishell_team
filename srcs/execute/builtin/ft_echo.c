/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 04:18:17 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	if (arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(char **argv)
{
	int	i;
	int	newline;

	if (!argv || !argv[0])
		return (1);

	newline = 1;
	i = 1;
	
	/* -n 옵션 처리 */
	while (argv[i] && is_n_flag(argv[i]))
	{
		newline = 0;
		i++;
	}
	
	/* 인자들 출력 */
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	
	/* 개행 출력 */
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	
	/* 출력 버퍼 플러시 */
	if (fflush(stdout) != 0)
		return (1);
		
	return (0);
}
