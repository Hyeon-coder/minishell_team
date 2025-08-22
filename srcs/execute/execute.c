/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:22:28 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/16 00:13:43 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static void	print_tokens_list(t_token *tok)
// {
// 	while (tok)
// 	{
// 		printf("[TOKEN] id=%zu type=%d str=\"%s\" sq=%d dq=%d\n",
// 			tok->id, tok->type,
// 			tok->str ? tok->str : "(null)",
// 			tok->sq, tok->dq);
// 		tok = tok->next;
// 	}
// }

// /* Print all commands for debugging */
// static void	print_commands_list(t_com *cmd)
// {
// 	int	i;

// 	while (cmd)
// 	{
// 		printf("[COMMAND] path=\"%s\" type=%d infile=\"%s\" outfile=\"%s\"",
// 			cmd->path ? cmd->path : "(null)",
// 			cmd->type,
// 			cmd->infile ? cmd->infile : "(null)",
// 			cmd->outfile ? cmd->outfile : "(null)");
// 		printf(" append=%d is_piped=%d\n", cmd->append, cmd->is_piped);
// 		if (cmd->args)
// 		{
// 			i = 0;
// 			while (cmd->args[i])
// 			{
// 				printf("   arg[%d] = \"%s\"\n", i, cmd->args[i]);
// 				i++;
// 			}
// 		}
// 		cmd = cmd->next;
// 	}
// }

// /* Print all environment variables for debugging */
// static void	print_env_list(t_env *env)
// {
// 	while (env)
// 	{
// 		printf("[ENV] %s=%s\n",
// 			env->name ? env->name : "(null)",
// 			env->value ? env->value : "(null)");
// 		env = env->next;
// 	}
// }

// /* Main debug print function */
// static void	print_shell_state(t_shell *sh)
// {
// 	if (!sh)
// 	{
// 		printf("[SHELL] (null)\n");
// 		return ;
// 	}
// 	printf("========== SHELL STATE ==========\n");
// 	printf("last_exit = %d\n", sh->last_exit);
// 	printf("---- TOKENS ----\n");
// 	print_tokens_list(sh->tokens);
// 	printf("---- COMMANDS ----\n");
// 	print_commands_list(sh->commands);
// 	printf("---- ENVS ----\n");
// 	print_env_list(sh->envs);
// 	printf("=================================\n");
// }

static int	count_cmds(t_com *cur)
{
	int	n;

	n = 0;
	while (cur)
	{
		n++;
		cur = cur->next;
	}
	return (n);
}

static int	has_redirs(t_com *cmd)
{
	if (!cmd)
		return (0);
	if (cmd->infile != NULL)
		return (1);
	if (cmd->outfile != NULL)
		return (1);
	/* If you have heredoc flags/fields, check them here as well. */
	return (0);
}

static int	needs_parent(const char *name)
{
	if (!name)
		return (0);
	if (!ft_strcmp(name, "cd"))
		return (1);
	if (!ft_strcmp(name, "export"))
		return (1);
	if (!ft_strcmp(name, "unset"))
		return (1);
	if (!ft_strcmp(name, "exit"))
		return (1);
	return (0);
}

int	execute(t_shell *sh)
{
	t_com	*cmd;
	int		n;

	cmd = sh->commands;
	if (!cmd)
		return (0);
	n = count_cmds(cmd);
	if (n == 1 && is_builtin(cmd->args[0]))
	{
		if (needs_parent(cmd->args[0]))
		{
			if (has_redirs(cmd))
			{
				sh->last_exit = run_builtin_parent_with_redirs(sh, cmd);
				return (sh->last_exit);
			}
			sh->last_exit = run_builtin_parent(cmd, sh);
			return (sh->last_exit);
		}
		if (!has_redirs(cmd))
		{
			sh->last_exit = run_builtin_parent(cmd, sh);
			return (sh->last_exit);
		}
	}
	return (exec_pipeline(sh, cmd, n));
}
