/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 18:19:49 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/21 23:22:52 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env  *env_new(const char *name, const char *value)
{
    t_env  *node;

    node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    node->name  = ft_strdup(name);
    node->value = ft_strdup(value);
    node->next  = NULL;
    return (node);
}

t_env  *env_init(char **envp)
{
    t_env   *list = NULL;
    char    *eq;
    size_t  len;
    int     i = 0;

    while (envp[i])
    {
        eq = ft_strchr(envp[i], '=');
        if (!eq)
            return (env_clear(&list), NULL);
        len = eq - envp[i];
        if (!env_add_back(&list, env_new(ft_strndup(envp[i], len), eq + 1)))
            return (env_clear(&list), NULL);
        i++;
    }
    return (list);
}

int  env_add_back(t_env **lst, t_env *new_node)
{
    t_env  *p;

    if (!lst || !new_node)
        return (0);
    if (!*lst)
        *lst = new_node;
    else
    {
        p = *lst;
        while (p->next)
            p = p->next;
        p->next = new_node;
    }
    return (1);
}

void  env_clear(t_env **lst)
{
    t_env  *tmp;

    while (*lst)
    {
        tmp = (*lst)->next;
        free((*lst)->name);
        free((*lst)->value);
        free(*lst);
        *lst = tmp;
    }
}

t_env  *env_find(t_env *lst, const char *name)
{
    while (lst)
    {
        if (ft_strcmp(lst->name, name) == 0)
            return (lst);
        lst = lst->next;
    }
    return (NULL);
}
