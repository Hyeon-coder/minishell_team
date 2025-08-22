/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_resolve_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 12:03:24 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*resolve_path(char *arg, t_env *env_list, int *alloc_flag)
{
	t_env	*home_env;
	char	*path;

	*alloc_flag = 0;
	if (!arg)
	{
		home_env = find_env("HOME", &env_list);
		if (!home_env || !home_env->value)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (NULL);
		}
		return (home_env->value);
	}
	if (ft_strcmp(arg, "~") == 0)
	{
		home_env = find_env("HOME", &env_list);
		if (!home_env || !home_env->value)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (NULL);
		}
		return (home_env->value);
	}
	if (ft_strncmp(arg, "~/", 2) == 0)
	{
		home_env = find_env("HOME", &env_list);
		if (!home_env || !home_env->value)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (NULL);
		}
		path = ft_strjoin(home_env->value, arg + 1);
		if (!path)
			return (NULL);
		*alloc_flag = 1;
		return (path);
	}
	return (arg);
}
