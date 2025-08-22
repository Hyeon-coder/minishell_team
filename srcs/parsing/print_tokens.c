/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:28:14 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/07 19:28:16 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	write_syntax_error(char *msg, t_shell *shell)
{
	size_t	i;

	i = 0;
	shell->last_exit = 2;
	if (!msg)
	{
		write(2, "\n", 1);
		return ;
	}
	while (msg[i])
	{
		write(2, &msg[i], 1);
		i++;
	}
	write(2, "\n", 1);
	return ;
}

void	write_str(char *str)
{
	size_t	i;

	i = -1;
	if (!str)
		return ;
	if (!*str)
		return ;
	while (str[++i])
		write(1, &str[i], 1);
}

void	print_tokens(t_token **tokens)
{
	t_token	*token;

	if (!tokens)
		return ;
	token = *tokens;
	while (token)
	{
		write_str(token->str);
		token = token->next;
	}
}
