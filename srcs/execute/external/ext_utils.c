/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ext_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/21 23:22:20 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ext_has_slash(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		if (s[i++] == '/')
			return (1);
	return (0);
}

char	*ext_get_path_value(t_env *env_list)
{
	t_env	*node;

	node = env_find(env_list, "PATH");
	if (node && node->value)
		return (node->value);
	return (getenv("PATH"));
}

char	**ext_split_paths(const char *pathvar)
{
	if (!pathvar)
		return (NULL);
	return (ft_split(pathvar, ':'));
}

int	ext_errno_to_exit(int e)
{
	if (e == EACCES)
		return (126);
	if (e == ENOENT)
		return (127);
	return (126);
}

int	ext_try_exec(const char *path, char **argv, char **envp)
{
	if (!path)
		return (ENOENT);
	execve(path, argv, envp);
	return (errno);
}
