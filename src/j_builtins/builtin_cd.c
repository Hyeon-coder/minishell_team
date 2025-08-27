/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/27 23:35:28 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * PWD와 OLDPWD 환경변수를 업데이트
 * ge_mini_shell의 로직을 minishell_team 구조에 적용
 */
static void	update_pwd_vars(t_ms *ms)
{
	char	*old_pwd;
	char	*new_pwd_path;
	char	*new_pwd_var;
	char	*old_pwd_var;

	// 현재 PWD를 OLDPWD로 저장
	old_pwd = find_var(ms, ms->envp, "PWD");
	if (old_pwd)
	{
		old_pwd_var = ft_strjoin("OLDPWD=", old_pwd);
		if (old_pwd_var)
		{
			export_var_to_envp(ms, old_pwd_var);
			free(old_pwd_var);
		}
	}

	// 새로운 PWD 설정
	new_pwd_path = getcwd(NULL, 0);
	if (new_pwd_path)
	{
		new_pwd_var = ft_strjoin("PWD=", new_pwd_path);
		if (new_pwd_var)
		{
			export_var_to_envp(ms, new_pwd_var);
			free(new_pwd_var);
		}
		free(new_pwd_path);
	}
	else
	{
		// getcwd 실패시에도 에러 출력하지만 계속 진행
		ft_putstr_fd("minishell: cd: getcwd failed: ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
	}
}

/**
 * 대상 경로 결정 (ge_mini_shell 로직 적용)
 * 인자가 없으면 HOME 환경변수 사용
 */
static char	*get_target_path(char **args, t_ms *ms)
{
	char	*home_path;

	if (!args[1])
	{
		home_path = find_var(ms, ms->envp, "HOME");
		if (!home_path)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
			return (NULL);
		}
		return (home_path);
	}
	return (args[1]);
}

/**
 * minishell_team 구조를 활용한 cd 구현
 * ge_mini_shell의 핵심 로직 유지
 */
int	builtin_cd(t_ms *ms, char **args)
{
	char	*target_path;

	target_path = get_target_path(args, ms);
	if (!target_path)
	{
		ms->exit_status = 1;
		return (1);
	}

	if (chdir(target_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(target_path, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		ms->exit_status = 1;
		return (1);
	}

	update_pwd_vars(ms);
	ms->exit_status = 0;
	return (0);
}
