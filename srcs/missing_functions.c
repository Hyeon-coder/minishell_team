/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   missing_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 13:05:15 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 토큰 초기화 */
void	init_token_vals(t_token *token)
{
	if (!token)
		return ;
	token->str = NULL;
	token->type = WORD;
	token->sq = false;
	token->dq = false;
	token->does_exist = false;
	token->next = NULL;
	token->prev = NULL;
}

/* 문자열 비교 (ft_strcmp와 동일) */
int	ftstrcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (-1);
	return (ft_strcmp(s1, s2) == 0);
}

/* n개 문자 비교 */
int	ftstrncmp(const char *s1, const char *s2, size_t n)
{
	if (!s1 || !s2)
		return (-1);
	return (ft_strncmp(s1, s2, n) == 0);
}

/* 메타문자 확인 */
bool	is_meta(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '$' || 
			c == '\'' || c == '"' || c == '\\' || c == ';');
}

/* 메모리 에러 출력 */
void	print_mem_error(char *msg, t_shell *shell)
{
	if (!msg)
		msg = "memory allocation failed";
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	if (shell)
		shell->last_exit = 1;
}

/* 문자열 복사 */
char	*custom_dup(char *str)
{
	if (!str)
		return (NULL);
	return (ft_strdup(str));
}

/* 토큰 경로 설정 (기본 구현) */
void	token_path_setter(char *str, t_token *token)
{
	(void)str;
	(void)token;
	/* 경로 설정 로직은 필요에 따라 구현 */
}

/* 커맨드 경로 설정 (기본 구현) */
void	com_path_setter(char *str, t_com *token)
{
	(void)str;
	(void)token;
	/* 경로 설정 로직은 필요에 따라 구현 */
}

/* 환경변수 존재 확인 */
bool	does_env_exist(t_token *token, t_env **envs)
{
	t_env	*current;
	char	*name;

	if (!token || !token->str || !envs)
		return (false);
	
	/* $? 는 항상 존재 */
	if (ft_strcmp(token->str, "$?") == 0)
		return (true);
	
	/* $ 제거하고 이름만 추출 */
	if (token->str[0] == '$')
		name = token->str + 1;
	else
		name = token->str;
	
	current = *envs;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (true);
		current = current->next;
	}
	return (false);
}

/* 구분자 확인 (make_args.c에서 사용) */
bool	is_separator(char c)
{
	return (c == '$' || is_whitespace(c) || is_meta(c));
}
