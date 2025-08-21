/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 00:46:37 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_heredoc_warning(void)
{
	ft_putstr_fd("minishell: warning: here-document delimited", 2);
	ft_putstr_fd(" by end-of-file\n", 2);
}

static int	write_line_to_pipe(int pipefd, const char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (write(pipefd, line, len) == -1)
		return (-1);
	if (write(pipefd, "\n", 1) == -1)
		return (-1);
	return (0);
}

static int	check_signal_interrupt(void)
{
	extern volatile sig_atomic_t	g_signal;

	if (g_signal == SIGINT)
	{
		g_signal = 0;
		return (1);
	}
	return (0);
}

static int	read_heredoc_lines(const char *delimiter, int pipefd)
{
	char	*line;
	int		delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write_heredoc_warning();
			break ;
		}
		if (check_signal_interrupt())
		{
			free(line);
			return (-1);
		}
		if (ft_strncmp(line, delimiter, delimiter_len) == 0 
			&& line[delimiter_len] == '\0')
		{
			free(line);
			break ;
		}
		if (write_line_to_pipe(pipefd, line) == -1)
		{
			free(line);
			return (-1);
		}
		free(line);
	}
	return (0);
}

int	handle_heredoc(const char *delimiter, t_shell *sh)
{
	int	pipefd[2];

	(void)sh;
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (read_heredoc_lines(delimiter, pipefd[1]) < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(pipefd[0]);
		return (-1);
	}
	close(pipefd[0]);
	return (0);
}
