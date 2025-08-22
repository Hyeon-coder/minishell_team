/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_and_file_validators.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:20:36 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/23 02:19:23 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_valid_dir(char *path, t_shell *shell)
{
	DIR	*dir;

	dir = opendir(path);
	if (!dir)
	{
		write_syntax_error("invalid directory", shell);
		return (false);
	}
	closedir(dir);
	return (true);
}

// 입력 파일 검증
bool	is_valid_input_file(const char *filename, t_shell *shell)
{
	int	fd;

	if (!filename)
		return (false);
	
	// 파일 존재 여부 확인
	if (access(filename, F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putchar_fd('\n', 2);
		shell->last_exit = 1;
		return (false);
	}
	
	// 읽기 권한 확인
	if (access(filename, R_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)filename, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		shell->last_exit = 1;
		return (false);
	}
	
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror((char *)filename);
		shell->last_exit = 1;
		return (false);
	}
	close(fd);
	return (true);
}

// 출력 파일 검증
bool	is_valid_output_file(const char *filename, t_shell *shell)
{
	int	fd;
	char *dir_path;
	char *last_slash;

	if (!filename)
		return (false);
	
	// 디렉토리 경로 추출
	last_slash = ft_strrchr(filename, '/');
	if (last_slash)
	{
		dir_path = ft_substr(filename, 0, last_slash - filename);
		if (access(dir_path, W_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd((char *)filename, 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(strerror(errno), 2);
			ft_putchar_fd('\n', 2);
			free(dir_path);
			shell->last_exit = 1;
			return (false);
		}
		free(dir_path);
	}
	
	// 쓰기 가능 여부 테스트
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror((char *)filename);
		shell->last_exit = 1;
		return (false);
	}
	close(fd);
	unlink(filename);  // 테스트 파일 삭제
	return (true);
}

// 기존 is_valid_file 함수를 토큰 타입에 따라 분기
bool	is_valid_file(t_token *token, t_shell *shell)
{
	if (!token || !token->str)
		return (false);
	
	// 이전 토큰을 확인해서 입력/출력 파일인지 판단
	if (token->prev)
	{
		if (token->prev->type == RD_I || token->prev->type == HERE_DOC)
			return (is_valid_input_file(token->str, shell));
		else if (token->prev->type == RD_O || token->prev->type == RD_O_APPEND)
			return (is_valid_output_file(token->str, shell));
	}
	
	return (true);
}
