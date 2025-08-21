/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:13:10 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/20 23:40:34 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*joiner(char *arg, char *env, char *res, char *name)
{
	bool	detected;
	size_t	i;
	size_t	j;

	detected = false;
	i = 0;
	j = 0;
	while (arg[i])
	{
		if (arg[i] == '$' && !detected)
		{
			detected = true;
			if (env)
				j += move_env(&res[j], env);
			i += get_len(name);
			if (!ftstrcmp("$", name))
				i++;
		}
		else
			res[j++] = arg[i++];
	}
	res[j] = '\0';
	return (res);
}

char	*custom_join(char *arg, char *env, t_shell *shell, char *name)
{
	size_t	i;
	size_t	j;
	char	*res;

	i = get_arg_len(arg);
	j = get_len(env);
	res = (char *)malloc((i + j + 1) * sizeof(char));
	if (!res)
	{
		print_mem_error("memory allocation failed", shell);
		return (NULL);
	}
	res = joiner(arg, env, res, name);
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

	if (ftstrncmp(name, "?", 1))
	{
		sig_val = get_sig_val(shell->last_exit);
		ret = custom_join(str, sig_val, shell, "?");
	}
	else if (ftstrcmp(name, "$"))
		ret = custom_join(str, "$", shell, name);
	else
	{
		env = find_env(name, &shell->envs);
		if (!env)
			ret = custom_join(str, NULL, shell, name);
		else
			ret = custom_join(str, env->value, shell, name);
	}
	if (got_envs)
		free(str);
	return (ret);
}

char	*env_parse_handler(char *str, char *name, t_shell *shell, bool
got_envs)
{
	char	*ret;

	if (!name)
	{
		if (str)
			free(str);
		print_mem_error("memory allocation failed", shell);
		return (NULL);
	}
	ret = parse_env(str, name, shell, got_envs);
	free(name);
	return (ret);
}
