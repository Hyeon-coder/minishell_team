/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 02:58:48 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_input_redir(t_com *cmd)
{
	int	fd;

	if (!cmd->infile)
		return (0);
	fd = open(cmd->infile, O_RDONLY);
	if (fd < 0)
	{
		perror(cmd->infile);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		perror("dup2");
		return (1);
	}
	close(fd);
	return (0);
}

static int	handle_output_redir(t_com *cmd)
{
	int	fd;
	int	flags;

	if (!cmd->outfile)
		return (0);
	flags = O_WRONLY | O_CREAT;
	if (cmd->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(cmd->outfile, flags, 0644);
	if (fd < 0)
	{
		perror(cmd->outfile);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		perror("dup2");
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_redirs(t_com *cmd, t_shell *sh)
{
	(void)sh;
	if (handle_input_redir(cmd) != 0)
		return (1);
	if (handle_output_redir(cmd) != 0)
		return (1);
	return (0);
}
