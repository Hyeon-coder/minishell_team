/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/27 23:39:19 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * ge_mini_shell의 입력 리디렉션 로직을 minishell_team 구조에 적용
 * t_infile 배열을 순차 처리
 */
static int	handle_input_redirections(t_ms *ms, t_infile **infiles)
{
	int		i;
	int		fd;
	t_infile	*infile;

	if (!infiles)
		return (0);

	i = 0;
	while (infiles[i])
	{
		infile = infiles[i];
		
		// 파일 열기
		fd = open(infile->name, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(infile->name, STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			ms->exit_status = 1;
			return (-1);
		}

		// stdin으로 리디렉션
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			ft_putstr_fd("minishell: dup2: ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			ms->exit_status = 1;
			return (-1);
		}

		close(fd);
		
		// heredoc 파일인 경우 삭제
		if (infile->heredoc)
			unlink(infile->name);
			
		i++;
	}

	return (0);
}

/**
 * ge_mini_shell의 출력 리디렉션 로직을 minishell_team 구조에 적용
 * outfile 배열과 append 플래그를 함께 처리
 */
static int	handle_output_redirections(t_ms *ms, char **outfiles, int *append)
{
	int	i;
	int	fd;
	int	flags;

	if (!outfiles)
		return (0);

	i = 0;
	while (outfiles[i])
	{
		// append 플래그에 따라 파일 열기 모드 결정
		if (append && append[i])
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;

		fd = open(outfiles[i], flags, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(outfiles[i], STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			ms->exit_status = 1;
			return (-1);
		}

		// stdout으로 리디렉션
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			close(fd);
			ft_putstr_fd("minishell: dup2: ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			ms->exit_status = 1;
			return (-1);
		}

		close(fd);
		i++;
	}

	return (0);
}

/**
 * ge_mini_shell의 apply_redirections 로직을 minishell_team 구조에 적용
 * t_cmd의 리디렉션 정보를 모두 처리
 */
int	apply_redirections(t_ms *ms, t_cmd *cmd)
{
	if (!cmd)
		return (0);

	// 입력 리디렉션 처리
	if (cmd->infile && handle_input_redirections(ms, cmd->infile) == -1)
		return (-1);

	// 출력 리디렉션 처리
	if (cmd->outfile && handle_output_redirections(ms, cmd->outfile, cmd->append) == -1)
		return (-1);

	return (0);
}

/**
 * 빈 명령어의 리디렉션만 처리 (파일 존재 확인용)
 * ge_mini_shell의 apply_redirections_for_empty 로직 적용
 */
int	apply_redirections_for_empty(t_ms *ms, t_cmd *cmd)
{
	int		i;
	int		fd;
	int		status;

	if (!cmd)
		return (0);

	status = 0;

	// 입력 파일들 존재 확인
	if (cmd->infile)
	{
		i = 0;
		while (cmd->infile[i])
		{
			fd = open(cmd->infile[i]->name, O_RDONLY);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(cmd->infile[i]->name, STDERR_FILENO);
				ft_putstr_fd(": ", STDERR_FILENO);
				ft_putendl_fd(strerror(errno), STDERR_FILENO);
				status = 1;
			}
			else
				close(fd);
			i++;
		}
	}

	// 출력 파일들 생성 확인
	if (cmd->outfile)
	{
		i = 0;
		while (cmd->outfile[i])
		{
			if (cmd->append && cmd->append[i])
				fd = open(cmd->outfile[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				fd = open(cmd->outfile[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(cmd->outfile[i], STDERR_FILENO);
				ft_putstr_fd(": ", STDERR_FILENO);
				ft_putendl_fd(strerror(errno), STDERR_FILENO);
				status = 1;
			}
			else
				close(fd);
			i++;
		}
	}

	if (status)
		ms->exit_status = 1;
	return (status);
}
