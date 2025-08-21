/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 21:01:14 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 01:13:00 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_args(char **args)
{
	size_t	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;  // NULL로 설정하여 double free 방지
		i++;
	}
	free(args);
}

void	free_path(t_com *com)
{
	if (!com)
		return ;
	// 파싱 단계에서 할당된 메모리인지 확인하고 해제
	if (com->infile)
	{
		free(com->infile);
		com->infile = NULL;
	}
	if (com->outfile)
	{
		free(com->outfile);
		com->outfile = NULL;
	}
	if (com->heredoc_delimiter)
	{
		free(com->heredoc_delimiter);
		com->heredoc_delimiter = NULL;
	}
}

void	free_coms(t_com **coms)
{
	t_com	*current;
	t_com	*temp;

	if (!coms || !*coms)
		return ;
	current = *coms;
	while (current)
	{
		temp = current->next;
		
		// args 해제
		if (current->args)
			free_args(current->args);
		
		// path 관련 해제
		free_path(current);
		
		// 이전 노드 연결 해제
		if (current->prev)
			current->prev->next = current->next;
		if (current->next)
			current->next->prev = current->prev;
		
		// 현재 노드 해제
		free(current);
		current = temp;
	}
	*coms = NULL;
}

void	free_sh_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*temp;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	while (current)
	{
		temp = current->next;
		
		// 문자열 해제
		if (current->str)
		{
			free(current->str);
			current->str = NULL;
		}
		
		// 연결 해제
		if (current->prev)
			current->prev->next = current->next;
		if (current->next)
			current->next->prev = current->prev;
		
		// 현재 토큰 해제
		free(current);
		current = temp;
	}
	*tokens = NULL;
}

void	free_both(t_shell *shell)
{
	if (!shell)
		return ;
	free_sh_tokens(&shell->tokens);
	free_coms(&shell->commands);
}