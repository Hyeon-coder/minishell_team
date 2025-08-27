/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/27 23:40:13 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * _ 환경변수 업데이트
 * ge_mini_shell의 update_underscore_var 로직을 minishell_team 구조에 적용
 */
static void	update_underscore_var(t_ms *ms, t_ast *node)
{
	t_ast	*last_cmd_node;
	char	*last_arg;
	char	*underscore_var;
	int		i;

	if (!node)
		return;

	// 파이프라인에서 마지막 명령어 찾기
	last_cmd_node = node;
	while (last_cmd_node->type == NODE_PIPE)
		last_cmd_node = last_cmd_node->right;

	// 명령어가 없거나 인자가 없으면 return
	if (!last_cmd_node->cmd || !last_cmd_node->cmd->full_cmd 
		|| !last_cmd_node->cmd->full_cmd[0])
		return;

	// 마지막 인자 찾기
	i = 0;
	while (last_cmd_node->cmd->full_cmd[i])
		i++;
	
	if (i == 0)
		return;
		
	last_arg = last_cmd_node->cmd->full_cmd[i - 1];

	// _ 환경변수 업데이트
	underscore_var = ft_strjoin("_=", last_arg);
	if (underscore_var)
	{
		export_var_to_envp(ms, underscore_var);
		free(underscore_var);
	}
}

/**
 * ge_mini_shell의 executor 로직을 minishell_team 구조에 적용
 * 메인 실행 함수
 */
int	executor(t_ms *ms, t_ast *node)
{
	int	status;

	if (!node)
		return (0);

	// AST 노드 타입에 따른 분기
	if (node->type == NODE_PIPE)
	{
		status = execute_pipeline(ms, node);
	}
	else if (node->type == NODE_COMMAND)
	{
		status = execute_simple_command(ms, node->cmd, 0);
	}
	else if (node->type == NODE_MISSCMD)
	{
		// 명령어 없이 리디렉션만 있는 경우
		if (node->cmd)
			status = apply_redirections_for_empty(ms, node->cmd);
		else
			status = 0;
	}
	else
	{
		status = 0;
	}

	// _ 환경변수 업데이트 (bash 호환성)
	update_underscore_var(ms, node);

	ms->exit_status = status;
	return (status);
}
