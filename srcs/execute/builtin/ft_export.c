/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 01:13:41 by ljh3900           #+#    #+#             */
/*   Updated: 2025/08/23 05:31:31 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void parse_arg(const char *arg, char **out_name, char **out_value)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*out_name  = ft_strndup(arg, eq - arg);
		*out_value = ft_strdup(eq + 1);
	}
	else
	{
		*out_name  = ft_strdup(arg);
		*out_value = ft_strdup("");
	}
}

static int handle_single_export(char *name, char *value, t_env **env_list)
{
	t_env	*node;

	if (!is_valid_name(name))
	{
		err_with_cmd("export: '", name, "': not a valid identifier\n");
		return (export_cleanup(name, value, NULL, 1)); /* 에러 시 1 반환 */
	}
	node = env_find(*env_list, name);
	if (node)
	{
		free(node->value);
		node->value = value;
		free(name);
	}
	else
	{
		node = env_new(name, value);
		if (!node)
			return export_cleanup(name, value, NULL, 1);
		if (!env_add_back(env_list, node))
			return export_cleanup(name, value, node, 1);
		free(name);
		free(value);
	}
	return (0);
}

int ft_export(char **argv, t_env **env_list)
{
	int		i;
	char	*name;
	char	*value;
	int		status;

	if (!argv[1])
	{
		print_export_list(*env_list);
		return (0);
	}
	i = 1;
	status = 0;
	while (argv[i])
	{
		parse_arg(argv[i], &name, &value);
		if (handle_single_export(name, value, env_list))
			status = 1; /* 에러 발생 시 최종 status를 1로 설정 */
		i++;
	}
	return (status);
}
