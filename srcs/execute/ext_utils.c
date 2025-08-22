/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ext_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 02:47:29 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ----- env list -> envp ("KEY=VAL") ----- */
static int	append_kv(char **dst, size_t *i, t_env *cur)
{
	char	*kv;

	kv = ft_strjoin(cur->name, "=");
	if (!kv)
		return (-1);
	dst[*i] = ft_strjoin(kv, cur->value ? cur->value : "");
	free(kv);
	if (!dst[*i])
		return (-1);
	(*i)++;
	return (0);
}

char	**ext_env_to_envp(t_env *env)
{
	size_t	count;
	size_t	i;
	t_env	*cur;
	char	**envp;

	count = 0;
	cur = env;
	while (cur && ++count)
		cur = cur->next;
	envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	cur = env;
	while (cur)
	{
		if (append_kv(envp, &i, cur) < 0)
			return (ft_free_2d_array(envp), NULL);
		cur = cur->next;
	}
	envp[i] = NULL;
	return (envp);
}

/* ----- small utils ----- */
int	ext_has_slash(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		if (s[i++] == '/')
			return (1);
	return (0);
}

char	*ext_get_path_value(t_env *env_list)
{
    t_env *node;

    node = env_find(env_list, "PATH");
    if (node && node->value)
        return node->value;
    return getenv("PATH");
}

char	**ext_split_paths(const char *pathvar)
{
	if (!pathvar)
		return (NULL);
	return (ft_split(pathvar, ':'));
}

int	ext_errno_to_exit(int e)
{
	if (e == EACCES)
		return (126);
	if (e == ENOENT)
		return (127);
	return (126);
}

int	ext_try_exec(const char *path, char **argv, char **envp)
{
	if (!path)
		return (ENOENT);
	execve(path, argv, envp);
	return (errno);
}

/* ----- error printers ----- */
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
