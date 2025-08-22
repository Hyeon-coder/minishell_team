/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   missing_utility_functions.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 12:00:00 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	i;

	if (!s)
		return (NULL);
	dup = (char *)malloc((n + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

bool	is_separator(char c)
{
	return (c == ' ' || is_whitespace(c) || is_meta(c) || c == '\0');
}

void	ft_free_2d_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	err_with_cmd(char *prefix, char *cmd, char *suffix)
{
	if (prefix)
		ft_putstr_fd(prefix, 2);
	if (cmd)
		ft_putstr_fd(cmd, 2);
	if (suffix)
		ft_putstr_fd(suffix, 2);
}

char	*resolve_path(char *path, t_env *env_list, int *alloc_flag)
{
	t_env	*home;

	*alloc_flag = 0;
	if (!path)
	{
		home = env_find(env_list, "HOME");
		if (home && home->value)
		{
			*alloc_flag = 1;
			return (ft_strdup(home->value));
		}
		return (NULL);
	}
	return (path);
}

int	is_valid_name(const char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	export_cleanup(char *name, char *value, t_env *node, int exit_code)
{
	if (name)
		free(name);
	if (value)
		free(value);
	if (node)
	{
		if (node->name)
			free(node->name);
		if (node->value)
			free(node->value);
		free(node);
	}
	return (exit_code);
}

void	print_export_list(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		printf("declare -x %s", current->name);
		if (current->value && current->value[0])
			printf("=\"%s\"", current->value);
		printf("\n");
		current = current->next;
	}
}

char	*get_env_value(char *name, t_env *env_list)
{
	t_env	*node;

	node = env_find(env_list, name);
	if (node)
		return (node->value);
	return (NULL);
}
