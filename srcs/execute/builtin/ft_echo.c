/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 08:21:02 by ljh3900           #+#    #+#             */
/*   Updated: 2025/08/23 04:36:00 by juhyeonl         ###   ########.fr       */
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

int ft_echo(char **argv, t_env *env_list, int last_exit)
{
	int	i;
	int	print_nl;

	(void)env_list;    // 환경변수 처리는 이제 parsing 단계에서 완료됨
	(void)last_exit;   // $? 처리도 parsing 단계에서 완료됨
	
	i = 1;
	print_nl = 1;
	while (argv[i] && is_n_option(argv[i]))
	{
		print_nl = 0;
		i++;
	}
	while (argv[i])
	{
		/* 모든 인자를 그대로 출력 - 환경변수 확장은 이미 parsing에서 완료됨 */
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (print_nl)
		ft_putchar_fd('\n', 1);
	return (0);
}
