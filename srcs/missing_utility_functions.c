/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   missing_utility_functions.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 12:00:00 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 구분자 확인 (make_args.c에서 사용) */
bool	is_separator(char c)
{
	return (c == ' || is_whitespace(c) || is_meta(c) || c == '\0');
}

/* 2D 배열 메모리 해제 */
void	ft_free_2d_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/* 에러 메시지 출력 */
void	err_with_cmd(char *prefix, char *cmd, char *suffix)
{
	if (prefix)
		ft_putstr_fd(prefix, 2);
	if (cmd)
		ft_putstr_fd(cmd, 2);
	if (suffix)
		ft_putstr_fd(suffix, 2);
}

/* 경로 해결 (기본 구현) */
char	*resolve_path(char *path, t_env *env_list, int *alloc_flag)
{
	t_env	*home;

	*alloc_flag = 0;
	
	if (!path)
	{
		/* HOME 디렉토리로 이동 */
		home = env_find(env_list, "HOME");
		if (home && home->value)
		{
			*alloc_flag = 1;
			return (ft_strdup(home->value));
		}
		return (NULL);
	}
	
	/* 절대경로나 상대경로 그대로 사용 */
	return (path);
}

/* 환경변수 이름 유효성 검사 */
int	is_valid_name(const char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	
	/* 첫 문자는 알파벳 또는 _ */
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	
	/* 나머지는 알파벳, 숫자, _ */
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/* export 정리 함수 */
int	export_cleanup(char *name, char *value, t_env *node, int exit_code)
{
	if (name)
		free(name);
	if (value)
		free(value);
	if (node)
		free(node);
	return (exit_code);
}

/* export 목록 출력 */
void	print_export_list(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		printf("declare -x %s", current->name);
		if (current->value && current->value[0])
			printf("=\"%s\"", current->value);
		printf("\n");
		current = current->next;
	}
}

/* n개 문자 복사 */
char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	i;

	if (!s)
		return (NULL);
	dup = malloc(n + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

/* PATH에서 명령어 찾기 */
char	*find_in_path(const char *cmd, t_env *env_list)
{
	char	**paths;
	t_env	*path_env;
	char	*full_path;
	char	*tmp;
	int		i;

	if (!cmd)
		return (NULL);
	
	/* PATH 환경변수 가져오기 */
	path_env = env_find(env_list, "PATH");
	if (!path_env || !path_env->value)
		return (NULL);
	
	paths = ft_split(path_env->value, ':');
	if (!paths)
		return (NULL);
	
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		
		if (access(full_path, X_OK) == 0)
		{
			ft_free_2d_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	
	ft_free_2d_array(paths);
	return (NULL);
}

/* 명령어 경로 가져오기 */
char	*get_cmd_path(const char *cmd, t_env *env_list)
{
	if (!cmd)
		return (NULL);
	
	/* 절대 경로 또는 상대 경로인 경우 */
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	
	/* PATH에서 찾기 */
	return (find_in_path(cmd, env_list));
}
