/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_stub.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 12:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 04:48:24 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_simple_stub(t_shell *sh)
{
	t_com	*cmd;

	if (!sh || !sh->commands)
		return (0);
	cmd = sh->commands;
	if (!cmd->args || !cmd->args[0])
		return (0);
	printf("[STUB] Executing: %s\n", cmd->args[0]);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
	{
		if (cmd->args[1])
			printf("%s\n", cmd->args[1]);
		else
			printf("\n");
	}
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
	{
		char	*cwd;

		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			printf("%s\n", cwd);
			free(cwd);
		}
	}
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
	{
		printf("exit\n");
		exit(0);
	}
	else
	{
		printf("minishell: %s: command not found\n", cmd->args[0]);
		return (127);
	}
	return (0);
}
