/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljh3900 <ljh3900@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:39:31 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/06/08 07:55:02 by ljh3900          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int ft_unset(char **argv, t_env **env_list)
{
	int i;

	if (!argv[1])
		err_with_cmd("unset: ", NULL, "not enough arguments\n");
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
