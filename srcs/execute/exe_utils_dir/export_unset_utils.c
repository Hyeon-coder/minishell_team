/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljh3900 <ljh3900@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 07:36:42 by ljh3900           #+#    #+#             */
/*   Updated: 2025/06/08 07:47:49 by ljh3900          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	export_cleanup(char *name, char *value, t_env *node, int ret_code)
{
	if (ret_code)
		ft_putstr_fd("export: malloc failed\n", 2);
	if (node)
	{
		free(node->name);
		free(node->value);
		free(node);
	}
	free(name);
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
	while (env)
	{
		if (env->value && env->value[0])
			printf("%s=%s\n", env->name, env->value);
		else
			printf("%s\n", env->name);
		env = env->next;
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
            free(cur->name);
            free(cur->value);
            free(cur);
            return (1);
        }
        prev = cur;
        cur  = cur->next;
    }
    return (0);
}