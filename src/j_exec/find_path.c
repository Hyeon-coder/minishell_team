/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/27 23:39:34 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * ge_mini_shell의 search_in_paths 로직을 minishell_team 구조에 적용
 * PATH 디렉터리들에서 명령어 검색
 */
static char	*search_in_paths(char *cmd, char **paths)
{
	int		i;
	char	*path_part;
	char	*full_path;

	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		// 경로/명령어 조합 생성
		path_part = ft_strjoin(paths[i], "/");
		if (!path_part)
			return (NULL);
		
		full_path = ft_strjoin(path_part, cmd);
		free(path_part);
		if (!full_path)
			return (NULL);

		// 실행 가능한지 확인
		if (access(full_path, X_OK) == 0)
			return (full_path);
		
		free(full_path);
		i++;
	}
	
	return (NULL);
}

/**
 * ge_mini_shell의 find_command_path 로직을 minishell_team 구조에 적용
 * PATH 환경변수를 사용하여 명령어 경로 찾기
 */
char	*find_command_path(char *cmd, t_ms *ms)
{
	char	*path_env;
	char	**paths;
	char	*full_path;

	if (!cmd)
		return (NULL);

	// 절대경로나 상대경로인 경우
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}

	// PATH 환경변수 가져오기
	path_env = find_var(ms, ms->envp, "PATH");
	if (!path_env)
		return (NULL);

	// PATH를 ':'로 분할
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);

	// 각 경로에서 명령어 검색
	full_path = search_in_paths(cmd, paths);
	
	ft_free_array(paths);
	return (full_path);
}
