/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:41:17 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/20 11:41:41 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// free_functions.c - 안전한 해제
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
		if (current->str)
		{
			free(current->str);
			current->str = NULL;
		}
		current->prev = NULL;
		current->next = NULL;
		free(current);
		current = temp;
	}
	*tokens = NULL;
}

// 리디렉션 처리에서 메모리 누수 방지
static int	open_outfile_safe(const char *path, int append)
{
	int	flags;
	int	fd;

	if (!path || !*path)
		return (0);
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(path, flags, 0644);
	if (fd < 0)
	{
		perror((char *)path);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
