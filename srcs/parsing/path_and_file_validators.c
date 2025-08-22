/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_and_file_validators.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:20:36 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/08/08 18:20:41 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_valid_dir(char *path, t_shell *shell)
{
	DIR	*dir;

	dir = opendir(path);
	if (!dir)
	{
		write_syntax_error("invalid directory", shell);
		return (false);
	}
	closedir(dir);
	return (true);
}

bool	is_valid_file(t_token *token, t_shell *shell)
{
	int	fd;

	fd = open(token->str, O_RDONLY);
	if (fd == -1)
	{
		write_syntax_error("invalid filename", shell);
		return (false);
	}
	close(fd);
	return (true);
}
