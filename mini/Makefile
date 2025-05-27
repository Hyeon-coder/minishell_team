# Makefile for minishell (no wildcards, parse.c 포함)

NAME        = minishell

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -Iincludes -Ilibft
LDFLAGS     = -lreadline

SRCDIR      = srcs
OBJDIR      = objs
LIBFT_DIR   = libft
LIBFT_LIB   = $(LIBFT_DIR)/libft.a

SRCS        = \
    srcs/main.c \
    srcs/utils.c \
    srcs/parse.c

OBJS        = \
    $(OBJDIR)/main.o \
    $(OBJDIR)/utils.o \
    $(OBJDIR)/parse.o

.PHONY: all clean fclean re libft

all: libft $(NAME)

# 1) libft 빌드
libft:
	@$(MAKE) -C $(LIBFT_DIR)

# 2) minishell 링크
$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(LIBFT_LIB) $(LDFLAGS) -o $(NAME)

# --- 개별 소스 컴파일 규칙 ---

$(OBJDIR)/main.o: srcs/main.c includes/minishell.h
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/utils.o: srcs/utils.c includes/minishell.h
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/parse.o: srcs/parse.c includes/minishell.h
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# --- 정리 규칙 ---

clean:
	@$(RM) -rf $(OBJDIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@$(RM) -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all
