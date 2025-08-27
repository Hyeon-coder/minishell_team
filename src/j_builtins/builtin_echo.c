/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/27 23:34:26 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * ge_mini_shell의 is_n_option 로직을 minishell_team 구조에 맞게 적용
 * -n 옵션이 유효한지 검사 (연속된 'n'만 허용)
 */
static int	is_valid_n_option(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i] == 'n')
		i++;
	if (arg[i] == '\0')
		return (1);
	return (0);
}

/**
 * minishell_team의 t_ms 구조를 활용한 echo 구현
 * ge_mini_shell의 핵심 로직을 유지하되 데이터 구조 적응
 */
int	builtin_echo(t_ms *ms, char **args)
{
	int	i;
	int	newline;

	if (!args || !args[0])
		return (0);
	
	i = 1;
	newline = 1;
	
	// -n 옵션들 처리 (ge_mini_shell 로직)
	while (args[i] && is_valid_n_option(args[i]))
	{
		newline = 0;
		i++;
	}
	
	// 인자들 출력
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	
	// 줄바꿈 처리
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	
	ms->exit_status = 0;
	return (0);
}
