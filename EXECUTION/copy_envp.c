/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:16:34 by jisokim2          #+#    #+#             */
/*   Updated: 2025/05/10 13:43:45 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
	my_envp = (char**)do_alloc(&gc->shell, sizeof(char *) * (count + 1), \
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
