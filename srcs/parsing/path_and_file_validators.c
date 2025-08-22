/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_and_file_validators.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 02:56:04 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_valid_file(char *str)
{
	struct stat	path_stat;

	if (!str)
		return (false);
	if (stat(str, &path_stat) != 0)
		return (false);
	return (S_ISREG(path_stat.st_mode));
}

bool	is_valid_dir(char *path)
{
	struct stat	path_stat;

	if (!path)
		return (false);
	if (stat(path, &path_stat) != 0)
		return (false);
	return (S_ISDIR(path_stat.st_mode));
}
