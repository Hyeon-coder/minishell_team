/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ext_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/21 23:22:14 by juhyeonl         ###   ########.fr       */
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

static char	*join_path(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full);
}

static int	try_execute_path(char *full, char **argv, char **envp, t_shell *sh)
{
	if (access(full, F_OK) == 0 && !is_directory(full))
	{
		if (access(full, X_OK) == 0)
		{
			ext_try_exec(full, argv, envp);
			sh->last_exit = ext_errno_to_exit(errno);
			return (1);
		}
		return (0);
	}
	return (-1);
}

static void	handle_search_result(int saw_perm, char **argv, t_shell *sh)
{
	if (saw_perm)
	{
		ext_print_perm_denied(argv[0]);
		sh->last_exit = 126;
	}
	else
	{
		ext_print_cmd_not_found(argv[0]);
		sh->last_exit = 127;
	}
}

void	ext_exec_with_path_search(char **argv, t_env *env_list,
			char **envp, t_shell *sh)
{
	char	*pathvar;
	char	**dirs;
	char	*full;
	int		i;
	int		saw_perm;
	int		result;

	pathvar = ext_get_path_value(env_list);
	if (!pathvar || !*pathvar)
	{
		ext_print_cmd_not_found(argv[0]);
		sh->last_exit = 127;
		return ;
	}
	dirs = ext_split_paths(pathvar);
	if (!dirs)
	{
		sh->last_exit = 1;
		return ;
	}
	i = 0;
	saw_perm = 0;
	while (dirs[i])
	{
		full = join_path(dirs[i], argv[0]);
		if (full)
		{
			result = try_execute_path(full, argv, envp, sh);
			if (result == 1)
			{
				free(full);
				ft_free_2d_array(dirs);
				return ;
			}
			else if (result == 0)
				saw_perm = 1;
			free(full);
		}
		i++;
	}
	ft_free_2d_array(dirs);
	handle_search_result(saw_perm, argv, sh);
}
