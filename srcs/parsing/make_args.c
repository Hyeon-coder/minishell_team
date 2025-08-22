/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:09:48 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/22 16:39:50 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_com_type(char *str, t_com *token)
{
	token->type = WORD;
	if (!ft_strcmp("echo", str))
		token->type = ECHO;
	if (!ft_strcmp("pwd", str))
		token->type = PWD;
	if (!ft_strcmp("exit", str))
		token->type = EXIT;
	if (!ft_strcmp("|", str))
		token->type = PIPE;
	if (!ft_strcmp("unset", str))
		token->type = UNSET;
	if (!ft_strcmp("export", str))
		token->type = EXPORT;
	if (!ft_strcmp("-n", str))
		token->type = N;
	if (!ft_strcmp(">", str))
		token->type = RD_O;
	if (!ft_strcmp(">>", str))
		token->type = RD_O_APPEND;
	if (!ft_strcmp("<", str))
		token->type = RD_I;
	if (!ft_strcmp("<<", str))
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
	i = 0;
	arg = NULL;
	while (str[i])
	{
		if (str[i] == '$')
		{
			name = make_name(&str[i]);
			if (name)
			{
				if (!got_envs)
					arg = env_parse_handler(str, name, shell, got_envs);
				else
					arg = env_parse_handler(arg, name, shell, got_envs);
				got_envs = true;
				free(name);
				if (!arg)
					return (NULL);
				i += arg_mover(&str[i]);
			}
			else
				i++;
		}
		else
			i++;
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
	while (i < ac && current)
	{
		if (current->type == PIPE)
			break;
		if (current->type != RD_I && current->type != RD_O 
			&& current->type != RD_O_APPEND && current->type != HERE_DOC)
		{
			if (current->sq)
				args[i] = custom_dup(current->str);
			else
				args[i] = make_arg(current->str, shell);
			if (!args[i])
			{
				free_args(args);
				return (NULL);
			}
			i++;
		}
		current = current->next;
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
		print_mem_error("minishell: memory allocation failed", shell);
		return (NULL);
	}
	args = args_creation_loop(tokens, args, shell, ac);
	if (args)
		args[ac] = NULL;
	return (args);
}
