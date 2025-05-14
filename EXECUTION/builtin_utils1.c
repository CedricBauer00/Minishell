/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:47:59 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/14 15:19:44 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	**expand_envp(t_shell *shell, char *new_path)
{
	int		env_count;
	int		i;
	t_gc	*gc;
	char	**new_envp;

	gc = get_gc();
	env_count = get_env_count(shell->my_envp);
	new_envp = do_alloc(&gc->shell, sizeof(char *) * (env_count + 2), \
		TYPE_DOUBLE_PTR, "new_envp");
	is_exited((char **)new_envp, gc);
	i = 0;
	while (shell->my_envp[i])
	{
		new_envp[i] = shell->my_envp[i];
		i++;
	}
	new_envp[env_count] = new_path;
	new_envp[env_count + 1] = NULL;
	return (new_envp);
}

int	get_envp_count(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
	{
		count++;
	}
	return (count);
}

char	**copy_envp(t_gc *gc, char **envp)
{
	int		count;
	char	**my_envp;
	int		i;

	i = 0;
	if (!gc)
		return (NULL);
	count = get_envp_count(envp);
	my_envp = (char **)do_alloc(&gc->shell, sizeof(char *) * (count + 1), \
		TYPE_DOUBLE_PTR, "copy_envp");
	if (!my_envp)
		return (gc_free(gc), NULL);
	while (envp[i])
	{
		my_envp[i] = gc_strdup(envp[i], &gc->shell);
		if (!my_envp[i])
			gc_free(gc);
		i++;
	}
	my_envp[i] = NULL;
	return (my_envp);
}

int	is_valid_dir(const char *path)
{
	struct stat	file_stat;

	if (stat(path, &file_stat) != 0)
		return (0);
	else
	{
		if (S_ISDIR(file_stat.st_mode))
			return (1);
	}
	return (0);
}

// void	handle_no_env_minishell(t_shell	*shell)
// {
// 	int	i;

// 	i = 0;
// 	while(shell->my_envp[i])
// 	{
		//  = get_cwd(NULL);
// 		shell->my_envp[i] = gc_strdup(ft_pwd())
// 	}
// }