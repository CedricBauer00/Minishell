/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 11:26:40 by cbauer            #+#    #+#             */
/*   Updated: 2025/05/10 11:26:59 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	**expand_envp(t_shell *shell, char *new_path)
{
	int		env_count;
	int		i;
	t_gc	*gc;
	char	**new_envp;

	i = 0;
	gc = get_gc();
	env_count = get_env_count(shell->my_envp);
	new_envp = do_alloc(&gc->shell, sizeof(char *) * (env_count + 2), \
		TYPE_DOUBLE_PTR, "new_envp");
	is_exited((char **)new_envp, gc);
	while (shell->my_envp[i])
	{
		new_envp[i] = shell->my_envp[i];
		i++;
	}
	new_envp[env_count] = new_path;
	new_envp[env_count + 1] = NULL;
	return (new_envp);
}
