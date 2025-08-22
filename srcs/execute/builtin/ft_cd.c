/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljh3900 <ljh3900@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:15:35 by ljh3900           #+#    #+#             */
/*   Updated: 2025/06/08 15:18:46 by ljh3900          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	cd_arg_error(char **argv)
{
	if (argv[1] && argv[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

static void	update_env(t_env **env_list, const char *key, char *value)
{
	t_env	*node;

	node = env_find(*env_list, key);
	if (node)
	{
		free(node->value);
		node->value = value;
	}
	else
	{
		node = env_new(key, value);
		env_add_back(env_list, node);
		free(value);
	}
}

static int	cd_error(const char *path, int alloc_flag, char *oldpwd)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd((char *)path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	if (oldpwd)
		free(oldpwd);
	if (alloc_flag)
		free((void *)path);
	return (1);
}

int	ft_cd(char **argv, t_env **env_list)
{
	char	*path;
	char	*oldpwd;
	char	*newpwd;
	int		alloc_flag;

	if (cd_arg_error(argv))
		return (1);
	path = resolve_path(argv[1], *env_list, &alloc_flag);
	if (!path)
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		if (alloc_flag)
			free(path);
		return (ft_putstr_fd("cd: getcwd error\n", 2), 1);
	}
	if (chdir(path) != 0)
		return (cd_error(path, alloc_flag, oldpwd));
	newpwd = getcwd(NULL, 0);
	update_env(env_list, "OLDPWD", oldpwd);
	update_env(env_list, "PWD", newpwd);
	if (alloc_flag)
		free(path);
	return (0);
}
