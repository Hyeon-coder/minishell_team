/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 03:01:37 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_unset(char **argv, t_env **env_list)
{
	int	i;
	int	exit_code;

	if (!argv[1])
		return (0);
	exit_code = 0;
	i = 1;
	while (argv[i])
	{
		if (!is_valid_name(argv[i]))
		{
			err_with_cmd("unset: '", argv[i], "': not a valid identifier");
			exit_code = 1;
		}
		else
			env_remove(env_list, argv[i]);
		i++;
	}
	return (exit_code);
}
