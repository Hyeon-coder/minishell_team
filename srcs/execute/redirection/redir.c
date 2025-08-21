/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/21 23:21:47 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_infile(const char *path)
{
	int	fd;

	if (!path)
		return (0);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		perror((char *)path);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	open_outfile(const char *path, int append)
{
	int	flags;
	int	fd;

	if (!path)
		return (0);
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(path, flags, 0644);
	if (fd < 0)
	{
		perror((char *)path);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	apply_redirs(t_com *cmd, t_shell *sh)
{
	if (!cmd)
		return (0);
	if (cmd->heredoc_delimiter)
	{
		if (handle_heredoc(cmd->heredoc_delimiter, sh) < 0)
			return (-1);
	}
	else if (cmd->infile)
	{
		if (open_infile(cmd->infile) < 0)
			return (-1);
	}
	if (cmd->outfile)
	{
		if (open_outfile(cmd->outfile, cmd->append) < 0)
			return (-1);
	}
	return (0);
}
