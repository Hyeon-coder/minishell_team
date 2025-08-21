/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 18:19:49 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 02:01:02 by juhyeonl         ###   ########.fr       */
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
    if (value)
        node->value = ft_strdup(value);
    else
        node->value = NULL;
    node->next  = NULL;
    return (node);
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

/* 추가된 env_set 함수 */
int	env_set(t_env **env_list, const char *name, const char *value)
{
	t_env	*existing;
	t_env	*new_node;

	if (!env_list || !name)
		return (-1);
	
	// 기존 변수 찾기
	existing = env_find(*env_list, name);
	if (existing)
	{
		// 기존 값 해제하고 새 값 설정
		if (existing->value)
			free(existing->value);
		
		if (value)
		{
			existing->value = ft_strdup(value);
			if (!existing->value)
				return (-1);
		}
		else
		{
			existing->value = NULL;
		}
		return (0);
	}
	
	// 새 노드 생성
	new_node = env_new(name, value);
	if (!new_node)
		return (-1);
	
	// 리스트에 추가
	if (!env_add_back(env_list, new_node))
	{
		if (new_node->name)
			free(new_node->name);
		if (new_node->value)
			free(new_node->value);
		free(new_node);
		return (-1);
	}
	
	return (0);
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
