/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljh3900 <ljh3900@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:18:18 by ljh3900           #+#    #+#             */
/*   Updated: 2025/05/25 23:18:20 by ljh3900          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

/* 전역 시그널 플래그 (최대 1개) */
extern volatile sig_atomic_t g_signal;

/* 사용자 구현 부분 */
char    **tokenize(char *line);
void    execute(char **args);
void    free_tokens(char **args);

void    setup_signals(void);

#endif
