/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ext_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/21 23:31:10 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_kv(char **dst, size_t *i, t_env *cur)
{
	char	*kv;

	kv = ft_strjoin(cur->name, "=");
	if (!kv)
		return (-1);
	dst[*i] = ft_strjoin(kv, cur->value ? cur->value : "");
	free(kv);
	if (!dst[*i])
		return (-1);
	(*i)++;
	return (0);
}

static size_t	count_env_vars(t_env *env)
{
	size_t	count;
	t_env	*cur;

	count = 0;
	cur = env;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	return (count);
}

static char	**allocate_envp(size_t count)
{
	char	**envp;

	envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	return (envp);
}

char	**ext_env_to_envp(t_env *env)
{
	size_t	count;
	size_t	i;
	t_env	*cur;
	char	**envp;

	count = count_env_vars(env);
	envp = allocate_envp(count);
	if (!envp)
		return (NULL);
	i = 0;
	cur = env;
	while (cur)
	{
		if (append_kv(envp, &i, cur) < 0)
			return (ft_free_2d_array(envp), NULL);
		cur = cur->next;
	}
	envp[i] = NULL;
	return (envp);
}
