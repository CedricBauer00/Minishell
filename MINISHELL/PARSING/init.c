/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 11:44:53 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/02 11:50:56 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_shell	*init_shell_info(t_gc_list *gc_list)
{
	t_shell *shell_info;
	shell_info = do_alloc(gc_list, sizeof(t_shell), TYPE_SINGLE_PTR);
	if (!shell_info)
	{
		return (NULL);
	}
	shell_info->cur_dir = NULL;
	shell_info->old_dir = NULL;
	shell_info->my_envp = NULL;
	shell_info->last_status_exit = 0;
	return shell_info;
}

//after using this function at the beginning, this func ensures that the 'shell' is no longer 'null'
//because it has already been initialized as a static var.
//static var is initialized just only at once when it called and assigned in data area.
t_shell *get_shell(t_gc_list *gc_list)
{
	static t_shell *shell = NULL;
	if (shell == NULL)
	{
		shell = init_shell_info(gc_list);
	}
	return (shell);
}