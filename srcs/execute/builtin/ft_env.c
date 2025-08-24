/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* ft_env.c                                           :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: ljh3900 <ljh3900@student.42.fr>            +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/05/28 14:39:34 by juhyeonl          #+#    #+#             */
/* Updated: 2025/06/08 01:42:35 by ljh3900          ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

// 환경 변수 노드 이름으로 비교하는 함수
int	env_cmp(t_env *a, t_env *b)
{
	return ft_strcmp(a->name, b->name);
}

// 환경 변수 리스트를 버블 정렬하는 함수
void	sort_env_list(t_env **env_list)
{
	int		swapped;
	t_env	*ptr1;
	t_env	*lptr = NULL;

	if (env_list == NULL || *env_list == NULL)
		return;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		ptr1 = *env_list;
		while (ptr1->next != lptr)
		{
			if (env_cmp(ptr1, ptr1->next) > 0)
			{
				// 스왑 로직 (노드 자체를 스왑)
				t_env *tmp = ptr1->next;
				ptr1->next = tmp->next;
				tmp->next = ptr1;
				if (lptr == ptr1->next)
					lptr = tmp;
				*env_list = tmp;
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	}
}

static int	check_option(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '-')
			return (1);
		i++;
	}
	return (0);
}

int	ft_env(char **argv, t_env *env_list)
{
	t_env	*tmp;

	if (argv[1])
	{
		if (check_option(argv))
			ft_putstr_fd("env: options are not supported\n", 2);
		else
		{
			ft_putstr_fd("env: '", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putstr_fd("': no arguments expected\n", 2);
		}
		return (1);
	}
	
	// 출력 전에 환경 변수 리스트를 정렬
	sort_env_list(&env_list);
	
	tmp = env_list;
	while (tmp)
	{
		printf("%s=%s\n", tmp->name, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
