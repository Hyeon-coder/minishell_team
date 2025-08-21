/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 01:13:19 by juhyeonl         ###   ########.fr       */
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

static int	check_file_permissions(const char *path)
{
	struct stat	file_stat;
	
	if (access(path, F_OK) != 0)
		return (-1);  // 파일이 존재하지 않음
	
	if (stat(path, &file_stat) != 0)
		return (-1);
	
	// 디렉토리인지 확인
	if (S_ISDIR(file_stat.st_mode))
	{
		errno = EISDIR;
		return (-1);
	}
	
	return (0);
}

static int	open_outfile(const char *path, int append)
{
	int	flags;
	int	fd;

	if (!path)
		return (0);
	
	// 파일이 이미 존재하는 경우 권한 확인
	if (access(path, F_OK) == 0)
	{
		if (check_file_permissions(path) < 0)
		{
			if (errno == EISDIR)
				ft_putstr_fd(": Is a directory\n", 2);
			else
				perror((char *)path);
			return (-1);
		}
		if (access(path, W_OK) != 0)
		{
			perror((char *)path);
			return (-1);
		}
	}
	
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

static int	process_multiple_redirections(t_com *cmd)
{
	// 여러 리다이렉션이 있는 경우 마지막 것만 적용 (bash 동작 모방)
	// 하지만 모든 파일들의 유효성은 검사해야 함
	
	// 입력 파일들 검사
	if (cmd->infile)
	{
		if (access(cmd->infile, F_OK) != 0)
		{
			perror((char *)cmd->infile);
			return (-1);
		}
		if (access(cmd->infile, R_OK) != 0)
		{
			perror((char *)cmd->infile);
			return (-1);
		}
	}
	
	// 출력 파일들 검사 (하지만 실제로는 마지막 것만 사용)
	if (cmd->outfile)
	{
		// 부모 디렉토리가 존재하는지 확인
		char *dir = ft_strdup(cmd->outfile);
		char *last_slash = ft_strrchr(dir, '/');
		if (last_slash && last_slash != dir)
		{
			*last_slash = '\0';
			if (access(dir, F_OK) != 0)
			{
				free(dir);
				ft_putstr_fd(cmd->outfile, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				return (-1);
			}
		}
		free(dir);
	}
	
	return (0);
}

int	apply_redirs(t_com *cmd, t_shell *sh)
{
	if (!cmd)
		return (0);
	
	// 여러 리다이렉션 처리
	if (process_multiple_redirections(cmd) < 0)
		return (-1);
		
	// heredoc이 있으면 우선 처리
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
	
	// 출력 리다이렉션 처리
	if (cmd->outfile)
	{
		if (open_outfile(cmd->outfile, cmd->append) < 0)
			return (-1);
	}
	
	return (0);
}
