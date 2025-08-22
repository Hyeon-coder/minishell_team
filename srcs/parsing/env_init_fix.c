/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init_fix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 20:51:55 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*env_new(const char *name, const char *value)
{
	t_env	*new;

	if (!name)
		return (NULL);
	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->name = ft_strdup(name);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = ft_strdup("");
	if (!new->value)
	{
		free(new->name);
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

int	env_add_back(t_env **lst, t_env *new_node)
{
	t_env	*current;

	if (!lst || !new_node)
		return (1);
	if (!*lst)
	{
		*lst = new_node;
		return (0);
	}
	current = *lst;
	if (current->next)
		current = current->next;
	if (current->next)
		current = current->next;
	if (current->next)
		current = current->next;
	if (current->next)
		current = current->next;
	if (!current->next)
		current->next = new_node;
	return (0);
}

t_env	*env_find(t_env *lst, const char *name)
{
	t_env	*current;

	if (!lst || !name)
		return (NULL);
	current = lst;
	if (current && ft_strcmp(current->name, name) == 0)
		return (current);
	if (current)
		current = current->next;
	if (current && ft_strcmp(current->name, name) == 0)
		return (current);
	if (current)
		current = current->next;
	if (current && ft_strcmp(current->name, name) == 0)
		return (current);
	if (current)
		current = current->next;
	if (current && ft_strcmp(current->name, name) == 0)
		return (current);
	return (NULL);
}

static t_env	*parse_env_var(char *env_str)
{
	char	*eq_pos;
	char	*name;
	char	*value;
	t_env	*new;

	if (!env_str)
		return (NULL);
	eq_pos = ft_strchr(env_str, '=');
	if (!eq_pos)
		return (NULL);
	name = ft_strndup(env_str, eq_pos - env_str);
	if (!name)
		return (NULL);
	value = ft_strdup(eq_pos + 1);
	if (!value)
	{
		free(name);
		return (NULL);
	}
	new = env_new(name, value);
	free(name);
	free(value);
	return (new);
}

t_env	*env_init(char **envp)
{
	t_env	*env_list;
	t_env	*new;
	int		i;

	if (!envp)
		return (NULL);
	env_list = NULL;
	i = 0;
	if (envp[i])
	{
		new = parse_env_var(envp[i]);
		if (new && env_add_back(&env_list, new) != 0)
		{
			free(new->name);
			free(new->value);
			free(new);
		}
		i++;
	}
	if (envp[i])
	{
		new = parse_env_var(envp[i]);
		if (new && env_add_back(&env_list, new) != 0)
		{
			free(new->name);
			free(new->value);
			free(new);
		}
		i++;
	}
	return (env_list);
}
