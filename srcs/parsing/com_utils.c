/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   com_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:36:15 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/23 04:53:30 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 리다이렉션 파일명에서 따옴표를 제거하는 함수 */
static char	*remove_quotes_from_filename(const char *str)
{
	char	*result;
	size_t	i;
	size_t	j;
	size_t	len;
	bool	in_single_quote;
	bool	in_double_quote;

	if (!str)
		return (NULL);
	
	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	
	i = 0;
	j = 0;
	in_single_quote = false;
	in_double_quote = false;
	
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			i++; // 따옴표 건너뛰기
		}
		else if (str[i] == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			i++; // 따옴표 건너뛰기
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	
	result[j] = '\0';
	return (result);
}

size_t	count_args(t_token **tokens)
{
	size_t	count;
	t_token	*current;

	if (!tokens || !*tokens)
		return (0);
		
	current = *tokens;
	count = 0;
	
	while (current && current->type != PIPE)
	{
		/* 리디렉션 토큰과 그 다음 파일명은 인자에서 제외 */
		if (current->type == RD_I || current->type == RD_O || 
		    current->type == RD_O_APPEND || current->type == HERE_DOC)
		{
			current = current->next; // 리디렉션 토큰 건너뛰기
			if (current)             // 파일명 토큰 건너뛰기
				current = current->next;
		}
		else
		{
			count++;
			current = current->next;
		}
	}
	return (count);
}

size_t	count_coms(t_token **tokens)
{
	size_t	res;
	t_token	*current;

	if (!tokens || !*tokens)
		return (0);
		
	res = 1;
	current = *tokens;
	while (current)
	{
		if (current->type == PIPE)
			res++;
		current = current->next;
	}
	return (res);
}

void	fill_o_dir(t_com *new, t_token *d)
{
	char	*clean_filename;

	if (!new || !d || !d->next || !d->next->str)
		return ;
		
	/* 기존 파일명 해제 */
	if (new->outfile)
		free(new->outfile);
	
	/* 따옴표 제거한 파일명 생성 */
	clean_filename = remove_quotes_from_filename(d->next->str);
	if (!clean_filename)
	{
		new->outfile = ft_strdup(d->next->str); // 실패 시 원본 사용
	}
	else
	{
		new->outfile = clean_filename;
	}
	
	new->redir_type_out = true;
	
	if (d->type == RD_O_APPEND)
		new->append = true;
	else
		new->append = false;
}

void	fill_in_dir(t_com *new, t_token *d)
{
	char	*clean_filename;

	if (!new || !d || !d->next || !d->next->str)
		return ;
		
	/* 기존 파일명 해제 */
	if (new->infile)
		free(new->infile);
	
	/* 따옴표 제거한 파일명 생성 */
	clean_filename = remove_quotes_from_filename(d->next->str);
	if (!clean_filename)
	{
		new->infile = ft_strdup(d->next->str); // 실패 시 원본 사용
	}
	else
	{
		new->infile = clean_filename;
	}
	
	new->redir_type_in = true;
}

void	setup_directors(t_com *new, t_token **tokens)
{
	t_token	*last_in_dir;
	t_token	*last_o_dir;
	t_token	*current;

	if (!new || !tokens || !*tokens)
		return ;
		
	current = *tokens;
	last_in_dir = NULL;
	last_o_dir = NULL;
	
	while (current && current->type != PIPE)
	{
		if (current->type == RD_I || current->type == HERE_DOC)
			last_in_dir = current;
		if (current->type == RD_O || current->type == RD_O_APPEND)
			last_o_dir = current;
		current = current->next;
	}
	
	if (last_in_dir && last_in_dir->next)
		fill_in_dir(new, last_in_dir);
	if (last_o_dir && last_o_dir->next)
		fill_o_dir(new, last_o_dir);
}
