/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_external.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:58:20 by you               #+#    #+#             */
/*   Updated: 2025/08/17 18:01:38 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Entry point called in child:
** - On return (failure), caller must: exit(sh->last_exit)
** - On success, execve never returns.
*/
void	run_external(char **argv, t_env *env_list, t_shell *sh)
{
	char	**envp;

	if (!argv || !argv[0] || argv[0][0] == '\0')
	{
		sh->last_exit = 0;
		return ;
	}
	envp = ext_env_to_envp(env_list);
	if (!envp)
	{
		perror("minishell: malloc");
		sh->last_exit = 1;
		return ;
	}
	if (ext_has_slash(argv[0]))
		ext_exec_direct_path(argv, envp, sh);
	else
		ext_exec_with_path_search(argv, env_list, envp, sh);
	ft_free_2d_array(envp);
}
