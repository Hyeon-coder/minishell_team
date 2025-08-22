/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:09:48 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/23 02:18:27 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_com_type(char *str, t_com *token)
{
	token->type = WORD;
	if (ftstrcmp("echo", str))
		token->type = ECHO;
	if (ftstrcmp("pwd", str))
		token->type = PWD;
	if (ftstrcmp("exit", str))
		token->type = EXIT;
	if (ftstrcmp("|", str))
		token->type = PIPE;
	if (ftstrcmp("unset", str))
		token->type = UNSET;
	if (ftstrcmp("export", str))
		token->type = EXPORT;
	if (ftstrcmp("-n", str))
		token->type = N;
	if (ftstrcmp(">", str))
		token->type = RD_O;
	if (ftstrcmp(">>", str))
		token->type = RD_O_APPEND;
	if (ftstrcmp("<", str))
		token->type = RD_I;
	if (ftstrcmp("<<", str))
		token->type = HERE_DOC;
	com_path_setter(str, token);
}

size_t	arg_mover(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (i);
	while (str[i] && !is_whitespace(str[i]))
		i++;
	return (i);
}

char	*make_arg(char *str, t_shell *shell, bool is_dq)
{
	size_t	i;
	char	*arg;
	char	*name;
	bool	got_envs;

	got_envs = false;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '$')
		{
			name = make_name(&str[i + 1], is_dq);
			if (!got_envs)
				arg = env_parse_handler(str, name, shell, got_envs);
			else
				arg = env_parse_handler(arg, name, shell, got_envs);
			got_envs = true;
			if (!arg)
				return (NULL);
			i += arg_mover(&str[i]);
		}
	}
	if (!got_envs)
		arg = custom_dup(str);
	return (arg);
}

static char	*remove_quotes(const char *str, bool sq, bool dq)
{
	char	*result;
	size_t	len;
	size_t	i;
	size_t	j;

	if (!str)
		return (NULL);
	
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	
	// 단일 따옴표 제거
	if (sq && str[0] == '\'' && str[len - 1] == '\'')
	{
		result = malloc(len - 1);
		if (!result)
			return (NULL);
		i = 1;
		j = 0;
		while (i < len - 1)
			result[j++] = str[i++];
		result[j] = '\0';
		return (result);
	}
	
	// 이중 따옴표 제거
	if (dq && str[0] == '"' && str[len - 1] == '"')
	{
		result = malloc(len - 1);
		if (!result)
			return (NULL);
		i = 1;
		j = 0;
		while (i < len - 1)
			result[j++] = str[i++];
		result[j] = '\0';
		return (result);
	}
	
	return (ft_strdup(str));
}

// args_creation_loop 함수 수정
char	**args_creation_loop(t_token **tokens, char **args,
	t_shell *shell, size_t ac)
{
	size_t	i;
	t_token	*current;
	char	*temp;

	i = 0;
	current = *tokens;
	while (i < ac)
	{
		if (current->sq)
		{
			// 단일 따옴표: 환경변수 확장 없이 따옴표만 제거
			args[i] = remove_quotes(current->str, true, false);
		}
		else if (current->dq)
		{
			// 이중 따옴표: 환경변수 확장 후 따옴표 제거
			temp = make_arg(current->str, shell, current->dq);
			if (!temp)
			{
				free_args(args);
				return (NULL);
			}
			args[i] = remove_quotes(temp, false, true);
			free(temp);
		}
		else
		{
			// 따옴표 없음: 환경변수 확장만
			args[i] = make_arg(current->str, shell, current->dq);
		}
		
		if (!args[i])
		{
			free_args(args);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	return (args);
}

char	**make_args(t_token **tokens, t_shell *shell)
{
	size_t	ac;
	char	**args;
	
	ac = count_args(tokens);
	args = (char **)malloc((ac + 1) * sizeof(char *));
	if (!args)
	{
		print_mem_error("memory allocation failed", shell);
		return (NULL);
	}
	args = args_creation_loop(tokens, args, shell, ac);
	if (!args)
	{	
		print_mem_error("memory allocation failed", shell);
		return (NULL);
	}
	args[ac] = NULL;
	return (args);
}
