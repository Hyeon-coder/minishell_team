/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_printers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 20:23:17 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/12 20:23:27 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_mem_error(char *msg, t_shell *shell)
{
	size_t	i;

	shell->last_exit = 12;
	i = 0;
	if (!msg)
	{
		write(2, "\n", 1);
		return ;
	}
	while (msg[i])
	{
		write(2, &msg[i], 1);
		i++;
	}
	write(2, "\n", 1);
}
