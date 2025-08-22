/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:42:36 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 20:13:32 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void	set_type(t_token *token)
{
	if (!ft_strcmp("echo", token->str))        // ← ! 추가
		token->type = ECHO;
	if (!ft_strcmp("pwd", token->str))         // ← ! 추가
		token->type = PWD;
	if (!ft_strcmp("exit", token->str))        // ← ! 추가
		token->type = EXIT;
	if (!ft_strcmp("|", token->str) && !token->sq && !token->dq)  // ← ! 추가
	{
		printf("[DEBUG] set_type: PIPE detected! Setting type to PIPE\n");
		token->type = PIPE;
	}
	if (!ft_strcmp("unset", token->str))       // ← ! 추가
		token->type = UNSET;
	if (!ft_strcmp("export", token->str))      // ← ! 추가
		token->type = EXPORT;
	printf("[DEBUG] set_type: final type = %d\n", token->type);
	if (!ft_strcmp("-n", token->str))          // ← ! 추가
		token->type = N;
	if (token->str[0] == '$' && !token->sq)
		setenv_type(token);
	if (!ft_strcmp(">", token->str) && !token->sq && !token->dq)    // ← ! 추가
		token->type = RD_O;
	if (!ft_strcmp(">>", token->str) && !token->sq && !token->dq)   // ← ! 추가
		token->type = RD_O_APPEND;
	if (!ft_strcmp("<", token->str) && !token->sq && !token->dq)    // ← ! 추가
		token->type = RD_I;
	if (!ft_strcmp("<<", token->str) && !token->sq && !token->dq)   // ← ! 추가
		token->type = HERE_DOC;
	token_path_setter(token->str, token);
}

size_t	handle_rd(char *line)
{
	size_t	i;

	i = 0;
	while (is_rd(line[i]))
		i++;
	return (i);
}

size_t	rd_loop(char *line)
{
	size_t	i;

	i = 0;
	while (is_rd(line[i]))
		i++;
	return (i);
}

size_t	defloop(char *line)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\t')
			break ;
		else if (line[i] == '|')
			break ;
		else if (line[i] == ')')  /* 수정된 부분: 작은따옴표 닫기 */
			break ;
		else if (line[i] == '(')
			break ;
		else if (is_rd(line[i]))
			break ;
		else
			i++;
	}
	return (i);
}
