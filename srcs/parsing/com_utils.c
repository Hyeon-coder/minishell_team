/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   com_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:36:15 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/23 06:32:46 by juhyeonl         ###   ########.fr       */
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

/* bash 동작 모방: 첫 번째 리다이렉션만 유지 */
/* com_utils.c 수정 - bash처럼 마지막 리다이렉션 사용 */

void	setup_directors(t_com *new, t_token **tokens)
{
	t_token	*current;
	char	*clean_filename;

	if (!new || !tokens || !*tokens)
		return ;
		
	current = *tokens;
	
	while (current && current->type != PIPE)
	{
		/* 입력 리다이렉션 처리 - 마지막 것 사용 (bash 동작) */
		if ((current->type == RD_I || current->type == HERE_DOC) && current->next)
		{
			/* 기존 파일명 해제 후 새로 설정 */
			if (new->infile)
				free(new->infile);
			
			clean_filename = remove_quotes_from_filename(current->next->str);
			if (clean_filename)
				new->infile = clean_filename;
			else
				new->infile = ft_strdup(current->next->str);
			
			new->redir_type_in = true;
			current = current->next; /* 파일명 토큰도 건너뛰기 */
		}
		/* 출력 리다이렉션 처리 - 마지막 것 사용 (bash 동작) */
		else if ((current->type == RD_O || current->type == RD_O_APPEND) && current->next)
		{
			/* 기존 파일명 해제 후 새로 설정 */
			if (new->outfile)
				free(new->outfile);
			
			clean_filename = remove_quotes_from_filename(current->next->str);
			if (!clean_filename)
				clean_filename = ft_strdup(current->next->str);
			
			if (clean_filename)
			{
				new->outfile = clean_filename;
				new->redir_type_out = true;
				
				/* append 플래그는 마지막 리다이렉션 타입에 따라 결정 */
				if (current->type == RD_O_APPEND)
					new->append = true;
				else
					new->append = false;
			}
			current = current->next; /* 파일명 토큰도 건너뛰기 */
		}
		
		if (current)
			current = current->next;
	}
}
