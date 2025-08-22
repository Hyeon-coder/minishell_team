#include "../../includes/minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static char	**stub_split_ws(const char *line)
{
	return ft_split(line, ' ');
}

static t_cmd	*stub_parse(const char *line, size_t *out_n)
{
	char   **tok = stub_split_ws(line);
	if (!tok)
		return NULL;
	size_t cmd_cnt = 1;
	for (size_t i = 0; tok[i]; ++i)
		if (ft_strcmp(tok[i], "|") == 0)
			++cmd_cnt;
	t_cmd *v = ft_calloc(cmd_cnt, sizeof(t_cmd));
	if (!v)
		return (ft_free_2d_array(tok), NULL);
	for (size_t i = 0; i < cmd_cnt; ++i)
		v[i].fd_in = v[i].fd_out = -1;
	size_t ci = 0;
	for (size_t i = 0; tok[i]; ++i)
	{
		if (ft_strcmp(tok[i], "|") == 0)
		{
			++ci;
			continue;
		}
		if (ft_strcmp(tok[i], "<") == 0 && tok[i + 1])
		{
			v[ci].fd_in = open(tok[i + 1], O_RDONLY);
			++i;
			continue;
		}
		if ((ft_strcmp(tok[i], ">")  == 0 ||
			 ft_strcmp(tok[i], ">>") == 0) && tok[i + 1])
		{
			v[ci].append = (tok[i][1] == '>');
			int flags = O_WRONLY | O_CREAT |
						(v[ci].append ? O_APPEND : O_TRUNC);
			v[ci].fd_out = open(tok[i + 1], flags, 0644);
			++i;
			continue;
		}
		size_t alen = 0;
		if (v[ci].argv)
			while (v[ci].argv[alen])
				++alen;
		char **tmp = ft_calloc(alen + 2, sizeof(char *));
		if (!tmp)
			return (ft_free_2d_array(tok), NULL);
		for (size_t k = 0; k < alen; ++k)
			tmp[k] = v[ci].argv[k];
		tmp[alen] = ft_strdup(tok[i]);
		tmp[alen + 1] = NULL;
		free(v[ci].argv);
		v[ci].argv = tmp;
	}
	ft_free_2d_array(tok);
	*out_n = cmd_cnt;
	return v;
}

static void	stub_free_cmds(t_cmd *v, size_t n)
{
	for (size_t i = 0; i < n; ++i)
	{
		ft_free_2d_array(v[i].argv);
		if (v[i].fd_in  != -1)  close(v[i].fd_in);
		if (v[i].fd_out != -1)  close(v[i].fd_out);
	}
	free(v);
}

static void	stub_child_exec(t_cmd *c, t_shell *sh)
{
	if (c->argv && is_builtin(c->argv[0]))
	{
		handle_builtin(c->argv, &sh->envs, sh);
		exit(0);
	}
	if (c->argv)
		execvp(c->argv[0], c->argv);
	perror("execvp");
	exit(127);
}

int	execute_stub_line(const char *line, t_shell *sh)
{
	size_t  n;
	t_cmd  *cmd = stub_parse(line, &n);
	if (!cmd || n == 0)
		return (stub_free_cmds(cmd, n), 1);

	int prev_read = -1;
	for (size_t i = 0; i < n; ++i)
	{
		int fd[2] = {-1, -1};
		if (i + 1 < n && pipe(fd) == -1)
			return (perror("pipe"), stub_free_cmds(cmd, n), 1);

		pid_t pid = fork();
		if (pid == 0)
			stub_child_exec(&cmd[i], sh);
		if (prev_read != -1)
			close(prev_read);
		if (i + 1 < n) {
			close(fd[1]);
			prev_read = fd[0];
		}
	}
	int status = 0;
	while (wait(&status) > 0)
		;
	if (WIFEXITED(status))
		sh->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->last_exit = 128 + WTERMSIG(status);
	else
		sh->last_exit = 1;
	stub_free_cmds(cmd, n);
	return 0;
}
