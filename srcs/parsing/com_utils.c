/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   com_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:36:15 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 01:13:31 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	count_args(t_token **tokens)
{
	size_t	i;
	t_token	*current;

	current = *tokens;
	i = 1;
	while (current->next && current->next->type != PIPE)
	{
		i++;
		current = current->next;
	}
	return (i);
}

size_t	count_coms(t_token **tokens)
{
	size_t	res;
	t_token	*current;

	res = 1;
	current = *tokens;
	while (current->next)
	{
		if (current->type == PIPE)
			res++;
		current = current->next;
	}
	return (res);
}

void	fill_o_dir(t_com *new, t_token *d)
{
	// 기존 outfile이 있다면 해제 (마지막 리다이렉션만 유효)
	if (new->outfile)
		free(new->outfile);
	
	new->outfile = ft_strdup(d->next->str);
	new->redir_type_out = true;
	
	// append 모드 설정
	if (d->type == RD_O_APPEND)
		new->append = true;
	else
		new->append = false;
}

void	fill_in_dir(t_com *new, t_token *d)
{
	// 기존 infile이 있다면 해제 (마지막 리다이렉션만 유효)
	if (new->infile)
		free(new->infile);
	
	// heredoc이 있다면 해제
	if (new->heredoc_delimiter)
	{
		free(new->heredoc_delimiter);
		new->heredoc_delimiter = NULL;
	}
	
	if (d->type == HERE_DOC)
	{
		new->heredoc_delimiter = ft_strdup(d->next->str);
	}
	else
	{
		new->infile = ft_strdup(d->next->str);
	}
	new->redir_type_in = true;
}

void	setup_directors(t_com *new, t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	
	// 명령어 구조체 초기화
	new->infile = NULL;
	new->outfile = NULL;
	new->heredoc_delimiter = NULL;
	new->redir_type_in = false;
	new->redir_type_out = false;
	new->append = false;
	
	// 토큰을 순회하면서 리다이렉션 처리
	while (current && current->type != PIPE)
	{
		if (current->type == RD_I || current->type == HERE_DOC)
		{
			if (current->next && current->next->str)
				fill_in_dir(new, current);
		}
		else if (current->type == RD_O || current->type == RD_O_APPEND)
		{
			if (current->next && current->next->str)
				fill_o_dir(new, current);
		}
		current = current->next;
	}
}
