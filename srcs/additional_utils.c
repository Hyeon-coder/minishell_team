/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 15:42:41 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* 환경변수 값 가져오기 */
char	*get_env_value(char *name, t_env *env_list)
{
	t_env	*node;

	if (!name || !env_list)
		return (NULL);
	node = env_find(env_list, name);
	if (node && node->value)
		return (node->value);
	return (NULL);
}

/* 빌트인 명령어 확인 */
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

/* 문자열 복사 */
char	*custom_dup(char *str)
{
	if (!str)
		return (NULL);
	return (ft_strdup(str));
}

/* 메타 문자 확인 */
bool	is_meta(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/* 메모리 에러 출력 */
void	print_mem_error(char *msg, t_shell *shell)
{
	if (msg)
		ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	if (shell)
		shell->last_exit = 1;
}
