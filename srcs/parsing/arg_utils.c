/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:36:34 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/23 03:02:11 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*find_env(char *name, t_env **envs)
{
	t_env	*current;

	if (!name || !envs || !*envs)
		return (NULL);
	current = *envs;
	while (current)
	{
		if (current->name && ft_strcmp(current->name, name) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

// static bool	has_space(char *str)
// {
// 	size_t	i;
	
// 	if (!str)
// 		return (false);
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (is_whitespace(str[i]))
// 			return (true);
// 		i++;
// 	}
// 	return (false);
// }

static size_t	get_var_name_len(char *str)
{
	size_t	len;
	
	if (!str || !str[0])
		return (0);
	
	/* 특수 변수 $? */
	if (str[0] == '?')
		return (1);
	
	len = 0;
	/* 변수명은 알파벳, 숫자, 언더스코어로 구성 */
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	
	return (len);
}

char	*make_name(char *str, bool is_dq)
{
	size_t	var_len;
	char	*ret;
	
	if (!str || !str[0])
		return (NULL);
	
	var_len = get_var_name_len(str);
	if (var_len == 0)
		return (NULL);
	
	ret = malloc(var_len + 1);
	if (!ret)
		return (NULL);
	
	ft_strlcpy(ret, str, var_len + 1);
	(void)is_dq;
	return (ret);
}

size_t	get_arg_len(char *arg)
{
	size_t	len;
	size_t	i;
	bool	in_expansion;

	if (!arg)
		return (0);
	
	len = 0;
	i = 0;
	in_expansion = false;
	
	while (arg[i])
	{
		if (arg[i] == '$' && !in_expansion && arg[i + 1])
		{
			/* 환경변수 확장 부분을 건너뛰기 */
			i++; // '$' 건너뛰기
			size_t var_len = get_var_name_len(&arg[i]);
			i += var_len;
			in_expansion = true;
		}
		else
		{
			len++;
			i++;
			in_expansion = false;
		}
	}
	return (len);
}

size_t	get_len(char *str)
{
	if (!str)
		return (0);
	return (ft_strlen(str));
}

size_t	move_env(char *res, char *env)
{
	size_t	i;
	
	if (!res || !env)
		return (0);
	
	i = 0;
	while (env[i])
	{
		res[i] = env[i];
		i++;
	}
	return (i);
}
