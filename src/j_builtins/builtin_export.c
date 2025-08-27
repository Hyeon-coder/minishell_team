/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/27 23:35:41 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * ge_mini_shell의 유효한 식별자 검사 로직
 * minishell_team에도 비슷한 함수가 있지만 ge_mini_shell 로직 사용
 */
static int	is_valid_export_identifier(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	
	// 첫 문자는 알파벳 또는 언더스코어
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	
	i = 1;
	// = 전까지 확인 (= 없으면 끝까지)
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/**
 * 환경변수 출력 (정렬된 형태)
 * minishell_team의 export_print_env 함수 활용
 */
static void	print_sorted_env(t_ms *ms)
{
	// minishell_team의 기존 함수 활용
	export_print_env(ms);
}

/**
 * 단일 변수 처리
 * ge_mini_shell의 로직을 minishell_team 구조에 적용
 */
static int	process_single_var(t_ms *ms, char *arg)
{
	char	*eq_pos;
	char	*var_name;
	int		status;

	// = 위치 찾기
	eq_pos = ft_strchr(arg, '=');
	if (eq_pos)
	{
		// 변수명만 추출해서 유효성 검사
		var_name = ft_substr(arg, 0, eq_pos - arg);
		if (!var_name)
		{
			ms->exit_status = 1;
			return (1);
		}
	}
	else
	{
		// = 없는 경우 전체가 변수명
		var_name = ft_strdup(arg);
		if (!var_name)
		{
			ms->exit_status = 1;
			return (1);
		}
	}

	// 유효성 검사
	if (!is_valid_export_identifier(var_name))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		free(var_name);
		return (1);
	}

	free(var_name);
	
	// minishell_team의 함수 활용하여 실제 export 실행
	status = export_var_to_envp(ms, arg) ? 0 : 1;
	return (status);
}

/**
 * ge_mini_shell 로직을 적용한 export 구현
 * minishell_team의 기존 유틸리티 함수들 활용
 */
int	builtin_export(t_ms *ms, char **args)
{
	int	i;
	int	status;

	if (!args[1])
	{
		// 인자 없으면 정렬된 환경변수 출력
		print_sorted_env(ms);
		ms->exit_status = 0;
		return (0);
	}

	status = 0;
	i = 1;
	while (args[i])
	{
		// 각 인자를 개별적으로 처리
		if (process_single_var(ms, args[i]) != 0)
			status = 1;
		i++;
	}

	ms->exit_status = status;
	return (status);
}
