/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/28 00:58:18 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * ge_mini_shell의 is_builtin 로직을 minishell_team 구조에 적용
 * 명령어가 빌트인인지 확인
 */
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	
	return (0);
}

/**
 * ge_mini_shell의 execute_builtin 로직을 minishell_team 구조에 적용
 * 실제 빌트인 명령어 실행
 */
int	execute_builtin(t_ms *ms, char **args)
{
	char	*cmd;

	if (!args || !args[0])
		return (127);
	
	cmd = args[0];
	
	if (ft_strcmp(cmd, "echo") == 0)
		return (builtin_echo(ms, args));
	if (ft_strcmp(cmd, "cd") == 0)
		return (builtin_cd(ms, args));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (builtin_pwd(ms));
	if (ft_strcmp(cmd, "export") == 0)
		return (builtin_export(ms, args));
	if (ft_strcmp(cmd, "unset") == 0)
		return (builtin_unset(ms, args));
	if (ft_strcmp(cmd, "env") == 0)
		return (builtin_env(ms));
	if (ft_strcmp(cmd, "exit") == 0)
		return (builtin_exit(ms, args));
	
	return (127); // command not found
}

/**
 * t_cmd 구조체를 사용하는 버전의 execute_builtin
 * minishell_team 구조에 맞춤
 */
int	execute_builtin_cmd(t_ms *ms, t_cmd *cmd)
{
	if (!cmd || !cmd->full_cmd || !cmd->full_cmd[0])
		return (127);
	
	return (execute_builtin(ms, cmd->full_cmd));
}

int	builtin_env(t_ms *ms)
{
	int		i;
	int		j;
	bool	has_value;

	if (!ms || !ms->envp)
		return (1);

	i = 0;
	while (ms->envp[i])
	{
		j = 0;
		has_value = false;
		
		// = 문자가 있는지 확인 (값이 있는 환경변수만 출력)
		while (ms->envp[i][j])
		{
			if (ms->envp[i][j] == '=')
			{
				has_value = true;
				break;
			}
			j++;
		}
		
		if (has_value)
			ft_putendl_fd(ms->envp[i], STDOUT_FILENO);
		i++;
	}
	
	ms->exit_status = 0;
	return (0);
}
