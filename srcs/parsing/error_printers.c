/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_printers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 02:55:38 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	write_syntax_error(char *msg, t_shell *shell)
{
	size_t	i;

	i = 0;
	shell->last_exit = 2;
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

void	print_mem_error(char *msg, t_shell *shell)
{
	size_t	i;

	i = 0;
	shell->last_exit = 1;
	if (!msg)
	{
		write(2, "Error\n", 6);
		return ;
	}
	while (msg[i])
	{
		write(2, &msg[i], 1);
		i++;
	}
	write(2, "\n", 1);
}
