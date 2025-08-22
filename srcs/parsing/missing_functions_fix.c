/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   missing_functions_fix.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 22:15:11 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	com_path_setter(char *str, t_com *com)
{
	if (!str || !com)
		return ;
	if (com->type != WORD)
		return ;
	if (ft_strncmp("./", str, 2) == 0)
		com->type = PATH;
	else if (ft_strncmp("/", str, 1) == 0)
		com->type = PATH;
	else if (ft_strcmp("cd", str) == 0)
		com->type = CD;
	else if (ft_strcmp("env", str) == 0)
		com->type = ENV;
}

void	set_type(t_token *token)
{
	if (!token || !token->str)
		return ;
	
	printf("[DEBUG] set_type: checking token '%s'\n", token->str);
	
	if (ft_strcmp(token->str, "|") == 0)
		token->type = PIPE;
	else if (ft_strcmp(token->str, "<") == 0)
		token->type = RD_I;
	else if (ft_strcmp(token->str, ">") == 0)
		token->type = RD_O;
	else if (ft_strcmp(token->str, ">>") == 0)
		token->type = RD_O_APPEND;
	else if (ft_strcmp(token->str, "<<") == 0)
		token->type = HERE_DOC;
	else if (ft_strcmp(token->str, "echo") == 0)
		token->type = ECHO;
	else if (ft_strcmp(token->str, "cd") == 0)
		token->type = CD;
	else if (ft_strcmp(token->str, "pwd") == 0)
		token->type = PWD;
	else if (ft_strcmp(token->str, "export") == 0)
		token->type = EXPORT;
	else if (ft_strcmp(token->str, "unset") == 0)
		token->type = UNSET;
	else if (ft_strcmp(token->str, "env") == 0)
		token->type = ENV;
	else if (ft_strcmp(token->str, "exit") == 0)
		token->type = EXIT;
	else if (ft_strcmp(token->str, "-n") == 0)
		token->type = N;
	else if (token->str[0] == '$' && !token->sq)
		token->type = EV;
	else
		token->type = WORD;
	
	printf("[DEBUG] set_type: final type = %d\n", token->type);
}

int	setup_redirections(t_com *cmd)
{
	int	fd;

	if (!cmd)
		return (0);
	if (cmd->redir_type_in && cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
		{
			perror(cmd->infile);
			return (-1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->redir_type_out && cmd->outfile)
	{
		if (cmd->append)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror(cmd->outfile);
			return (-1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (0);
}

bool	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
		return (true);
	if (c == '\v' || c == '\f')
		return (true);
	return (false);
}

bool	is_rd(char c)
{
	return (c == '<' || c == '>');
}

size_t	handle_rd(char *line)
{
	if (!line)
		return (0);
	if (line[0] == '<' || line[0] == '>')
	{
		if (line[1] == line[0])
			return (2);
		return (1);
	}
	return (0);
}

size_t	increment_index(char *line)
{
	size_t	i;

	i = 0;
	if (!line)
		return (0);
	if (line[i] == '|')
		return (1);
	if (is_rd(line[i]))
		return (handle_rd(line));
	if (!is_whitespace(line[i]) && !is_separator(line[i]))
	{
		if (i == 0 && line[i])
			i++;
		if (i < ft_strlen(line) && line[i] && 
			!is_whitespace(line[i]) && !is_separator(line[i]))
			i++;
		if (i < ft_strlen(line) && line[i] && 
			!is_whitespace(line[i]) && !is_separator(line[i]))
			i++;
		if (i < ft_strlen(line) && line[i] && 
			!is_whitespace(line[i]) && !is_separator(line[i]))
			i++;
		if (i < ft_strlen(line) && line[i] && 
			!is_whitespace(line[i]) && !is_separator(line[i]))
			i++;
	}
	return (i);
}
