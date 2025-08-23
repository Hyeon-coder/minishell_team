/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:13:10 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/23 03:04:56 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*joiner(char *arg, char *env, char *res, char *name)
{
	bool	detected;
	size_t	i;
	size_t	j;
	size_t	name_len;

	if (!arg || !env || !res || !name)
		return (NULL);
	
	detected = false;
	i = 0;
	j = 0;
	name_len = ft_strlen(name);
	
	while (arg[i])
	{
		if (arg[i] == '$' && !detected && 
		    ft_strncmp(&arg[i + 1], name, name_len) == 0 &&
		    (i + 1 + name_len >= ft_strlen(arg) && 
		     !ft_isalnum(arg[i + 1 + name_len]) && arg[i + 1 + name_len] != '_'))
		{
			detected = true;
			j += move_env(&res[j], env);
			i += name_len + 1; // +1 for '$'
		}
		else
		{
			res[j++] = arg[i++];
		}
	}
	res[j] = '\0';
	return (res);
}

static size_t	calculate_result_len(char *arg, char *env, char *name)
{
	size_t	arg_len;
	size_t	env_len;
	size_t	name_len;

	if (!arg || !env || !name)
		return (0);
	
	arg_len = ft_strlen(arg);
	env_len = ft_strlen(env);
	name_len = ft_strlen(name);
	(void)name_len;
	/* 안전한 계산: 최대 크기로 할당 */
	return (arg_len + env_len + 1);
}

char	*custom_join(char *arg, char *env, bool got_envs, char *name)
{
	size_t	result_len;
	char	*res;

	if (!env)
		env = "";
	if (!arg)
		return (ft_strdup(env));
	if (!name)
		return (got_envs ? free(arg), NULL : NULL);
	
	result_len = calculate_result_len(arg, env, name);
	res = malloc(result_len + 1);
	if (!res)
	{
		if (got_envs)
			free(arg);
		return (NULL);
	}
	
	res = joiner(arg, env, res, name);
	if (!res)
	{
		if (got_envs)
			free(arg);
		return (NULL);
	}
	
	if (got_envs)
		free(arg);
	return (res);
}

char	*get_sig_val(int lsig)
{
	char	*ret;

	ret = ft_itoa(lsig);
	return (ret);
}

char	*parse_env(char *str, char *name, t_shell *shell, bool got_envs)
{
	char	*ret;
	t_env	*env;
	char	*sig_val;

	if (!name)
		return (got_envs && str ? (free(str), NULL) : NULL);
	
	if (ft_strcmp(name, "?") == 0)
	{
		sig_val = get_sig_val(shell->last_exit);
		if (!sig_val)
		{
			if (got_envs && str)
				free(str);
			return (NULL);
		}
		ret = custom_join(str, sig_val, got_envs, "?");
		free(sig_val);
	}
	else
	{
		env = find_env(name, &shell->envs);
		if (!env || !env->value)
			ret = custom_join(str, "", got_envs, name);
		else
			ret = custom_join(str, env->value, got_envs, name);
	}
	return (ret);
}

char	*env_parse_handler(char *str, char *name, t_shell *shell, bool got_envs)
{
	char	*ret;

	if (!name)
	{
		if (got_envs && str)
			free(str);
		print_mem_error("memory allocation failed", shell);
		return (NULL);
	}
	ret = parse_env(str, name, shell, got_envs);
	free(name);
	return (ret);
}
