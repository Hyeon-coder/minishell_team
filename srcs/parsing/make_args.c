/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:09:48 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/20 23:40:45 by juhyeonl         ###   ########.fr       */
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
	while (str[i] && !is_separator(str[i]))
		i++;
	return (i);
}

char	*make_arg(char *str, t_shell *shell)
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
			name = make_name(&str[i]);
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

char	**args_creation_loop(t_token **tokens, char **args,
	t_shell *shell, size_t ac)
{
	size_t	i;
	t_token	*current;

	i = 0;
	current = *tokens;
	while (i < ac)
	{
		if (current->sq)
		{
			args[i] = custom_dup(current->str);
		}
		else
			args[i] = make_arg(current->str, shell);
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
		print_mem_error("minishell: memory allocation failed", shell);
		return (NULL);
	}
	args = args_creation_loop(tokens, args, shell, ac);
	if (args)
		args[ac] = NULL;
	return (args);
}
