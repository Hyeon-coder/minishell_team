/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 13:05:35 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* write_syntax_error 함수 (line_validator.c에서 사용) */
void	write_syntax_error(char *msg, t_shell *shell)
{
	if (!msg)
		return ;
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	if (shell)
		shell->last_exit = 2;
}

/* make_name 함수 (환경변수 이름 추출) */
char	*make_name(char *str)
{
	int		i;
	int		start;
	char	*name;

	if (!str || str[0] != '$')
		return (NULL);
	start = 1;  /* $ 다음부터 시작 */
	i = start;
	
	/* $? 처리 */
	if (str[i] == '?')
		return (ft_strdup("$"));
	
	/* 환경변수 이름 추출 */
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	
	if (i == start)  /* 이름이 없음 */
		return (NULL);
	
	name = ft_substr(str, start, i - start);
	return (name);
}

/* get_sig_val 함수 ($? 값 반환) */
char	*get_sig_val(int exit_code)
{
	return (ft_itoa(exit_code));
}

/* setup_redirections 함수 (기본 구현) */
int	setup_redirections(t_com *cmd)
{
	int	fd;

	if (!cmd)
		return (0);
	
	/* 입력 리다이렉션 */
	if (cmd->redir_type_in && cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
		{
			perror(cmd->infile);
			return (-1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	
	/* 출력 리다이렉션 */
	if (cmd->redir_type_out && cmd->outfile)
	{
		if (cmd->append)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		
		if (fd < 0)
		{
			perror(cmd->outfile);
			return (-1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	
	return (0);
}

/* is_builtin 함수 */
int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}
