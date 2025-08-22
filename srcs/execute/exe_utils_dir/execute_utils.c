/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:05:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/21 01:07:18 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/* Core implementation that knows the execution context. */
/* Will terminate here; does not return on success/error except
 * for "too many arguments" case where it returns 1. */
static int	handle_builtin_core(char **argv, t_env **env_list,
		t_shell *sh, int is_parent, int in_pipe)
{
	int			exit_code;
	const char	*cmd;

	exit_code = 0;
	cmd = argv[0];
	if (ft_strcmp(cmd, "cd") == 0)
		exit_code = ft_cd(argv, env_list);
	else if (ft_strcmp(cmd, "echo") == 0)
		exit_code = ft_echo(argv);
	else if (ft_strcmp(cmd, "env") == 0)
		exit_code = ft_env(argv, *env_list);
	else if (ft_strcmp(cmd, "export") == 0)
		exit_code = ft_export(argv, env_list);
	else if (ft_strcmp(cmd, "unset") == 0)
		exit_code = ft_unset(argv, env_list);
	else if (ft_strcmp(cmd, "pwd") == 0)
		exit_code = ft_pwd(argv);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (ft_exit_builtin(sh, argv, is_parent, in_pipe));
	return (exit_code);
}

/* Wrapper for parent(single) context. */
int	handle_builtin_parent(char **argv, t_env **env_list, t_shell *sh)
{
	return (handle_builtin_core(argv, env_list, sh, 1, 0));
}

/* Wrapper for child(pipeline) context. */
int	handle_builtin_child(char **argv, t_env **env_list, t_shell *sh)
{
	return (handle_builtin_core(argv, env_list, sh, 0, 1));
}

/* Legacy wrapper kept for compatibility (parent by default). */
int	handle_builtin(char **argv, t_env **env_list, t_shell *sh)
{
	return (handle_builtin_core(argv, env_list, sh, 1, 0));
}
