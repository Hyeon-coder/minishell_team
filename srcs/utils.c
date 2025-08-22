/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 17:21:20 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/15 17:36:26 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_tokens(t_token **args)
{
	(void)args;
}

void	ft_free_2d_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	err_with_cmd(char *prefix, char *arg, char *suffix)
{
	ft_putstr_fd(prefix, 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(suffix, 2);
	ft_putstr_fd("\n", 2);
}
