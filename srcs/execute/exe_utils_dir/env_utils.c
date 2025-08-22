/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 03:00:37 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_env	*env_new(const char *name, const char *value)
{
	t_env	*new;

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

t_env	*env_init(char **envp)
{
	t_env	*head;
	t_env	*current;
	char	*eq_pos;
	char	*name;
	char	*value;
	int		i;

	if (!envp || !envp[0])
		return (NULL);
	head = NULL;
	i = 0;
	while (envp[i])
	{
		eq_pos = ft_strchr(envp[i], '=');
		if (eq_pos)
		{
			name = ft_substr(envp[i], 0, eq_pos - envp[i]);
			value = ft_strdup(eq_pos + 1);
		}
		else
		{
			name = ft_strdup(envp[i]);
			value = ft_strdup("");
		}
		current = env_new(name, value);
		free(name);
		free(value);
		if (current)
			env_add_back(&head, current);
		i++;
	}
	return (head);
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
	while (current->next)
		current = current->next;
	current->next = new_node;
	return (0);
}

void	env_clear(t_env **lst)
{
	t_env	*current;
	t_env	*next;

	if (!lst || !*lst)
		return ;
	current = *lst;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
	*lst = NULL;
}

t_env	*env_find(t_env *lst, const char *name)
{
	t_env	*current;

	if (!lst || !name)
		return (NULL);
	current = lst;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}
