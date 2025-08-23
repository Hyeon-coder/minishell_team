/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:39:31 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/23 07:18:46 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int ft_unset(char **argv, t_env **env_list)
{
	int i;

	if (!argv[1])
		return (0);
	i = 1;
	while (argv[i])
	{
		if (!is_valid_name(argv[i]))
			err_with_cmd("unset: '", argv[i], "': not a valid identifier\n");
		else
			env_remove(env_list, argv[i]);
		i++;
	}
	return (0);
}
