/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:30:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 01:21:23 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_argv(char **argv)
{
	int	count;

	count = 0;
	while (argv[count])
		count++;
	return (count);
}

int	ft_pwd(char **argv)
{
	char	*cwd;
	int		argc;
	
	argc = count_argv(argv);
	
	// pwd는 인자를 받지 않음
	if (argc > 1)
	{
		ft_putstr_fd("pwd: options are not supported\n", 2);
		return (1);
	}
	
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (0);
}
