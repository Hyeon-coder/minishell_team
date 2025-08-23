/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:09:48 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/23 03:45:00 by juhyeonl         ###   ########.fr       */
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

/* 필요한 최종 크기를 미리 계산하는 함수 */
static size_t	calculate_final_size(const char *str, t_shell *shell)
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
			/* 환경변수 확장 */
			char *var_name;
			char *var_value;
			size_t var_len;
			
			i++; // '$' 건너뛰기
			var_len = 0;
			
			/* 변수명 길이 계산 */
			if (str[i] == '?')
			{
				var_len = 1;
			}
			else
			{
				while (str[i + var_len] && 
					  (ft_isalnum(str[i + var_len]) || str[i + var_len] == '_'))
					var_len++;
			}
			
			if (var_len == 0)
			{
				final_size++; // '$'만 있는 경우
				continue;
			}
			
			/* 변수명 추출 */
			var_name = malloc(var_len + 1);
			if (!var_name)
				return (0);
			ft_strlcpy(var_name, &str[i], var_len + 1);
			
			/* 환경변수 값 가져오기 */
			if (ft_strcmp(var_name, "?") == 0)
			{
				var_value = get_sig_val(shell->last_exit);
			}
			else
			{
				t_env *env_node = find_env(var_name, &shell->envs);
				var_value = env_node && env_node->value ? ft_strdup(env_node->value) : ft_strdup("");
			}
			
			free(var_name);
			if (!var_value)
				return (0);
			
			/* 환경변수 값의 길이를 최종 크기에 추가 */
			final_size += ft_strlen(var_value);
			free(var_value);
			i += var_len;
		}
		else
		{
			final_size++;
			i++;
		}
	}
	
	return (final_size + 1); // null terminator 포함
}

/* 개선된 따옴표 제거 및 환경변수 확장 함수 */
static char	*process_quotes_and_expand(const char *str, t_shell *shell, bool expand_vars)
{
	char	*result;
	size_t	i;
	size_t	j;
	size_t	final_size;
	bool	in_single_quote;
	bool	in_double_quote;

	if (!str)
		return (ft_strdup(""));
	
	/* 최종 크기를 미리 계산 */
	final_size = calculate_final_size(str, shell);
	if (final_size == 0)
		return (NULL);
	
	result = malloc(final_size);
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
		else if (str[i] == '$' && expand_vars && !in_single_quote && str[i + 1])
		{
			/* 환경변수 확장 */
			char *var_name;
			char *var_value;
			size_t var_len;
			size_t value_len;
			
			i++; // '$' 건너뛰기
			var_len = 0;
			
			/* 변수명 길이 계산 */
			if (str[i] == '?')
			{
				var_len = 1;
			}
			else
			{
				while (str[i + var_len] && 
					  (ft_isalnum(str[i + var_len]) || str[i + var_len] == '_'))
					var_len++;
			}
			
			if (var_len == 0)
			{
				result[j++] = '$'; // '$'만 있는 경우
				continue;
			}
			
			/* 변수명 추출 */
			var_name = malloc(var_len + 1);
			if (!var_name)
			{
				free(result);
				return (NULL);
			}
			ft_strlcpy(var_name, &str[i], var_len + 1);
			
			/* 환경변수 값 가져오기 */
			if (ft_strcmp(var_name, "?") == 0)
			{
				var_value = get_sig_val(shell->last_exit);
			}
			else
			{
				t_env *env_node = find_env(var_name, &shell->envs);
				var_value = env_node && env_node->value ? ft_strdup(env_node->value) : ft_strdup("");
			}
			
			free(var_name);
			if (!var_value)
			{
				free(result);
				return (NULL);
			}
			
			/* 환경변수 값 복사 (안전하게) */
			value_len = ft_strlen(var_value);
			if (j + value_len < final_size)
			{
				ft_strlcpy(&result[j], var_value, value_len + 1);
				j += value_len;
			}
			i += var_len;
			
			free(var_value);
		}
		else
		{
			if (j < final_size - 1) // null terminator를 위한 공간 확보
				result[j++] = str[i++];
			else
				i++; // 버퍼 오버플로우 방지
		}
	}
	
	result[j] = '\0';
	return (result);
}

char	**args_creation_loop(t_token **tokens, char **args,
	t_shell *shell, size_t ac)
{
	size_t	i;
	t_token	*current;

	i = 0;
	current = *tokens;
	
	while (i < ac && current)
	{
		/* 리디렉션 토큰들은 건너뛰기 */
		while (current && (current->type == RD_I || current->type == RD_O || 
						   current->type == RD_O_APPEND || current->type == HERE_DOC))
		{
			current = current->next;
			if (current) /* 파일명도 건너뛰기 */
				current = current->next;
		}
		
		if (!current)
			break;
		
		if (current->type == PIPE)
		{
			current = current->next;
			break;
		}
		
		/* 단일 따옴표가 있으면 환경변수 확장하지 않음 */
		bool expand_vars = !current->sq;
		
		args[i] = process_quotes_and_expand(current->str, shell, expand_vars);
		if (!args[i])
		{
			/* 메모리 해제 */
			while (i > 0)
			{
				i--;
				free(args[i]);
			}
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
	size_t	ac;
	char	**args;
	
	ac = count_args(tokens);
	if (ac == 0)
		return (NULL);
		
	args = (char **)malloc((ac + 1) * sizeof(char *));
	if (!args)
	{
		print_mem_error("memory allocation failed", shell);
		return (NULL);
	}
	
	/* 배열 초기화 */
	for (size_t i = 0; i <= ac; i++)
		args[i] = NULL;
	
	args = args_creation_loop(tokens, args, shell, ac);
	if (!args)
	{	
		print_mem_error("memory allocation failed", shell);
		return (NULL);
	}
	
	return (args);
}
