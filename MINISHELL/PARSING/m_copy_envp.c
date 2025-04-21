/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_copy_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbauer < cbauer@student.42heilbronn.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:56:46 by cbauer            #+#    #+#             */
/*   Updated: 2025/04/20 12:57:42 by cbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

char	**copy_envp(t_gc_list *gc_lst, char **envp)
{
	int		count;
	char	**my_envp;
	int		i;

	i = 0;
	if (!gc_lst)
		return (NULL);
	count = get_envp_count(envp);
	my_envp = do_alloc(&gc_lst, sizeof(char *) * count, \
		TYPE_DOUBLE_PTR, "copy_envp");
	if (!my_envp)
		return (NULL);
	while (envp[i])
	{
		my_envp[i] = gc_strdup(envp[i], gc_lst);
		if (!my_envp[i])
		{
			while (--i >= 0)
			{
				free(my_envp[i]);
			}
			return (NULL);
		}
		i++;
	}
	my_envp[i -1] = NULL;
	return (my_envp);
}
