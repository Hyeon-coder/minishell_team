/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/27 23:36:18 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * ge_mini_shell의 유효한 unset 식별자 검사 로직
 * = 가 포함되면 안됨 (export와 다른 점)
 */
static int	is_valid_unset_identifier(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	
	// 첫 문자는 알파벳 또는 언더스코어
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	
	i = 1;
	// 전체 문자열 검사 (= 있으면 안됨)
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/**
 * ge_mini_shell 로직을 적용한 unset 구현
 * minishell_team의 unset_envp 함수 활용
 */
int	builtin_unset(t_ms *ms, char **args)
{
	int	i;
	int	status;

	if (!args[1])
	{
		// 인자가 없어도 에러는 아님
		ms->exit_status = 0;
		return (0);
	}

	status = 0;
	i = 1;
	while (args[i])
	{
		// 유효성 검사
		if (!is_valid_unset_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			status = 1;
		}
		else
		{
			// minishell_team의 함수를 활용하여 실제 unset 실행
			unset_envp(ms, args[i]);
		}
		i++;
	}

	ms->exit_status = status;
	return (status);
}
