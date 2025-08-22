/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   missing_functions_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 16:05:58 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 환경변수 제거 */
int	env_remove(t_env **lst, const char *name)
{
	t_env	*current;
	t_env	*prev;

	if (!lst || !*lst || !name)
		return (1);
	current = *lst;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*lst = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}

/* 문법 에러 출력 */
void	write_syntax_error(char *msg, t_shell *shell)
{
	if (!msg)
		return ;
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	if (shell)
		shell->last_exit = 2;
}

/* 메타 문자 확인 */
bool	is_meta(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/* 공백 문자 확인 */
bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

/* 구분자 확인 */
bool	is_separator(char c)
{
	return (c == ' ' || is_whitespace(c) || is_meta(c) || c == '\0');
}
