/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ext_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 02:59:55 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ext_exec_direct_path(char **argv, char **envp, t_shell *sh)
{
	int		exec_errno;
	struct stat	path_stat;

	if (stat(argv[0], &path_stat) != 0)
	{
		ext_print_no_such_file(argv[0]);
		sh->last_exit = 127;
		return ;
	}
	
	if (S_ISDIR(path_stat.st_mode))
	{
		ext_print_is_directory(argv[0]);
		sh->last_exit = 126;
		return ;
	}
	
	if (access(argv[0], X_OK) != 0)
	{
		ext_print_perm_denied(argv[0]);
		sh->last_exit = 126;
		return ;
	}
	
	exec_errno = ext_try_exec(argv[0], argv, envp);
	sh->last_exit = ext_errno_to_exit(exec_errno);
	
	if (exec_errno == EACCES)
		ext_print_perm_denied(argv[0]);
	else
		ext_print_no_such_file(argv[0]);
}

static char	*build_full_path(const char *dir, const char *cmd)
{
	size_t	dir_len;
	size_t	cmd_len;
	char	*full_path;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
	
	ft_strcpy(full_path, dir);
	if (dir_len > 0 && full_path[dir_len - 1] != '/')
		ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	
	return (full_path);
}

static int	try_path_exec(char **argv, char **envp, char *full_path)
{
	if (access(full_path, F_OK) == 0)
	{
		if (access(full_path, X_OK) == 0)
			return (ext_try_exec(full_path, argv, envp));
		else
			return (EACCES);
	}
	return (ENOENT);
}

void	ext_exec_with_path_search(char **argv, t_env *env_list, char **envp, t_shell *sh)
{
	char	*path_var;
	char	**paths;
	char	*full_path;
	int		exec_errno;
	int		i;

	path_var = ext_get_path_value(env_list);
	if (!path_var)
	{
		ext_print_cmd_not_found(argv[0]);
		sh->last_exit = 127;
		return ;
	}
	
	paths = ext_split_paths(path_var);
	if (!paths)
	{
		sh->last_exit = 1;
		return ;
	}
	
	i = 0;
	while (paths[i])
	{
		full_path = build_full_path(paths[i], argv[0]);
		if (full_path)
		{
			exec_errno = try_path_exec(argv, envp, full_path);
			free(full_path);
			if (exec_errno != ENOENT)
			{
				ft_free_2d_array(paths);
				sh->last_exit = ext_errno_to_exit(exec_errno);
				if (exec_errno == EACCES)
					ext_print_perm_denied(argv[0]);
				return ;
			}
		}
		i++;
	}
	
	ft_free_2d_array(paths);
	ext_print_cmd_not_found(argv[0]);
	sh->last_exit = 127;
}
