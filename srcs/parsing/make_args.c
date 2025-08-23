/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:09:48 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/23 06:00:38 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 환경변수명이 유효한지 확인 */
static int	is_valid_var_start(char c)
{
	return (ft_isalpha(c) || c == '_' || c == '?');
}

static int	is_valid_var_continue(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/* 유효한 환경변수명의 길이를 계산 (0이면 유효하지 않음) */
static size_t	get_valid_var_len(const char *str)
{
	size_t	len;
	
	if (!str || !str[0])
		return (0);
	
	/* 특수 변수 $? */
	if (str[0] == '?')
		return (1);
	
	/* 첫 글자가 유효하지 않으면 */
	if (!is_valid_var_start(str[0]))
		return (0);
	
	len = 1;
	/* 나머지 글자들 검사 */
	while (str[len] && is_valid_var_continue(str[len]))
		len++;
	
	return (len);
}

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
	i = 0;
	arg = NULL;
	
	if (!str)
		return (ft_strdup(""));
	
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			name = make_name(&str[i + 1], is_dq);
			if (!name)
				return (got_envs ? free(arg), NULL : NULL);
			
			if (!got_envs)
				arg = env_parse_handler(str, name, shell, got_envs);
			else
				arg = env_parse_handler(arg, name, shell, got_envs);
			got_envs = true;
			if (!arg)
				return (NULL);
			i += arg_mover(&str[i]);
		}
		else
			i++;
	}
	if (!got_envs)
		arg = ft_strdup(str);
	return (arg);
}

/* 단일따옴표로 둘러싸인 토큰의 따옴표만 제거 */
static char	*remove_single_quotes_only(const char *str)
{
	char	*result;
	size_t	i;
	size_t	j;
	size_t	len;

	if (!str)
		return (ft_strdup(""));
	
	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			i++; // 단일따옴표 건너뛰기
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

/* 환경변수 확장 시 필요한 최종 크기를 계산하는 함수 */
static size_t	calculate_expanded_size(const char *str, t_shell *shell)
{
	size_t	final_size;
	size_t	i;
	bool	in_single_quote;
	bool	in_double_quote;

	if (!str)
		return (1);
	
	final_size = 0;
	i = 0;
	in_single_quote = false;
	in_double_quote = false;
	
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			i++; // 따옴표는 최종 결과에 포함되지 않음
		}
		else if (str[i] == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			i++; // 따옴표는 최종 결과에 포함되지 않음
		}
		else if (str[i] == '$' && !in_single_quote && str[i + 1])
		{
			size_t	var_len = get_valid_var_len(&str[i + 1]);
			if (var_len == 0)
			{
				final_size++;
				i++;
				continue;
			}
			
			char	*var_name = malloc(var_len + 1);
			if (!var_name)
				return (0);
			ft_strlcpy(var_name, &str[i + 1], var_len + 1);
			
			char	*var_value;
			if (ft_strcmp(var_name, "?") == 0)
				var_value = get_sig_val(shell->last_exit);
			else
			{
				t_env *env_node = find_env(var_name, &shell->envs);
				var_value = (env_node && env_node->value) ? ft_strdup(env_node->value) : ft_strdup("");
			}
			
			free(var_name);
			if (!var_value)
				return (0);
			
			final_size += ft_strlen(var_value);
			free(var_value);
			i += var_len + 1;
		}
		else
		{
			final_size++;
			i++;
		}
	}
	
	return (final_size + 1);
}

/* 따옴표 제거 및 환경변수 확장 */
static char	*process_quotes_and_expand(const char *str, t_shell *shell)
{
	size_t	final_size = calculate_expanded_size(str, shell);
	if (final_size == 0)
		return (NULL);
	
	char	*result = malloc(final_size);
	if (!result)
		return (NULL);
	
	size_t	i = 0, j = 0;
	bool	in_single_quote = false;
	bool	in_double_quote = false;
	
	while (str[i] && j < final_size - 1)
	{
		if (str[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			i++;
		}
		else if (str[i] == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			i++;
		}
		else if (str[i] == '$' && !in_single_quote && str[i + 1])
		{
			size_t	var_len = get_valid_var_len(&str[i + 1]);
			if (var_len == 0)
			{
				result[j++] = '$';
				i++;
				continue;
			}
			
			char	*var_name = malloc(var_len + 1);
			if (!var_name)
			{
				free(result);
				return (NULL);
			}
			ft_strlcpy(var_name, &str[i + 1], var_len + 1);
			
			char	*var_value;
			if (ft_strcmp(var_name, "?") == 0)
				var_value = get_sig_val(shell->last_exit);
			else
			{
				t_env *env_node = find_env(var_name, &shell->envs);
				var_value = (env_node && env_node->value) ? ft_strdup(env_node->value) : ft_strdup("");
			}
			
			free(var_name);
			if (!var_value)
			{
				free(result);
				return (NULL);
			}
			
			size_t	value_len = ft_strlen(var_value);
			for (size_t k = 0; k < value_len && j < final_size - 1; k++)
				result[j++] = var_value[k];
			
			free(var_value);
			i += var_len + 1;
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	
	result[j] = '\0';
	return (result);
}

/* 빈 문자열 인자들을 필터링하는 함수 */
static char	**filter_empty_args(char **args)
{
	if (!args)
		return (NULL);
	
	size_t	count = 0;
	for (size_t i = 0; args[i]; i++)
		if (args[i][0] != '\0')
			count++;
	
	if (count == 0)
	{
		free_args(args);
		return (NULL);
	}
	
	char	**filtered = malloc((count + 1) * sizeof(char *));
	if (!filtered)
	{
		free_args(args);
		return (NULL);
	}
	
	size_t	j = 0;
	for (size_t i = 0; args[i]; i++)
	{
		if (args[i][0] != '\0')
		{
			filtered[j++] = args[i];
		}
		else
		{
			free(args[i]);
		}
	}
	filtered[j] = NULL;
	
	free(args);
	return (filtered);
}

char	**args_creation_loop(t_token **tokens, char **args,
	t_shell *shell, size_t ac)
{
	size_t	i = 0;
	t_token	*current = *tokens;
	
	while (i < ac && current)
	{
		/* 리디렉션 토큰들은 건너뛰기 */
		while (current && (current->type == RD_I || current->type == RD_O || 
						   current->type == RD_O_APPEND || current->type == HERE_DOC))
		{
			current = current->next;
			if (current)
				current = current->next;
		}
		
		if (!current || current->type == PIPE)
			break;

		/* 토큰의 따옴표 플래그에 따른 처리 */
		if (current->sq && !current->dq)
		{
			/* 단일따옴표: 환경변수 확장 안 함 */
			args[i] = remove_single_quotes_only(current->str);
		}
		else
		{
			/* 그 외: 환경변수 확장 및 따옴표 제거 */
			args[i] = process_quotes_and_expand(current->str, shell);
		}
		
		if (!args[i])
		{
			for (size_t k = 0; k < i; k++)
				free(args[k]);
			free(args);
			return (NULL);
		}
		
		current = current->next;
		i++;
	}
	
	return (args);
}

char	**make_args(t_token **tokens, t_shell *shell)
{
	size_t	ac = count_args(tokens);
	if (ac == 0)
		return (NULL);
		
	char	**args = malloc((ac + 1) * sizeof(char *));
	if (!args)
	{
		print_mem_error("memory allocation failed", shell);
		return (NULL);
	}
	
	for (size_t i = 0; i <= ac; i++)
		args[i] = NULL;
	
	args = args_creation_loop(tokens, args, shell, ac);
	if (!args)
	{
		print_mem_error("memory allocation failed", shell);
		return (NULL);
	}
	
	/* 빈 문자열 인자들 필터링 */
	args = filter_empty_args(args);
	return (args);
}
