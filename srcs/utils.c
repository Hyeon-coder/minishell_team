/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 01:58:45 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 기본 유틸리티 함수들 */
void	ft_free_2d_array(char **arr)
{
	int	i;

	if (!arr)
		return;
	
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	err_with_cmd(char *prefix, char *arg, char *suffix)
{
	ft_putstr_fd("minishell: ", 2);
	if (prefix)
		ft_putstr_fd(prefix, 2);
	if (arg)
		ft_putstr_fd(arg, 2);
	if (suffix)
		ft_putstr_fd(suffix, 2);
}

/* 누락된 external error 함수들 */
void	ext_print_perm_denied(const char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)cmd, 2);
	ft_putstr_fd(": Permission denied\n", 2);
}

void	ext_print_no_such_file(const char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)cmd, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}

void	ext_print_is_directory(const char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)cmd, 2);
	ft_putstr_fd(": Is a directory\n", 2);
}

void	ext_print_cmd_not_found(const char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

/* 누락된 parsing 함수 */
void	set_com_type(char *str, t_com *token)
{
	(void)str;
	(void)token;
	// 구현이 필요하다면 파트너 코드 확인 필요
}
