/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 13:47:44 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/28 14:02:57 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ms_error(t_ms *ms, char *msg, int ex_code, int free_msg)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	else
		ft_putstr_fd("Failed to get error message\n", 2);
	if (free_msg == 1)
		free(msg);
	bi_exit(ms, ex_code, 1);
}

char	*find_var(t_ms *ms, char **envp, char *name)
{
	int		i;
	int		j;
	char	*path;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		path = x_substr(ms, envp[i], 0, j);
		if (!ft_strcmp(path, name))
		{
			free(path);
			return (envp[i] + j + 1);
		}
		free(path);
		i++;
	}
	return (NULL);
}

bool	export_var_to_envp(t_ms *ms, char *var)
{
	int		i;
	char	*name;
	bool	append;

	i = -1;
	append = false;
	if (!var)
	{
		export_print_env(ms);
		return (true);
	}
	if (var[0] == '_' && (var[1] == 0 || var[1] == '='))
		return (true);
	while (var[++i] != '=' && var[i])
		if (var[i + 1] && var[i] == '+' && var[i + 1] == '=')
			append = true;
	name = x_substr(ms, var, 0, i - append);
	if (check_valid_identifier(ms, name) == false)
	{
		invalid_export(ms, name);
		free(name);
		return (false);
	}
	run_export(ms, var, name, append);
	return (true);
}

bool	export_var_to_envp(t_ms *ms, char *var)
{
	int		i;
	char	*name;
	bool	append;

	i = -1;
	append = false;
	if (!var)
	{
		export_print_env(ms);
		return (true);
	}
	if (var[0] == '_' && (var[1] == 0 || var[1] == '='))
		return (true);
	while (var[++i] != '=' && var[i])
		if (var[i + 1] && var[i] == '+' && var[i + 1] == '=')
			append = true;
	name = x_substr(ms, var, 0, i - append);
	if (check_valid_identifier(ms, name) == false)
	{
		invalid_export(ms, name);
		free(name);
		return (false);
	}
	run_export(ms, var, name, append);
	return (true);
}

void	arr_dup_fail(t_ms *ms, char **arr, int j)
{
	while (j--)
		free(arr[j]);
	free(arr);
	ms_error(ms, "Array duplication failed", 1, 0);
}

/*
** Creates unique heredoc name
** Starts at .heredoc0
*/
char	*heredoc_name(t_ms *ms, int i)
{
	char	*name;
	char	*no;

	no = ft_itoa(i);
	if (!no)
		ms_error(ms, "ft_itoa failure", 1, 0);
	name = ft_strjoin(".heredoc", no);
	free(no);
	if (!name)
		ms_error(ms, "heredoc file generation failure", 1, 0);
	return (name);
}

int	handle_heredoc(t_ms *ms, const char *heredoc, char *name, int quo)
{
	char	*line;
	int		fd;
	char	*temp;

	temp = NULL;
	fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		ms_error(ms, "Failed to create temporary file for HEREDOC", 1, 0);
	while (1)
	{
		if (isatty(fileno(stdin)))
			line = readline("> ");
		else
			line = get_line(temp, line);
		if (!line || ft_strcmp(line, heredoc) == 0 || g_signal == SIGINT)
		{
			if (readline_break(line, fd) < 0)
				return (-1);
			break ;
		}
		heredoc_help(ms, line, fd, quo);
	}
	close(fd);
	return (0);
}

void	export_print_env(t_ms *ms)
{
	char	**temp;
	int		i;

	i = -1;
	temp = malloc((ms->elements + 1) * sizeof(char *));
	if (!temp)
		ms_error(ms, "envp allocation failed", 1, 0);
	while (++i < ms->elements)
		temp[i] = ms->envp[i];
	temp[i] = NULL;
	sort_env(temp);
	i = -1;
	while (temp[++i])
	{
		if (!(temp[i][0] == '_' && temp[i][1] == '='))
			printf("declare -x %s\n", temp[i]);
	}
	free(temp);
}
