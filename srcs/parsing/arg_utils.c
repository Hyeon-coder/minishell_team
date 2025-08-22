/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:36:34 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/07 20:36:36 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*find_env(char *name, t_env **envs)
{
	t_env	*current;

	current = *envs;
	if (!current)
		return (NULL);
	while (current->next)
	{
		if (ftstrcmp(name, current->name))
			return (current);
		current = current->next;
	}
	if (ftstrcmp(name, current->name))
		return (current);
	return (NULL);
}

bool	has_space(char *str)
{
	size_t	i;
	i = 0;
	while(str[i])
	{
		if(is_whitespace(str[i]))
			return (true);
		i++;
	}
	return (false);
}

void name_s(char *str, char *ret)
{
	size_t	i;
	i = 0;
	while (str[i] && !is_whitespace(str[i]))
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
}

void name_no_s(char *str, char *ret)
{
	size_t	i;
	i = 0;
	while (str[i] && !is_separator(str[i + 1]))
	{
		ret[i] = str[i];
		i++;
	}
	ret[i - 1] = '\0';
}
char	*make_name(char *str, bool is_dq)
{
	size_t	i;
	char	*ret;
	i = 0;
	if (!str)
		return (NULL);
	if (*str == '?')
		return (custom_dup("?"));
	while (str[i])
		i++;
	ret = (char *)malloc((1 + i) * sizeof(char));
	if (!ret)
		return (NULL);
	if (!has_space(str) && is_dq)
		name_no_s(str, ret);
	else
		name_s(str, ret);
	return (ret);
}

size_t	get_arg_len(char *arg)
{
	size_t	l;
	size_t	i;
	bool	detected;

	l = 0;
	i = 0;
	detected = false;
	if (!arg)
		return (l);
	while (arg[i])
	{
		if (arg[i] == '$' && !detected)
		{
			while (arg[i] && !is_whitespace(arg[i]))
				i++;
			detected = true;
		}
		else
		{
			l++;
			i++;
		}
	}
	return (l);
}

size_t	get_len(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (i);
	while (str[i])
		i++;
	return (i);
}

size_t	move_env(char *res, char *env)
{
	size_t	i;

	i = 0;
	while (env[i])
	{
		res[i] = env[i];
		i++;
	}
	return (i);
}
