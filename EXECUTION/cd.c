/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:23 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/10 11:29:01 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	is_valid_dir(const char *path)
{
	struct stat	file_stat;

	if (stat(path, &file_stat) != 0)
	{
		perror("failed stat()");
		return (0);
	}
	else
	{
		if (S_ISDIR(file_stat.st_mode))
			return (1);
	}
	return (0);
}

static char	*get_cd_target(char **args, t_shell *shell, t_gc *gc)
{
	char	*target;

	if (args[0] == NULL || ft_strcmp(args[0], "~") == 0)
	{
		target = find_var_in_env(shell->my_envp, "HOME", 4);
	}
	else if (ft_strcmp(args[0], "-") == 0)
	{
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

	if (!shell)
		return ;
	shell->cur_dir = my_getcwd(gc);
	target = get_cd_target(args, shell, gc);
	if (!target || chdir(target) != 0)
	{
		perror(RED"cd () error"DEFAULT);
		shell->last_status_exit = 127;
		return ;
	}
	new_dir = my_getcwd(gc);
	find = find_node(gc->temp, (char *)target);
	delete_node(&gc->temp, find);
	shell->old_dir = shell->cur_dir;
	ft_setenv("OLDPWD", shell->cur_dir, 1, shell);
	shell->cur_dir = new_dir;
	ft_setenv("PWD", shell->cur_dir, 1, shell);
}
