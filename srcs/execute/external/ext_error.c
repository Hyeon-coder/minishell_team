/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ext_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/21 23:21:56 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ext_print_perm_denied(const char *cmd)
{
	err_with_cmd(NULL, (char *)cmd, ": Permission denied");
}

void	ext_print_no_such_file(const char *cmd)
{
	err_with_cmd(NULL, (char *)cmd, ": No such file or directory");
}

void	ext_print_is_directory(const char *cmd)
{
	err_with_cmd(NULL, (char *)cmd, ": is a directory");
}

void	ext_print_cmd_not_found(const char *cmd)
{
	err_with_cmd(NULL, (char *)cmd, ": command not found");
}