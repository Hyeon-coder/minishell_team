/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 13:43:26 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 입력 리다이렉션 처리 */
static int	handle_input_redir(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/* 출력 리다이렉션 처리 */
static int	handle_output_redir(char *file, int append)
{
	int	fd;
	int	flags;

	if (append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	
	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/* Here Document 처리 */
static int	handle_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	char	*line;
	
	if (pipe(pipe_fd) < 0)
		return (-1);
	
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n", 2);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	close(pipe_fd[0]);
	return (0);
}

/* 모든 리다이렉션 설정 */
int	setup_redirections(t_com *cmd)
{
	t_token	*redir;

	if (!cmd)
		return (0);
    redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == RD_I)
		{
			if (handle_input_redir(redir->str) < 0)
				return (-1);
		}
		else if (redir->type == RD_O)
		{
			if (handle_output_redir(redir->str, 0) < 0)
				return (-1);
		}
		else if (redir->type == RD_O_APPEND)
		{
			if (handle_output_redir(redir->str, 1) < 0)
				return (-1);
		}
		else if (redir->type == HERE_DOC)
		{
			if (handle_heredoc(redir->str) < 0)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}
