/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_shell_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 11:44:53 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/23 14:00:15 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_shell	*init_shell_info(t_gc_list *gc_list)
{
	t_shell	*shell_info;

	shell_info = do_alloc(&gc_list, sizeof(t_shell), \
		TYPE_SINGLE_PTR, "shell_info");
	if (!shell_info)
		return (NULL);
	shell_info->cur_dir = NULL;
	shell_info->old_dir = NULL;
	shell_info->my_envp = NULL;
	shell_info->last_status_exit = 0;
	return (shell_info);
}

t_shell	*get_shell(t_gc_list *gc_list)
{
	static t_shell	*shell = NULL;

	if (shell == NULL)
		shell = init_shell_info(gc_list);
	return (shell);
}
