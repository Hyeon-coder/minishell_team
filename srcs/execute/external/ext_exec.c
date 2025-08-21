/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ext_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/21 23:22:04 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_directory(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (0);
	return (S_ISDIR(st.st_mode));
}

static int	check_file_access(const char *path, t_shell *sh)
{
	if (access(path, F_OK) != 0)
	{
		ext_print_no_such_file(path);
		sh->last_exit = 127;
		return (-1);
	}
	if (is_directory(path))
	{
		ext_print_is_directory(path);
		sh->last_exit = 126;
		return (-1);
	}
	if (access(path, X_OK) != 0)
	{
		ext_print_perm_denied(path);
		sh->last_exit = 126;
		return (-1);
	}
	return (0);
}

void	ext_exec_direct_path(char **argv, char **envp, t_shell *sh)
{
	const char	*path;

	path = argv[0];
	if (check_file_access(path, sh) < 0)
		return ;
	ext_try_exec(path, argv, envp);
	sh->last_exit = ext_errno_to_exit(errno);
}

static int	validate_arguments(char **argv, t_shell *sh)
{
	if (!argv || !argv[0] || argv[0][0] == '\0')
	{
		sh->last_exit = 0;
		return (-1);
	}
	return (0);
}

void	run_external(char **argv, t_env *env_list, t_shell *sh)
{
	char	**envp;

	if (validate_arguments(argv, sh) < 0)
		return ;
	envp = ext_env_to_envp(env_list);
	if (!envp)
	{
		perror("minishell: malloc");
		sh->last_exit = 1;
		return ;
	}
	if (ext_has_slash(argv[0]))
		ext_exec_direct_path(argv, envp, sh);
	else
		ext_exec_with_path_search(argv, env_list, envp, sh);
	ft_free_2d_array(envp);
}
