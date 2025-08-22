/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_env_functions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:00:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/22 20:35:19 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_print_env_list(t_env *env_list, char *title)
{
	t_env	*current;
	int		count;

	printf("[DEBUG ENV] %s:\n", title);
	current = env_list;
	count = 0;
	while (current)
	{
		printf("[DEBUG ENV] [%d] %s=%s (addr: %p)\n", 
			count, current->name, current->value, current);
		current = current->next;
		count++;
	}
	printf("[DEBUG ENV] Total: %d variables\n", count);
}

void	debug_export_operation(char *name, char *value, t_env **env_list)
{
	printf("[DEBUG EXPORT] Before export: %s=%s\n", name, value);
	debug_print_env_list(*env_list, "Before export");
}

void	debug_find_operation(char *name, t_env **env_list)
{
	t_env	*found;

	printf("[DEBUG FIND] Looking for: %s\n", name);
	found = find_env(name, env_list);
	if (found)
		printf("[DEBUG FIND] Found: %s=%s (addr: %p)\n", 
			found->name, found->value, found);
	else
		printf("[DEBUG FIND] Not found: %s\n", name);
}
