# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/29 15:00:00 by JuHyeon           #+#    #+#              #
#    Updated: 2025/08/21 23:29:31 by juhyeonl         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = minishell
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -Iincludes -Ilibft
LDFLAGS = -lreadline
RM      = rm -f

OBJDIR  = objs

# Core execution files
EXECUTE_CORE = \
	srcs/execute/core/execute.c \
	srcs/execute/core/pipeline.c \
	srcs/execute/core/child_process.c \
	srcs/execute/core/wait_process.c

# Builtin commands
EXECUTE_BUILTIN = \
	srcs/execute/builtin/ft_cd.c \
	srcs/execute/builtin/ft_echo.c \
	srcs/execute/builtin/ft_env.c \
	srcs/execute/builtin/ft_exit.c \
	srcs/execute/builtin/ft_export.c \
	srcs/execute/builtin/ft_pwd.c \
	srcs/execute/builtin/ft_unset.c \
	srcs/execute/builtin/builtin_utils.c

# External command execution
EXECUTE_EXTERNAL = \
	srcs/execute/external/ext_exec.c \
	srcs/execute/external/ext_path.c \
	srcs/execute/external/ext_utils.c \
	srcs/execute/external/ext_error.c \
	srcs/execute/external/ext_env.c

# Redirection handling
EXECUTE_REDIR = \
	srcs/execute/redirection/redir.c \
	srcs/execute/redirection/heredoc.c

# Signal handling
EXECUTE_SIGNAL = \
	srcs/execute/signal/signals.c

# Utility functions
EXECUTE_UTILS = \
	srcs/execute/utils/pipe_utils.c \
	srcs/execute/utils/builtin_parent.c \
	srcs/execute/exe_utils_dir/env_utils.c \
	srcs/execute/exe_utils_dir/export_unset_utils.c \
	srcs/execute/exe_utils_dir/cd_resolve_path.c

# Parsing files (partner's code - do not modify)
PARSING_SRCS = \
	srcs/parsing/arg_utils.c \
	srcs/parsing/com_utils.c \
	srcs/parsing/env_expansion.c \
	srcs/parsing/error_printers.c \
	srcs/parsing/free_functions.c \
	srcs/parsing/line_validator.c \
	srcs/parsing/make_args.c \
	srcs/parsing/make_coms.c \
	srcs/parsing/path_and_file_validators.c \
	srcs/parsing/print_tokens.c \
	srcs/parsing/random_utils.c \
	srcs/parsing/str_utils.c \
	srcs/parsing/tokenization.c \
	srcs/parsing/token_utils.c \
	srcs/parsing/token_validator.c \
	srcs/parsing/token_dub_utils.c \
	srcs/parsing/type_setter_utils.c

# Main and utility files
MAIN_SRCS = \
	srcs/main.c \
	srcs/utils.c

# All source files
SRCS = $(EXECUTE_CORE) $(EXECUTE_BUILTIN) $(EXECUTE_EXTERNAL) \
       $(EXECUTE_REDIR) $(EXECUTE_SIGNAL) $(EXECUTE_UTILS) \
       $(PARSING_SRCS) $(MAIN_SRCS)

OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) libft/libft.a
	$(CC) $(CFLAGS) $(OBJS) libft/libft.a $(LDFLAGS) -o $(NAME)

libft/libft.a:
	$(MAKE) -C libft

$(OBJDIR)/%.o: %.c includes/minishell.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJDIR)
	$(MAKE) -C libft clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C libft fclean

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJS)