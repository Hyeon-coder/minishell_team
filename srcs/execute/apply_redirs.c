/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 13:48:20 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	apply_redirs(t_com *cmd, t_shell *sh)
{
	int	result;

	if (!cmd || !sh)
		return (-1);
	
	result = setup_redirections(cmd);
	if (result != 0)
		sh->last_exit = 1;
	
	return (result);
}
