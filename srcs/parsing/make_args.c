/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:36:15 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 01:52:00 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	is_redirection_token(enum e_Types type)
{
	return (type == RD_I || type == RD_O || 
			type == RD_O_APPEND || type == HERE_DOC);
}

static void	free_args_array(char **args, size_t count)
{
	size_t	i;

	if (!args)
		return;
	
	i = 0;
	while (i < count && args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
}

static char	**allocate_args_array(size_t count)
{
	char	**args;

	if (count == 0)
		return (NULL);
	
	args = (char **)malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	
	// 배열 초기화
	for (size_t i = 0; i <= count; i++)
		args[i] = NULL;
	
	return (args);
}

static char	*safe_strdup(const char *str)
{
	char	*dup;

	if (!str)
		return (NULL);
	
	dup = ft_strdup(str);
	if (!dup)
	{
		ft_putstr_fd("minishell: malloc failed\n", 2);
		return (NULL);
	}
	
	return (dup);
}

char	**make_args(t_token **tokens, t_shell *shell)
{
	t_token	*current;
	char	**args;
	size_t	count;
	size_t	i;

	if (!tokens || !*tokens)
		return (NULL);
	
	(void)shell;  // unused parameter
	
	count = count_args(tokens);
	if (count == 0)
		return (NULL);
	
	args = allocate_args_array(count);
	if (!args)
		return (NULL);
	
	current = *tokens;
	i = 0;
	
	while (current && current->type != PIPE && i < count)
	{
		// 리다이렉션 토큰이면 건너뛰기
		if (is_redirection_token(current->type))
		{
			current = current->next; // 리다이렉션 연산자
			if (current)
				current = current->next; // 파일명
		}
		else
		{
			// 일반 인자 복사
			args[i] = safe_strdup(current->str);
			if (!args[i])
			{
				free_args_array(args, i);
				return (NULL);
			}
			i++;
			current = current->next;
		}
	}
	
	args[i] = NULL;
	return (args);
}
