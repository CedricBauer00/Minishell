/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokim2 <jisokim2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:47:59 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/14 15:39:16 by jisokim2         ###   ########.fr       */
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

void	handle_no_env_minishell(t_shell	*shell, t_gc *gc)
{
	int		i;
	char	*temp;
	
	i = 0;
	temp = getcwd(NULL, 0);
	
	shell->my_envp = do_alloc(&gc->shell, sizeof(char *) * 5, TYPE_DOUBLE_PTR, "shell");
	char *pwd = gc_strjoin("PWD=", temp, &gc->temp);
	shell->my_envp[0] = gc_strdup(pwd, &gc->shell);
	free(temp);
	shell->my_envp[1] = gc_strdup("SHLVL=0", &gc->shell);
	shell->my_envp[2] = gc_strdup("OLDPWD", &gc->shell);
	shell->my_envp[3] = gc_strdup("_=/usr/bin/env", &gc->shell);
	shell->my_envp[4] = NULL;
}