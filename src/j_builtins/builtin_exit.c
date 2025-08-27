/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/27 23:36:34 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * ge_mini_shell의 숫자 검사 로직
 * +/- 부호와 숫자만 허용
 */
static int	is_numeric_string(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	
	i = 0;
	// 첫 문자가 +/- 부호인 경우 건너뛰기
	if (str[i] == '+' || str[i] == '-')
		i++;
	
	// 부호만 있고 숫자가 없는 경우
	if (str[i] == '\0')
		return (0);
	
	// 나머지는 모두 숫자여야 함
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	
	return (1);
}

/**
 * ge_mini_shell 로직을 적용한 exit 구현
 * minishell_team의 bi_exit 함수 활용
 */
int	builtin_exit(t_ms *ms, char **args)
{
	int	exit_code;

	// "exit" 메시지 출력 (터미널에서 실행 중일 때만)
	if (isatty(STDOUT_FILENO))
		ft_putendl_fd("exit", STDERR_FILENO);

	// 인자가 없으면 현재 exit status로 종료
	if (!args[1])
	{
		bi_exit(ms, ms->exit_status, 0);
		return (0); // 실제로는 도달하지 않음
	}

	// 첫 번째 인자가 숫자가 아닌 경우
	if (!is_numeric_string(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		bi_exit(ms, 2, 0);
		return (0); // 실제로는 도달하지 않음
	}

	// 인자가 너무 많은 경우 (2개 이상)
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		ms->exit_status = 1;
		return (1); // exit하지 않고 에러 상태로 계속 실행
	}

	// 정상적인 경우: exit code 계산 및 종료
	exit_code = ft_atoi(args[1]);
	// unsigned char로 변환 (0-255 범위로 제한)
	exit_code = (unsigned char)exit_code;
	
	bi_exit(ms, exit_code, 0);
	return (0); // 실제로는 도달하지 않음
}
