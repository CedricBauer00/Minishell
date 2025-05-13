/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:23 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/13 10:06:00 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static char	*get_cd_target(char **args, t_shell *shell, t_gc *gc)
{
	char	*target;

	if (args[0] == NULL || ft_strcmp(args[0], "~") == 0)
		target = find_var_in_env(shell->my_envp, "HOME", 4);
	else if (ft_strcmp(args[0], "-") == 0)
	{
		if (check_existing(shell->my_envp, "OLDPWD") < 0)
		{
			printf("OLDPWD not set\n");
		}
		target = find_var_in_env(shell->my_envp, "OLDPWD", 6);
	}
	else
	{
		target = gc_strdup(args[0], &gc->temp);
		is_exited(target, gc);
	}
	return (target);
}

void	cd(char **args, t_shell *shell, t_gc *gc)
{
	char		*target;
	char		*new_dir;
	t_gc_list	*find;

	target = NULL;
	if (!shell)
		return ;
	shell->cur_dir = my_getcwd();
	get_cd_target(args, shell, gc);
	if (!target || chdir(target) != 0)
	{
		shell->last_status_exit = 1;
		return ;
	}
	new_dir = my_getcwd();
	ft_setenv("PWD", new_dir, 1, shell);
	shell->cur_dir = new_dir;
	find = find_node(gc->temp, (char *)target);
	delete_node(&gc->temp, find);
	shell->old_dir = shell->cur_dir;
	if (check_existing(shell->my_envp, "OLDPWD") > 0)
		ft_setenv("OLDPWD", shell->cur_dir, 1, shell);
	printf("%s\n", shell->cur_dir);
}
