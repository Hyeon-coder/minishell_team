/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* redir.c                                            :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/15 16:24:02 by juhyeonl          #+#    #+#             */
/* Updated: 2025/08/23 06:34:27 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

/* redir.c 개선 - bash와 동일한 동작 */

/* 모든 리다이렉션 파일의 유효성을 먼저 검사 */
static int	validate_all_redirections(t_com *cmd)
{
	/* * bash는 실행 전에 모든 리다이렉션 파일을 검사한다
	 * 하나라도 실패하면 명령어 실행 자체를 중단
	 */
	
	/* 입력 파일 검사 */
	if (cmd->infile)
	{
		if (access(cmd->infile, F_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->infile, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (-1);
		}
		if (access(cmd->infile, R_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->infile, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			return (-1);
		}
	}
	
	/* 출력 파일 검사 */
	if (cmd->outfile)
	{
		/* 파일이 존재하는 경우 쓰기 권한 확인 */
		if (access(cmd->outfile, F_OK) == 0)
		{
			if (access(cmd->outfile, W_OK) != 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd->outfile, 2);
				ft_putstr_fd(": Permission denied\n", 2);
				return (-1);
			}
		}
		else
		{
			/* 파일이 없으면 디렉터리 쓰기 권한 확인 */
			char *dir_path = NULL;
			char *last_slash = ft_strrchr(cmd->outfile, '/');
			if (last_slash)
			{
				dir_path = ft_substr(cmd->outfile, 0, last_slash - cmd->outfile);
				if (access(dir_path, W_OK) != 0)
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(cmd->outfile, 2);
					ft_putstr_fd(": Permission denied\n", 2);
					free(dir_path);
					return (-1);
				}
				free(dir_path);
			}
		}
	}
	
	return (0);
}

int	apply_redirs(t_com *cmd, t_shell *sh)
{
	(void)sh;
	
	/* 먼저 모든 리다이렉션 검증 */
	if (validate_all_redirections(cmd) < 0)
	{
		sh->last_exit = 1;
		return (-1);
	}
	
	/* 실제 리다이렉션 적용 */
	if (open_infile(cmd->infile) < 0)
	{
		sh->last_exit = 1;
		return (-1);
	}
	if (open_outfile(cmd->outfile, cmd->append) < 0)
	{
		sh->last_exit = 1;
		return (-1);
	}
	return (0);
}
