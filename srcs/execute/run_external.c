/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_external.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 13:49:31 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* PATH에서 실행 파일 찾기 */
static char	*find_in_path(char *cmd, t_env *env_list)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	char	*tmp;
	int		i;

	path_env = get_env_value("PATH", env_list);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
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
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}

/* 명령어 경로 찾기 */
static char	*get_cmd_path(char *cmd, t_env *env_list)
{
	if (!cmd || !*cmd)
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

/* 환경 변수 배열 생성 */
char	**env_to_array(t_env *env_list)
{
	t_env	*current;
	char	**envp;
	char	*tmp;
	int		count;
	int		i;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	current = env_list;
	i = 0;
	while (current)
	{
		tmp = ft_strjoin(current->name, "=");
		envp[i] = ft_strjoin(tmp, current->value);
		free(tmp);
		if (!envp[i])
		{
			while (--i >= 0)
				free(envp[i]);
			free(envp);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

/* 외부 명령어 실행 (자식 프로세스용) */
void	run_external(char **argv, t_env *env_list, t_shell *sh)
{
	char	*cmd_path;
	char	**envp;

	(void)sh;
	cmd_path = get_cmd_path(argv[0], env_list);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	
	envp = env_to_array(env_list);
	if (!envp)
	{
		free(cmd_path);
		exit(1);
	}
	
	execve(cmd_path, argv, envp);
	
	/* execve 실패 시 */
	ft_putstr_fd("minishell: ", 2);
	perror(argv[0]);
	free(cmd_path);
	free_split(envp);
	exit(126);
}

/* 외부 명령어 실행 (부모 프로세스용) */
int	execute_external_command(t_com *cmd, t_shell *sh)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	
	if (pid == 0)
	{
		/* 자식 프로세스 */
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		run_external(cmd->args, sh->envs, sh);
		exit(127);
	}
	
	/* 부모 프로세스 */
	waitpid(pid, &status, 0);
	
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
