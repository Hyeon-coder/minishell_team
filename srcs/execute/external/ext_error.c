/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ext_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 01:50:32 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cmd_error(const char *cmd, const char *msg, int exit_code)
{
	(void)exit_code;  // unused parameter
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd((char *)msg, 2);
	ft_putstr_fd("\n", 2);
}

int	handle_exec_error(const char *cmd, t_shell *sh)
{
	struct stat	file_stat;
	
	if (!cmd)
		return (127);
	
	// 파일이 존재하는지 확인
	if (access(cmd, F_OK) != 0)
	{
		if (ft_strchr(cmd, '/'))
		{
			print_cmd_error(cmd, "No such file or directory", 127);
			sh->last_exit = 127;
			return (127);
		}
		else
		{
			print_cmd_error(cmd, "command not found", 127);
			sh->last_exit = 127;
			return (127);
		}
	}
	
	// 파일 정보 가져오기
	if (stat(cmd, &file_stat) != 0)
	{
		print_cmd_error(cmd, "No such file or directory", 127);
		sh->last_exit = 127;
		return (127);
	}
	
	// 디렉토리인지 확인
	if (S_ISDIR(file_stat.st_mode))
	{
		print_cmd_error(cmd, "Is a directory", 126);
		sh->last_exit = 126;
		return (126);
	}
	
	// 실행 권한 확인
	if (access(cmd, X_OK) != 0)
	{
		print_cmd_error(cmd, "Permission denied", 126);
		sh->last_exit = 126;
		return (126);
	}
	
	// 기타 실행 오류
	print_cmd_error(cmd, "Exec format error", 126);
	sh->last_exit = 126;
	return (126);
}

void	handle_directory_error(const char *path)
{
	struct stat	file_stat;
	
	if (!path)
		return;
	
	if (access(path, F_OK) != 0)
	{
		print_cmd_error(path, "No such file or directory", 1);
		return;
	}
	
	if (stat(path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
	{
		print_cmd_error(path, "Is a directory", 1);
		return;
	}
	
	print_cmd_error(path, "Not a directory", 1);
}

int	validate_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	
	// 첫 문자는 알파벳 또는 언더스코어
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	
	// 나머지 문자는 알파벳, 숫자, 언더스코어
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	
	return (1);
}

void	export_error(const char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}
