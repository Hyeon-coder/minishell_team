/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 00:35:58 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 15:36:31 by juhyeonl         ###   ########.fr       */
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
	RD_I,			/* < */
	RD_O,			/* > */
	RD_O_APPEND,	/* >> */
	HERE_DOC,		/* << */
	CD,
	EXPORT,
	ENV,
	ECHO,
	UNSET,
	EXIT,
	PATH,
	PWD,
	N,				/* -n option */
	EV,				/* Environment variable */
	LSIG			/* $? */
};

/* Structures */
typedef struct s_token
{
	char			*str;
	int				type;
	bool			sq;				/* single quote flag */
	bool			dq;				/* double quote flag */
	bool			does_exist;		/* for environment variables */
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_com
{
	char			**args;		/* 명령어와 인자들 */
	char			*path;		/* 실행 파일 경로 */
	int				type;		/* 명령어 타입 */
	char			*infile;	/* 입력 파일 */
	char			*outfile;	/* 출력 파일 */
	char			*heredoc_delimiter;	/* heredoc 구분자 */
	bool			redir_type_in;		/* 입력 리다이렉션 플래그 */
	bool			redir_type_out;		/* 출력 리다이렉션 플래그 */
	bool			append;				/* 추가 모드 플래그 */
	bool			is_piped;			/* 파이프 연결 플래그 */
	t_token			*redirections;		/* 리다이렉션 토큰들 */
	struct s_com	*next;				/* 다음 명령어 */
	struct s_com	*prev;				/* 이전 명령어 */
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
t_env	*env_init(char **envp);
void	cleanup_shell(t_shell *sh);

/* Execution Functions */
int		execute_commands(t_shell *sh);
int		execute_external_command(t_com *cmd, t_shell *sh);
int		setup_redirections(t_com *cmd);
int		apply_redirs(t_com *cmd, t_shell *sh);
void	run_external(char **args, t_env *env_list, t_shell *sh);
char	**env_to_array(t_env *env_list);

/* External Execution Functions */
void	ext_exec_direct_path(char **argv, char **envp, t_shell *sh);
void	ext_exec_with_path_search(char **argv, t_env *env_list, char **envp, t_shell *sh);

/* External Utility Functions */
char	**ext_env_to_envp(t_env *env);
int		ext_has_slash(const char *s);
char	*ext_get_path_value(t_env *env_list);
char	**ext_split_paths(const char *pathvar);
int		ext_errno_to_exit(int e);
int		ext_try_exec(const char *path, char **argv, char **envp);
void	ext_print_perm_denied(const char *cmd);
void	ext_print_no_such_file(const char *cmd);
void	ext_print_is_directory(const char *cmd);
void	ext_print_cmd_not_found(const char *cmd);

/* Signal Functions */
void	setup_signals(void);
void	set_child_signals(void);

/* Pipe Utility Functions */
void	close_pipe_pair(int p[2]);

/* Builtin Functions */
int		ft_cd(char **args, t_env **env_list);
int		ft_echo(char **args);
int		ft_env(char **args, t_env *env_list);
int		ft_export(char **args, t_env **env_list);
int		ft_unset(char **args, t_env **env_list);
int		ft_pwd(char **args);
int		ft_exit_builtin(t_shell *sh, char **args, int is_parent, int in_pipe);

/* Builtin Utils */
int		is_builtin(const char *cmd);
int		handle_builtin(char **args, t_env **env_list, t_shell *sh);
int		handle_builtin_parent(char **args, t_env **env_list, t_shell *sh);
int		handle_builtin_child(char **args, t_env **env_list, t_shell *sh);

/* Utility Functions */
void	ft_free_2d_array(char **arr);
void	err_with_cmd(char *prefix, char *cmd, char *suffix);
char	*resolve_path(char *path, t_env *env_list, int *alloc_flag);
int		is_valid_name(const char *name);
int		export_cleanup(char *name, char *value, t_env *node, int exit_code);
void	print_export_list(t_env *env_list);
char	*ft_strndup(const char *s, size_t n);
char	*get_env_value(char *name, t_env *env_list);
t_env	*find_env(char *name, t_env **envs);
void	env_clear(t_env **env_list);
bool	does_env_exist(t_token *token, t_env **envs);
t_env	*env_new(const char *name, const char *value);
int		env_add_back(t_env **lst, t_env *new_node);
t_env	*env_find(t_env *lst, const char *name);
int		env_remove(t_env **lst, const char *name);

/* Parsing Functions */
t_token	*tokenize(char *line, t_token **stack, t_shell *shell);
t_com	*init_coms(t_token **tokens, t_com **coms, t_shell *shell);
bool	line_validator(char *line, t_shell *shell);
bool	token_validator(t_token **tokens, t_shell *shell);
char	**make_args(t_token **tokens, t_shell *shell);
void	set_com_type(char *str, t_com *token);
size_t	count_args(t_token **tokens);
bool	is_separator(char c);
char	*env_parse_handler(char *str, char *name, t_shell *shell, bool got_envs);
void	setup_directors(t_com *new, t_token **tokens);
void	fill_o_dir(t_com *new, t_token *d);
void	fill_in_dir(t_com *new, t_token *d);
size_t	count_coms(t_token **tokens);

/* Token Utils */
void	set_type(t_token *token);
size_t	handle_rd(char *line);
size_t	rd_loop(char *line);
size_t	defloop(char *line);
size_t	increment_index(char *line);
void	init_token_vals(t_token *token);

/* Parsing Utils */
void	setenv_type(t_token *t);
bool	is_whitespace(char c);
size_t	handle_sq(char *line);
size_t	handle_dq(char *line);
bool	is_rd(char c);
size_t	handle_dollar(char *line);
bool	is_meta(char c);
size_t	token_dub_loop(char *line, t_token *token);
void	sq_dub(char *line, char *res, size_t l);
void	dq_dub(char *line, char *res, size_t l);

/* Memory Management */
void	free_args(char **args);
void	free_coms(t_com **coms);
void	free_sh_tokens(t_token **tokens);
void	free_env_list(t_env **envs);
void	free_split(char **arr);

/* String Utils */
char	*custom_dup(char *str);

/* Error Handling */
void	print_mem_error(char *msg, t_shell *shell);
void	write_syntax_error(char *msg, t_shell *shell);

/* Environment Utils */
char	*make_name(char *str);
char	*get_sig_val(int exit_code);

/* Path Utils */
void	token_path_setter(char *str, t_token *token);
void	com_path_setter(char *str, t_com *token);

/* Debug Functions (remove in production) */
void	print_comms(t_com **coms);
void	print_tokens(t_token **tokens);

#endif