/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_resolve_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:01:32 by ljh3900           #+#    #+#             */
/*   Updated: 2025/08/16 00:57:27 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static char	*cd_home(t_env *env_list)
{
	t_env	*node;

	node = env_find(env_list, "HOME");
	if (!node || !node->value)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return (NULL);
	}
	return (node->value);
}

static char	*cd_tilde(char *arg, t_env *env_list, int *alloc_flag)
{
	t_env	*node;
	char	*joined;

	node = env_find(env_list, "HOME");
	if (!node || !node->value)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return (NULL);
	}
	joined = ft_strjoin(node->value, arg + 1);
	if (!joined)
	{
		ft_putstr_fd("cd: malloc error\n", 2);
		return (NULL);
	}
	*alloc_flag = 1;
	return (joined);
}

static char	*cd_dash(t_env *env_list)
{
	t_env	*node;

	node = env_find(env_list, "OLDPWD");
	if (!node || !node->value)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		return (NULL);
	}
	ft_putstr_fd(node->value, 1);
	ft_putchar_fd('\n', 1);
	return (node->value);
}

char	*resolve_path(char *arg, t_env *env_list, int *alloc_flag)
{
	*alloc_flag = 0;
	if (!arg || (arg[0] == '~' && arg[1] == '\0'))
		return (cd_home(env_list));
	if (ft_strncmp(arg, "~/", 2) == 0)
		return (cd_tilde(arg, env_list, alloc_flag));
	if (ft_strcmp(arg, "-") == 0)
		return (cd_dash(env_list));
	return (arg);
}
