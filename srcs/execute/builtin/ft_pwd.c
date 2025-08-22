/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljh3900 <ljh3900@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 17:14:26 by ljh3900           #+#    #+#             */
/*   Updated: 2025/06/08 17:14:43 by ljh3900          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	ft_pwd_error(char **argv)
{
	if (argv[1])
	{
		ft_putstr_fd("pwd: options are not supported\n", 2);
		return (1);
	}
	return (0);
}

int	ft_pwd(char **argv)
{
	char *cwd;

	if (ft_pwd_error(argv))
		return (1);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr_fd("pwd: getcwd error\n", 2);
		return (1);
	}
	ft_putstr_fd(cwd, 1);
	ft_putchar_fd('\n', 1);
	free(cwd);
	return (0);
}
