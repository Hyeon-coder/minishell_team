/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 01:17:43 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin_name(const char *name)
{
	if (!name)
		return (0);
	if (ft_strcmp(name, "echo") == 0)
		return (1);
	if (ft_strcmp(name, "cd") == 0)
		return (1);
	if (ft_strcmp(name, "pwd") == 0)
		return (1);
	if (ft_strcmp(name, "export") == 0)
		return (1);
	if (ft_strcmp(name, "unset") == 0)
		return (1);
	if (ft_strcmp(name, "env") == 0)
		return (1);
	if (ft_strcmp(name, "exit") == 0)
		return (1);
	return (0);
}

bool	is_builtin(const char *cmd)
{
	return (is_builtin_name(cmd) != 0);
}

static int	dispatch_builtin(char **argv, t_env **env_list, t_shell *sh)
{
	const char	*cmd;

	cmd = argv[0];
	if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(argv, env_list));
	else if (ft_strcmp(cmd, "echo") == 0)
		return (ft_echo(argv));
	else if (ft_strcmp(cmd, "env") == 0)
		return (ft_env(argv, *env_list));
	else if (ft_strcmp(cmd, "export") == 0)
		return (ft_export(argv, env_list));
	else if (ft_strcmp(cmd, "unset") == 0)
		return (ft_unset(argv, env_list));
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd(argv));
	else if (ft_strcmp(cmd, "exit") == 0)
		return (ft_exit_builtin(sh, argv, 1, 0));
	return (1);
}

int	handle_builtin_parent(char **argv, t_env **env_list, t_shell *sh)
{
	return (dispatch_builtin(argv, env_list, sh));
}

int	handle_builtin_child(char **argv, t_env **env_list, t_shell *sh)
{
	return (dispatch_builtin(argv, env_list, sh));
}
