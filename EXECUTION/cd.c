/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:23 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/13 17:57:58 by jisokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static char	*get_cd_target(char **args, t_shell *shell, t_gc *gc)
{
	char	*target;

	target = NULL;
	if (args[0] == NULL || ft_strcmp(args[0], "~") == 0)
		target = find_var_in_env(shell->my_envp, "HOME", 4);
	else if (ft_strcmp(args[0], "-") == 0)
	{
		if (check_existing(shell->my_envp, "OLDPWD") < 0)
			return (target);
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
	shell->old_dir = my_getcwd();
	target = get_cd_target(args, shell, gc);
	if (!target || chdir(target) != 0)
	{
		if (args[0] && ft_strcmp(args[0], "-") == 0)
			printf("NOT SET OLDPWD\n");
		else
			printf("No such file or directory\n");
		shell->last_status_exit = 1;
		return ;
	}
	new_dir = my_getcwd();
	ft_setenv("PWD", new_dir, 1, shell);
	find = find_node(gc->temp, (char *)target);
	delete_node(&gc->temp, find);
	ft_setenv("OLDPWD", shell->old_dir, 1, shell);
	shell->cur_dir = new_dir;
	shell->old_dir = shell->cur_dir;
	printf("%s\n", shell->cur_dir);
}
