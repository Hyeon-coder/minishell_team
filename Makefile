# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/29 15:00:00 by JuHyeon           #+#    #+#              #
#    Updated: 2025/08/22 20:54:54 by juhyeonl         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = minishell
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -Iincludes -Ilibft
LDFLAGS = -lreadline
RM      = rm -f

OBJDIR  = objs

SRCS = srcs/main.c \
	srcs/utils.c \
	srcs/missing_functions.c \
	srcs/missing_functions_2.c \
	srcs/missing_functions_3.c \
	srcs/missing_functions_complete.c \
	srcs/debug_env_functions.c \
	srcs/execute/exe_utils_dir/env_utils.c \
	srcs/execute/exe_utils_dir/execute_utils.c \
	srcs/execute/builtin/ft_cd.c \
	srcs/execute/builtin/ft_echo.c \
	srcs/execute/builtin/ft_env.c \
	srcs/execute/builtin/ft_exit.c \
	srcs/execute/builtin/ft_export.c \
	srcs/execute/builtin/ft_pwd.c \
	srcs/execute/builtin/ft_unset.c \
	srcs/parsing/arg_utils.c \
	srcs/parsing/com_utils.c \
	srcs/parsing/env_expansion.c \
	srcs/parsing/free_functions.c \
	srcs/parsing/line_validator.c \
	srcs/parsing/make_args.c \
	srcs/parsing/make_coms.c \
	srcs/parsing/path_and_file_validators.c \
	srcs/parsing/print_tokens.c \
	srcs/parsing/random_utils.c \
	srcs/parsing/tokenization.c \
	srcs/parsing/token_utils.c \
	srcs/parsing/token_validator.c \
	srcs/parsing/token_dub_utils.c \
	srcs/execute/execute.c \
	srcs/execute/builtin_parent.c \
	srcs/execute/redir.c \
	srcs/execute/pipes_utils.c \
	srcs/execute/signals_child.c \
	srcs/execute/wait_utils.c \
	srcs/execute/child.c \
	srcs/execute/run_external.c \
	srcs/execute/ext_exec.c \
	srcs/execute/ext_utils.c \
	srcs/execute/run_builtin_parent_with_redirs.c \
	srcs/execute/pipeline.c \
	srcs/execute/execute_stub.c \
	srcs/execute/apply_redirs.c \
	srcs/parsing/missing_functions_fix.c \
	srcs/parsing/parsing_utils_fix.c \
	srcs/parsing/validation_quote_fix.c \
	srcs/parsing/env_init_fix.c

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