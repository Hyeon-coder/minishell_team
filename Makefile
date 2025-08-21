# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/29 15:00:00 by JuHyeon           #+#    #+#              #
#    Updated: 2025/08/22 01:43:42 by juhyeonl         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = minishell
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -Iincludes -Ilibft
DEBUG_FLAGS = -g3 -fsanitize=address -fsanitize=undefined
LDFLAGS = -lreadline
RM      = rm -f

OBJDIR  = objs

# Source files organized by category
EXECUTE_CORE = \
	srcs/execute/core/execute.c \
	srcs/execute/core/pipeline.c \
	srcs/execute/core/child_process.c \
	srcs/execute/core/wait_process.c

EXECUTE_BUILTIN = \
	srcs/execute/builtin/ft_cd.c \
	srcs/execute/builtin/ft_echo.c \
	srcs/execute/builtin/ft_env.c \
	srcs/execute/builtin/ft_exit.c \
	srcs/execute/builtin/ft_export.c \
	srcs/execute/builtin/ft_pwd.c \
	srcs/execute/builtin/ft_unset.c \
	srcs/execute/builtin/builtin_utils.c

EXECUTE_EXTERNAL = \
	srcs/execute/external/ext_exec.c \
	srcs/execute/external/ext_path.c \
	srcs/execute/external/ext_utils.c \
	srcs/execute/external/ext_error.c \
	srcs/execute/external/ext_env.c

EXECUTE_REDIR = \
	srcs/execute/redirection/redir.c \
	srcs/execute/redirection/heredoc.c

EXECUTE_SIGNAL = \
	srcs/execute/signal/signals.c

EXECUTE_UTILS = \
	srcs/execute/utils/pipe_utils.c \
	srcs/execute/utils/builtin_parent.c \
	srcs/execute/exe_utils_dir/env_utils.c \
	srcs/execute/exe_utils_dir/export_unset_utils.c \
	srcs/execute/exe_utils_dir/cd_resolve_path.c

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

MAIN_SRCS = \
	srcs/main.c \
	srcs/utils.c

# All source files
SRCS = $(EXECUTE_CORE) $(EXECUTE_BUILTIN) $(EXECUTE_EXTERNAL) \
       $(EXECUTE_REDIR) $(EXECUTE_SIGNAL) $(EXECUTE_UTILS) \
       $(PARSING_SRCS) $(MAIN_SRCS)

OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

# Default target
all: $(NAME)

# Debug target with sanitizers
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(NAME)

# Production build
$(NAME): $(OBJS) libft/libft.a
	$(CC) $(CFLAGS) $(OBJS) libft/libft.a $(LDFLAGS) -o $(NAME)

# Build libft
libft/libft.a:
	$(MAKE) -C libft

# Compile object files
$(OBJDIR)/%.o: %.c includes/minishell.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Memory leak check with valgrind
valgrind: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--suppressions=readline.supp ./$(NAME)

# Create readline suppression file
readline.supp:
	@echo "# Readline suppression file" > readline.supp
	@echo "{" >> readline.supp
	@echo "   ignore_readline_leaks" >> readline.supp
	@echo "   Memcheck:Leak" >> readline.supp
	@echo "   ..." >> readline.supp
	@echo "   fun:readline" >> readline.supp
	@echo "}" >> readline.supp
	@echo "{" >> readline.supp
	@echo "   ignore_add_history_leaks" >> readline.supp
	@echo "   Memcheck:Leak" >> readline.supp
	@echo "   ..." >> readline.supp
	@echo "   fun:add_history" >> readline.supp
	@echo "}" >> readline.supp

# Test target
test: $(NAME)
	@echo "Running basic tests..."
	@echo "echo hello" | ./$(NAME)
	@echo "pwd" | ./$(NAME)
	@echo "env | head -5" | ./$(NAME)

# Clean object files
clean:
	$(RM) -r $(OBJDIR)
	$(MAKE) -C libft clean

# Clean everything
fclean: clean
	$(RM) $(NAME)
	$(RM) readline.supp
	$(MAKE) -C libft fclean

# Rebuild everything
re: fclean all

# Help target
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  debug    - Build with debug flags and sanitizers"
	@echo "  valgrind - Run valgrind memory check"
	@echo "  test     - Run basic functionality tests"
	@echo "  clean    - Remove object files"
	@echo "  fclean   - Remove all generated files"
	@echo "  re       - Rebuild everything"
	@echo "  help     - Show this help message"

.PHONY: all debug clean fclean re test valgrind help
.SECONDARY: $(OBJS)