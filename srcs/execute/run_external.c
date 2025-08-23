/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_external.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:04:18 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/23 05:43:08 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 파일 상태를 확인하고 적절한 오류 메시지와 exit 코드를 결정 */
static int	check_file_executable(const char *path, char **error_msg)
{
	struct stat	st;

	if (stat(path, &st) != 0)
	{
		if (access(path, F_OK) == 0)
		{
			*error_msg = ": Permission denied\n";
			return (126);
		}
		else
		{
			*error_msg = ": No such file or directory\n";
			return (127);
		}
	}
	
	if (S_ISDIR(st.st_mode))
	{
		*error_msg = ": Is a directory\n";
		return (126);
	}
	
	if (access(path, X_OK) != 0)
	{
		*error_msg = ": Permission denied\n";
		return (126);
	}
	
	/* 실행 가능하지만 execve가 실패한 경우 */
	*error_msg = ": command not found\n";
	return (127);
}

/* Build a NULL-terminated "KEY=VAL" array from env list */
static char	**env_list_to_envp(t_env *env)
{
	size_t	count;
	t_env	*cur;
	char	**envp;
	char	*kv;

	count = 0;
	cur = env;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	cur = env;
	count = 0;
	while (cur)
	{
		kv = ft_strjoin(cur->name, "=");
		if (!kv)
			return (ft_free_2d_array(envp), NULL);
		envp[count] = ft_strjoin(kv, cur->value ? cur->value : "");
		free(kv);
		if (!envp[count])
			return (ft_free_2d_array(envp), NULL);
		count++;
		cur = cur->next;
	}
	envp[count] = NULL;
	return (envp);
}

/* Return 1 if s contains '/', which means a direct path was provided */
static int	has_slash(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

/* Get PATH variable value from env list (can be NULL) */
static char	*get_path_value(t_env *env_list)
{
	t_env	*node;

	node = env_find(env_list, "PATH");
	if (node)
		return (node->value);
	return (NULL);
}

/* Split PATH into array; can return NULL if no PATH */
static char	**split_paths(const char *pathvar)
{
	char	**paths;

	if (!pathvar)
		return (NULL);
	paths = ft_split(pathvar, ':');
	return (paths);
}

/* Try execve with given path; returns -1 if execve failed */
static int	try_exec(const char *path, char **argv, char **envp)
{
	if (!path)
		return (-1);
	execve(path, argv, envp);
	return (-1);
}

/* Iterate PATH entries and try execve for each candidate */
static int	try_exec_in_paths(char **argv, char **paths, char **envp)
{
	int		i;
	char	*tmp;
	char	*cand;

	if (!paths)
		return (-1);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (-1);
		cand = ft_strjoin(tmp, argv[0]);
		free(tmp);
		if (!cand)
			return (-1);
		(void)try_exec(cand, argv, envp);
		free(cand);
		i++;
	}
	return (-1);
}

/* Launch external program using PATH or direct path; no exit() here */
void	run_external(char **argv, t_env *env_list, t_shell *sh)
{
	char	**envp;
	char	**paths;
	char	*pathvar;
	char	*error_msg;
	int		exit_code;

	(void)sh;
	if (!argv || !argv[0] || argv[0][0] == '\0')
		return ;
	
	envp = env_list_to_envp(env_list);
	if (!envp)
	{
		perror("minishell: malloc");
		return ;
	}
	
	if (has_slash(argv[0]))
	{
		/* 직접 경로인 경우: 파일 상태를 먼저 확인 */
		exit_code = check_file_executable(argv[0], &error_msg);
		try_exec(argv[0], argv, envp);
		
		/* execve 실패 - 적절한 오류 메시지와 exit 코드 */
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)argv[0], 2);
		ft_putstr_fd(error_msg, 2);
		ft_free_2d_array(envp);
		exit(exit_code);
	}
	
	/* PATH에서 찾는 경우 */
	pathvar = get_path_value(env_list);
	paths = split_paths(pathvar);
	if (try_exec_in_paths(argv, paths, envp) < 0)
		err_with_cmd(NULL, (char *)argv[0], ": command not found\n");
	ft_free_2d_array(envp);
	ft_free_2d_array(paths);
	exit(127); /* PATH에서 찾을 수 없으면 127 */
}
