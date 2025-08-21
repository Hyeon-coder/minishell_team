/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 01:51:17 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_infile(const char *path)
{
	int	fd;

	if (!path)
		return (0);
	
	// 파일 존재 및 읽기 권한 확인
	if (access(path, F_OK) != 0)
	{
		perror((char *)path);
		return (-1);
	}
	if (access(path, R_OK) != 0)
	{
		perror((char *)path);
		return (-1);
	}
	
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

static int	check_dir_exists(const char *path)
{
	char	*dir;
	char	*last_slash;

	if (!path)
		return (0);
	
	dir = ft_strdup(path);
	if (!dir)
		return (-1);
	
	last_slash = ft_strrchr(dir, '/');
	if (last_slash && last_slash != dir)
	{
		*last_slash = '\0';
		if (access(dir, F_OK) != 0)
		{
			free(dir);
			ft_putstr_fd((char *)path, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (-1);
		}
	}
	
	free(dir);
	return (0);
}

static int	check_file_permissions(const char *path)
{
	struct stat	file_stat;
	
	if (access(path, F_OK) != 0)
		return (0);  // 파일이 존재하지 않으면 생성 가능
	
	if (stat(path, &file_stat) != 0)
		return (-1);
	
	// 디렉토리인지 확인
	if (S_ISDIR(file_stat.st_mode))
	{
		ft_putstr_fd((char *)path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		return (-1);
	}
	
	// 쓰기 권한 확인
	if (access(path, W_OK) != 0)
	{
		perror((char *)path);
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
	
	// 디렉토리 존재 확인
	if (check_dir_exists(path) < 0)
		return (-1);
	
	// 파일 권한 확인
	if (check_file_permissions(path) < 0)
		return (-1);
	
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
