/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 00:35:58 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 11:53:38 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* System Headers */
# include <stdio.h>
# include <errno.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h>
# include <limits.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

/* Macros */
# define INITIAL_CAP 16
# define TRUE 1
# define FALSE 0
# define GRN  "\033[1;32m"
# define RESET "\033[0m"

/* Token Types */
enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HERE_DOC,
	CD,
	EXPORT,
	ENV,
	ECHO,
	UNSET,
	EXIT,
	PWD
};

/* Structures */
typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_com
{
	char			**argv;
	char			*path;
	int				type;
	t_token			*redirections;
	struct s_com	*next;
}	t_com;

typedef struct s_shell
{
	t_com	*commands;
	t_token	*tokens;
	t_env	*envs;
	int		last_exit;
}	t_shell;

/* Global signal variable */
extern volatile sig_atomic_t	g_signal;

/* Main Functions */
void	setup_signals(void);
t_env	*env_init(char **envp);
void	cleanup_shell(t_shell *sh);

/* Execution Functions */
int		execute_commands(t_shell *sh);
int		execute_external_command(t_com *cmd, t_shell *sh);
int		setup_redirections(t_com *cmd);
void	run_external(char **argv, t_env *env_list, t_shell *sh);
char	**env_to_array(t_env *env_list);

/* Builtin Functions */
int		ft_cd(char **argv, t_env **env_list);
int		ft_echo(char **argv);
int		ft_env(char **argv, t_env *env_list);
int		ft_export(char **argv, t_env **env_list);
int		ft_unset(char **argv, t_env **env_list);
int		ft_pwd(char **argv);
int		ft_exit_builtin(t_shell *sh, char **argv, int is_parent, int in_pipe);

/* Builtin Utils */
int		is_builtin(const char *cmd);
int		handle_builtin(char **argv, t_env **env_list, t_shell *sh);
int		handle_builtin_parent(char **argv, t_env **env_list, t_shell *sh);
int		handle_builtin_child(char **argv, t_env **env_list, t_shell *sh);

/* Environment Utils */
char	*get_env_value(char *name, t_env *env_list);
t_env	*find_env(char *name, t_env **envs);
void	env_clear(t_env **env_list);

/* Parsing Functions */
t_token	*tokenize(char *line, t_token **stack, t_shell *shell);
t_com	*init_coms(t_token **tokens, t_com **coms, t_shell *shell);
bool	line_validator(char *line, t_shell *shell);
bool	token_validator(t_token **tokens, t_shell *shell);

/* Memory Management */
void	free_args(char **args);
void	free_coms(t_com **coms);
void	free_sh_tokens(t_token **tokens);
void	free_env_list(t_env **envs);
void	free_split(char **arr);

/* String Utils */
int		ft_strcmp(const char *s1, const char *s2);

/* Debug Functions (remove in production) */
void	print_comms(t_com **coms);
void	print_tokens(t_token **tokens);

#endif