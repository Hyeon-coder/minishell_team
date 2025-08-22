/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 17:21:20 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/23 02:36:18 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_2d_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
}

void	err_with_cmd(char *prefix, char *arg, char *suffix)
{
	if (prefix)
		ft_putstr_fd(prefix, 2);
	if (arg)
		ft_putstr_fd(arg, 2);
	if (suffix)
		ft_putstr_fd(suffix, 2);
}
