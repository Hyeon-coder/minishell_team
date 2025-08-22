/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:37:54 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 18:34:30 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	token_len(char *line, t_token *token)
{
	size_t	i;

	printf("[DEBUG tokenize] token_len start\n");
	i = 0;
	if (line[i] == '|')
		return (1);
	if (is_rd(line[i]))
		return (handle_rd(line));
	i = token_dub_loop(line, token);
	printf("[DEBUG tokenize] token_len end: %zu\n", i);
	return (i);
}

char	*token_dup(char *line, t_token *token)
{
	size_t	i;
	char	*res;
	size_t	l;

	printf("[DEBUG tokenize] token_dup start\n");
	l = token_len(line, token);
	res = (char *)malloc((l + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	if (token->sq)
		sq_dub(line, res, l);
	else if (token->dq)
		dq_dub(line, res, l);
	else
	{
		while (i < l)
		{
			res[i] = line[i];
			i++;
		}
		res[i] = '\0';
	}
	printf("[DEBUG tokenize] token_dup end: '%s'\n", res);
	return (res);
}

t_token	*make_token(char *line, t_shell *shell)
{
	t_token	*token;

	printf("[DEBUG tokenize] make_token start: line='%s'\n", line);
	token = (t_token *)malloc(1 * sizeof(t_token));
	if (!token)
	{
		print_mem_error("memory allocation failed", shell);
		return (NULL);
	}
	printf("[DEBUG tokenize] token allocated\n");
	
	init_token_vals(token);
	printf("[DEBUG tokenize] token values initialized\n");
	
	token->str = token_dup(line, token);
	if (!token->str)
	{
		print_mem_error("memory allocation failed", shell);
		free(token);
		return (NULL);
	}
	printf("[DEBUG tokenize] token->str = '%s'\n", token->str);
	
	token->type = WORD;
	set_type(token);
	printf("[DEBUG tokenize] token type set: %d\n", token->type);
	
	if (token->type == EV)
		token->does_exist = does_env_exist(token, &shell->envs);
	
	printf("[DEBUG tokenize] make_token end\n");
	return (token);
}

void	add_token(t_token **stack, t_token *new)
{
	t_token	*current;

	printf("[DEBUG tokenize] add_token start\n");
	if (new->type == EV && !new->does_exist)
	{
		if (new->str)
			free(new->str);
		free(new);
		return ;
	}
	if (!*stack)
	{
		*stack = new;
		return ;
	}
	current = *stack;
	while (current->next)
	{
		current = current->next;
	}
	current->next = new;
	new->prev = current;
	printf("[DEBUG tokenize] add_token end\n");
}

t_token	*tokenize(char *line, t_token **stack, t_shell *shell)
{
	t_token	*new;
	size_t	i;

	printf("[DEBUG tokenize] tokenize function start: line='%s'\n", line);
	
	printf("[DEBUG tokenize] before line_validator\n");
	if (!line_validator(line, shell))
	{
		printf("[DEBUG tokenize] line_validator failed\n");
		return (NULL);
	}
	printf("[DEBUG tokenize] after line_validator success\n");
	
	i = 0;
	while (line[i])
	{
		printf("[DEBUG tokenize] loop i=%zu, char='%c'\n", i, line[i]);
		if (!is_whitespace(line[i]))
		{
			printf("[DEBUG tokenize] found non-whitespace at %zu\n", i);
			new = make_token(&line[i], shell);
			if (!new)
			{
				printf("[DEBUG tokenize] make_token failed\n");
				free_sh_tokens(stack);
				return (NULL);
			}
			add_token(stack, new);
			size_t inc = increment_index(&line[i]);
			printf("[DEBUG tokenize] increment_index returned %zu\n", inc);
			i += inc;
		}
		else
		{
			i++;
		}
	}
	
	printf("[DEBUG tokenize] before token_validator\n");
	if (!token_validator(stack, shell))
	{
		printf("[DEBUG tokenize] token_validator failed\n");
		free_sh_tokens(stack);
		return (NULL);
	}
	printf("[DEBUG tokenize] tokenize function end success\n");
	return (*stack);
}
