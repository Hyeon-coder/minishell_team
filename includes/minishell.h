/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 00:35:58 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/16 01:09:26 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ======================= */
/*       System Headers    */
/* ======================= */
# include <stdio.h>
# include <errno.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <sys/stat.h>

/* ======================= */
/*    Third-party libs     */
/* ======================= */
# include <readline/readline.h>
# include <readline/history.h>

/* ======================= */
/*     Project headers     */
/* ======================= */
# include "../libft/libft.h"

/* ======================= */
/*         Macros          */
/* ======================= */
# define INITIAL_CAP 16
# define TRUE 1
# define FALSE 0
# define GRN  "\033[1;32m"
# define RESET "\033[0m"

/* ======================= */
/*      Token/Types Enum   */
/* ======================= */
enum e_Types
{
	WORD,
	PIPE,
	CD,
	EXPORT,
	ENV,
	ECHO,
	UNSET,
	EXIT,
	N,
	PWD,
	EV,
	RD_O,
	RD_O_APPEND,
	RD_I,
	HERE_DOC,
	PATH,
	REL_PATHF,
	LSIG
};

/* ======================= */
/*         Structs         */
/* ======================= */
typedef struct t_token
{
	size_t			id;
	enum e_Types	type;
	char			*str;
	int				int_val;
	bool			sq;
	bool			dq;
	struct t_token	*next;
	struct t_token	*prev;
}	t_token;

typedef struct t_com
{
	int				pipe_fd[2];
	char			**args;
	char			*path;
	char			*infile;
	char			*outfile;
	enum e_Types	type;
	bool			is_piped;
	bool			redir_type_in;
	bool			redir_type_out;
	bool			append;
	struct t_com	*next;
	struct t_com	*prev;
}	t_com;

typedef struct t_env
{
	char			*name;
	char			*value;
	struct t_env	*next;
}	t_env;

typedef struct t_shell
{
	t_com			*commands;
	t_token			*tokens;
	t_env			*envs;
	char			**lines;
	int				last_exit;
}	t_shell;

/* Stub command struct (used in temporary executor) */
typedef struct t_cmd
{
	char	**argv;
	int		fd_in;
	int		fd_out;
	int		append;
}	t_cmd;

/* ======================= */
/*        Globals          */
/* ======================= */
extern volatile sig_atomic_t	g_signal;

/* ======================= */
/*     Signals & Utils     */
/* ======================= */
void	setup_signals(void);
void	ft_free_2d_array(char **arr);
void	err_with_cmd(char *prefix, char *arg, char *suffix);

/* ======================= */
/*     Execute Layer       */
/* ======================= */
int		execute(t_shell *sh);
int		exec_pipeline(t_shell *sh, t_com *head, int n);
void	child_exec(t_com *cmd, t_shell *sh, int i, int n, int prev[2],
			int next[2]);
int		apply_redirs(t_com *cmd, t_shell *sh);
int		run_builtin_parent(t_com *cmd, t_shell *sh);
int		wait_all(pid_t *pids, int n, t_shell *sh);
void	close_pipe_pair(int p[2]);
void	set_child_signals(void);

/* Existing execute helpers */
bool	is_builtin(const char *cmd);
int		handle_builtin(char **argv, t_env **env_list, t_shell *sh);
void	run_external(char **argv, t_env *env_list, t_shell *sh);

/* Stub executor */
int		execute_stub_line(const char *line, t_shell *sh);

/* ======================= */
/*        Parsing          */
/* ======================= */
bool	does_contain_meta(t_token *token);
bool	is_pipe_or_rd(t_token *token);
bool	token_validator(t_token **tokens, t_shell *shell);
size_t	increment_index(char *line);

void	set_type(t_token *token);
ssize_t	count_token_amount(char *line);
size_t	handle_rd(char *line);
size_t	rd_loop(char *line);
size_t	defloop(char *line);

size_t	token_len(char *line, t_token *token);
char	*token_dup(char *line, t_token *token);
t_token	*make_token(char *line, t_shell *shell);
void	add_token(t_token **stack, t_token *new);
t_token	*tokenize(char *line, t_token **stack, t_shell *shell);

bool	ftstrcmp(char *s1, char *s2);
int		ftstrncmp(char *s1, char *s2, size_t n);
bool	is_meta(char c);
size_t	custom_len(char *line);
char	*custom_dup(char *line);

void	setenv_type(t_token *t);
int		is_whitespace(char c);
size_t	handle_sq(char *line);
size_t	handle_dq(char *line);
bool	is_rd(char c);

void	write_str(char *str);
void	print_tokens(t_token **tokens);

t_com	*make_com(t_token **tokens, t_shell *shell);
void	add_com(t_com *new, t_com **coms);
t_com	*make_coms(t_token **tokens, t_com **coms, t_shell *shell);
void	expand_env_com_types(t_com **coms);
t_com	*init_coms(t_token **tokens, t_com **coms, t_shell *shell);

void	set_com_type(char *str, t_com *token);
size_t	arg_mover(char *str);
char	*make_arg(char *str, t_shell *shell, bool is_dq);
char	**args_creation_loop(t_token **tokens, char **args,
			t_shell *shell, size_t ac);
char	**make_args(t_token **tokens, t_shell *shell);

bool	check_sq(char *line);
bool	check_dq(char *line);
bool	q_check_handler(char *line, char q);
size_t	q_count_handler(char *line, char q);
bool	line_validator(char *line);

void	free_args(char **args);
void	free_coms(t_com **coms);
void	free_sh_tokens(t_token **tokens);
void	free_both(t_shell *shell);

char	*joiner(char *arg, char *env, char *res, char *name);
char	*custom_join(char *arg, char *env, bool got_envs, char *name);
char	*get_sig_val(int lsig);
char	*parse_env(char *str, char *name, t_shell *shell, bool got_envs);
char	*env_parse_handler(char *str, char *name, t_shell *shell,
			bool got_envs);

size_t	count_args(t_token **tokens);
size_t	count_coms(t_token **tokens);
void	fill_o_dir(t_com *new, t_token *d);
void	fill_in_dir(t_com *new, t_token *d);
void	setup_directors(t_com *new, t_token **tokens);

t_env	*find_env(char *name, t_env **envs);
char	*make_name(char *str, bool is_dq);
size_t	get_arg_len(char *arg);
bool	check_if_exists(t_token *token, t_shell *shell);
bool	is_valid_dir(char *path, t_shell *shell);
bool	is_valid_file(t_token *token, t_shell *shell);
size_t	get_len(char *str);
size_t	move_env(char *res, char *env);
void	write_syntax_error(char *msg, t_shell *shell);
void	print_mem_error(char *msg, t_shell *shell);
void	com_path_setter(char *str, t_com *token);
void	token_path_setter(char *str, t_token *token);
void	init_token_vals(t_token *token);
bool	is_separator(char c);

/* ======================= */
/*        Builtins         */
/* ======================= */
int		ft_cd(char **argv, t_env **env_list);
char	*resolve_path(char *arg, t_env *env_list, int *alloc_flag);
int		ft_echo(char **argv, t_env *env_list, int last_exit);
int		ft_env(char **argv, t_env *env_list);
int		ft_export(char **argv, t_env **env_list);
int		ft_pwd(char **argv);
int		ft_unset(char **argv, t_env **env_list);

t_env	*env_new(const char *name, const char *value);
t_env	*env_init(char **envp);
int		env_add_back(t_env **lst, t_env *new_node);
void	env_clear(t_env **lst);
t_env	*env_find(t_env *lst, const char *name);

int		export_cleanup(char *name, char *value, t_env *node, int ret_code);
int		is_valid_name(const char *s);
void	print_export_list(t_env *env);
int		env_remove(t_env **lst, const char *name);

/* NEW builtin context-aware helpers and exit */
int		handle_builtin_parent(char **argv, t_env **env_list, t_shell *sh);
int		handle_builtin_child(char **argv, t_env **env_list, t_shell *sh);
int		ft_exit_builtin(t_shell *sh, char **argv, int is_parent, int in_pipe);

int		is_builtin_name(const char *s);
int		dispatch_builtin(t_shell *sh, t_com *cmd, int is_parent, int in_pipe);
int		run_builtin_parent_with_redirs(t_shell *sh, t_com *cmd);
/* int	ft_str_isalpha(char *str);  // remove or keep if used elsewhere */

#endif
