/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 07:36:42 by ljh3900           #+#    #+#             */
/*   Updated: 2025/08/22 01:49:24 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_cleanup(char *name, char *value, t_env *node, int ret_code)
{
	if (ret_code)
		ft_putstr_fd("export: malloc failed\n", 2);
	if (node)
	{
		if (node->name)
			free(node->name);
		if (node->value)
			free(node->value);
		free(node);
	}
	if (name)
		free(name);
	if (value)
		free(value);
	return (ret_code);
}

int is_valid_name(const char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	if (!(ft_isalpha(s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void print_export_list(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(current->name, 1);
		if (current->value && current->value[0])
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(current->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		current = current->next;
	}
}

int env_remove(t_env **lst, const char *name)
{
    t_env *prev = NULL;
    t_env *cur  = *lst;

    while (cur)
    {
        if (ft_strcmp(cur->name, name) == 0)
        {
            if (prev)
                prev->next = cur->next;
            else
                *lst = cur->next;
            if (cur->name)
                free(cur->name);
            if (cur->value)
                free(cur->value);
            free(cur);
            return (1);
        }
        prev = cur;
        cur  = cur->next;
    }
    return (0);
}
