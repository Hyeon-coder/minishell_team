/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:13:10 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/23 02:34:40 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*joiner(char *arg, char *env, char *res, char *name)
{
	bool	detected;
	size_t	i;
	size_t	j;
	size_t	name_len;

	detected = false;
	i = 0;
	j = 0;
	name_len = ft_strlen(name);
	
	while (arg[i])
	{
		if (arg[i] == '$' && !detected && ft_strncmp(&arg[i + 1], name, name_len) == 0)
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
	size_t	result_len;

	if (!arg || !env || !name)
		return (0);
	
	arg_len = ft_strlen(arg);
	env_len = ft_strlen(env);
	name_len = ft_strlen(name);
	
	// Calculate: original_length - ($ + name) + env_value
	result_len = arg_len - (name_len + 1) + env_len;
	
	return (result_len);
}

char	*custom_join(char *arg, char *env, bool got_envs, char *name)
{
	size_t	result_len;
	char	*res;

	if (!env)
		return (NULL);
	if (!arg)
		return (ft_strdup(env));
	
	result_len = calculate_result_len(arg, env, name);
	res = (char *)malloc((result_len + 1) * sizeof(char));
	if (!res)
	{
		if (got_envs)
			free(arg);
		return (NULL);
	}
	
	res = joiner(arg, env, res, name);
	if (got_envs)
		free(arg);
	return (res);
}

char	*get_sig_val(int lsig)
{
	int		copy;
	char	*ret;
	size_t	len;

	copy = lsig;
	len = 1;
	while (copy >= 10)
	{
		len++;
		copy /= 10;
	}
	ret = (char *)malloc((len + 1) * sizeof(char));
	if (!ret)
		return (NULL);
	ret[len] = '\0';
	len--;
	while (len > 0 && lsig >= 10)
	{
		ret[len] = (lsig % 10) + 48;
		lsig /= 10;
		len--;
	}
	ret[len] = lsig + 48;
	return (ret);
}

char	*parse_env(char *str, char *name, t_shell *shell, bool got_envs)
{
	char	*ret;
	t_env	*env;
	char	*sig_val;

	if (ft_strcmp(name, "?") == 0)
	{
		sig_val = get_sig_val(shell->last_exit);
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
